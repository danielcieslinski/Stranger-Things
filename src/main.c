#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sched.h>
#include <zconf.h>
#include <signal.h>
#include <errno.h>
#include <mqueue.h>

#include "Utilities.h"


void calculate_change(int customer_id, int to_change, struct Utils utils) {
    int denominations[3] = {1, 2, 5};

    printf("Calculating change %d, for customer %d \n", to_change, customer_id);
    msg_clear(utils.cashbox_msg);

    printf("Cashbox %d %d %d \n", utils.cashbox[0], utils.cashbox[1], utils.cashbox[2]);

    while (to_change != 0) {

        for (int i = 2; i >= 0; i--) {
            while (to_change - denominations[i] >= 0 && utils.cashbox[i] > 0) {
                to_change -= denominations[i];
                utils.wallets[customer_id][i]++;
                utils.cashbox[i]--;
            }
        }

        struct msgbuf message;
        if (to_change > 0)
            while (msgrcv(utils.cashbox_msg, &message, sizeof(message.mvalue), 1, IPC_NOWAIT) ==
                   -1) {} //Wait until new payment
    }
}

int take_payment(int customer_id, struct Utils utils) {
    int denominations[3] = {1, 2, 5};
    int to_pay = (int) HAIRCUT_PRICE;

    //copy wallet in case of trying many permutations
    int wallet_copy[3], cashbox_copy[3];
    copy_arr(wallet_copy, utils.wallets[customer_id], 3);
    copy_arr(cashbox_copy, utils.cashbox, 3);

    //Try the exact
    for (int i = 2; i >= 0; i--) {
        while (to_pay - denominations[i] >= 0 && wallet_copy[i] > 0) {
            to_pay -= denominations[i];
            wallet_copy[i]--;
            cashbox_copy[i]++;
        }
    }

    //Check if could pay, if couldn't, pay more
    if (to_pay != 0) {
        to_pay = (int) HAIRCUT_PRICE;
        copy_arr(wallet_copy, utils.wallets[customer_id], 3);
        copy_arr(cashbox_copy, utils.cashbox, 3);
        for (int i = 2; i >= 0; i--) {
            while (to_pay > 0 && wallet_copy[i] > 0) {
                to_pay -= denominations[i];
                wallet_copy[i]--;
                cashbox_copy[i]++;
            }
        }
    }

    for (int i = 0; i < 3; i++)
        utils.cashbox[i] = cashbox_copy[i];

    for (int i = 0; i < 3; i++)
        utils.wallets[customer_id][i] = wallet_copy[i];

    /* Add msg send here */
    struct msgbuf message;
    message.mtype = 1;
    message.mvalue = 1;
    msgsnd(utils.cashbox_msg, &message, sizeof(message.mvalue), 0);

    return abs(to_pay);
}

void haircut(int barber_id, int customer_id, struct Utils utils) {
    printf("Barber %d gives new haircut to %d customer \n", barber_id, customer_id);
    sem_down_wait(utils.free_chairs, 0); //Wait for chair
    printf("Barber %d found chair \n", barber_id);

    int to_change = take_payment(customer_id, utils);
    printf("Customer %d, to change, %d \n", customer_id, to_change);

    sleep(TIME_OF_HAIRCUT); //Give haircut

    sem_up(utils.free_chairs, 0); //Free chair

    /* Change counting comes here */
    calculate_change(customer_id, to_change, utils);

    printf("Barber %d finished haircut to %d customer \n", barber_id, customer_id);
    sem_up(utils.customers_processing, customer_id); //Free customer
}

int barber_check_queue(struct Utils utils) {
    sem_up(utils.queue_lock, 0);

    struct msgbuf message;

    if (msgrcv(utils.queue_msg, &message, sizeof(message.mvalue), 1, IPC_NOWAIT) == -1)
        return -1;

    sem_up(utils.queue_sem, 0);
    return message.mvalue;
}

void barber(int barber_id, struct Utils utils) {

    printf("New barber %d \n", barber_id);
    struct msgbuf message;

    while (true) {
        int customer_to_cut = barber_check_queue(utils);
        queue_lock_reset(utils.queue_lock); //Very weird behaviour, pass when 0, wait when 1

        if (customer_to_cut == -1) {
            sem_up(utils.sleeping_barbers, 0);
            printf("Barber %d goes to sleep \n", barber_id);

//            struct msqid_ds attr;
//            msgctl(utils.queue_msg,IPC_STAT, &attr);
//
//            printf("In queue: %d \n",(int) attr.msg_qnum);

            while (msgrcv(utils.customer_msg, &message, sizeof(message.mvalue), 1, IPC_NOWAIT) ==
                   -1); //If nobody in queue, sleep
            customer_to_cut = message.mvalue;
            printf("Customer %d wakes up barber %d \n", customer_to_cut, barber_id);
        }

        haircut(barber_id, customer_to_cut, utils);
    }

}

void customer_works(int customer_id, struct Utils utils) {
    for (int i = 0; i < 3; ++i)
        utils.wallets[customer_id][i] += rand() % 3;

//    printf("Customers %d wallet %d %d %d \n", customer_id, utils.wallets[customer_id][0], utils.wallets[customer_id][1],
//           utils.wallets[customer_id][2]);

    sleep(rand() % MAX_TIME_OF_CUSTOMER_WORKING + 1);
}


void customer(int customer_id, struct Utils utils) {
    srand(time(NULL) + customer_id);

    printf("New customer %d\n", customer_id);
//    int wallet[] = {0, 0, 0};

    //Message
    struct msgbuf message;
    message.mtype = 1;
    message.mvalue = customer_id;

    while (true) {
        customer_works(customer_id, utils);
        printf("Customer %d is ready and wants haircut \n", customer_id);
        queue_lock_down(utils.queue_lock, 0);
        if (sem_down_nowait(utils.sleeping_barbers, 0)) { //wake up barber, if no free skip
            msgsnd(utils.customer_msg, &message, sizeof(message.mvalue), 0);
            sem_down_wait(utils.customers_processing, customer_id);
        } else {
            if (sem_down_nowait(utils.queue_sem, 0)) {
                //There is place in queue
                printf("Customer %d waits in queue \n", customer_id);
                msgsnd(utils.queue_msg, &message, sizeof(message.mvalue), 0);
                sem_down_wait(utils.customers_processing, customer_id); //Waits in queue and is given haircut
            } else printf("Customer %d skiping queue \n", customer_id);
        }
    }
}


int main() {
    int to_gen = N_OF_BARBERS + N_OF_CUSTOMERS;
    struct Utils utils = utils_initializer();

    for (int i = 0; i < to_gen; i++) {
        if (fork() == 0) {
            if (i < N_OF_BARBERS)
                barber(i, utils);

            else customer(i - N_OF_BARBERS, utils);
        }
    }
    // Don't allow to leave orphans
    sleep(30000);
    return 0;
}

