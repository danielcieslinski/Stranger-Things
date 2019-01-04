#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sched.h>
#include <zconf.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include "Utilities.h"


void haircut(int barber_id, int customer_id, struct Utils utils) {
    printf("Barber %d gives new haircut to %d customer \n", barber_id, customer_id);
    sem_down_wait(utils.free_chairs, 0); //Wait for chair

    /*Cash will come here */

    sleep(time_of_haircut); //Give haircut

    sem_up(utils.free_chairs, 0); //Free chair
    sem_up(utils.customers_processing, customer_id); //Free customer


    /* Change counting comes here */

    printf("Barber %d finished haircut to %d customer \n", barber_id, customer_id);
}

int barber_check_queue(struct Utils utils){
    sem_down_wait(utils.sleeping_barbers, 0);
    struct msgbuf message;

    if (msgrcv(utils.queue_msg, &message, sizeof(message.mvalue), 1, IPC_NOWAIT) == -1)
        return -1;

    sem_up(utils.queue_sem, 0);
    return message.mvalue;
}

//msgrecv returs -1 if there is nothing in to recv

void barber(int barber_id, struct Utils utils) {

    printf("New barber %d \n", barber_id);
    struct msgbuf message;

    while (true) {
        int customer_to_cut = barber_check_queue(utils);
        if (customer_to_cut == -1) {
            sem_up(utils.sleeping_barbers, 0);
            while (msgrcv(utils.customer_msg, &message, sizeof(message.mvalue), 1, IPC_NOWAIT) == -1); //If nobody in queue, sleep
            customer_to_cut = message.mvalue;
        }

        haircut(barber_id, customer_to_cut, utils);
    }

}

void customer_works(int *wallet) {
    for (int i = 0; i < 3; ++i)
        wallet[i] += rand() % 3 + 1;

    sleep(rand() % max_time_of_customer_working + 1);
}


void customer(int customer_id, struct Utils utils) {
    srand(time(NULL) + customer_id);

    printf("New customer %d\n", customer_id);
    int wallet[] = {0, 0, 0};

    //Message
    struct msgbuf message;
    message.mtype = 1;
    message.mvalue = customer_id;

    while (true) {
        customer_works(wallet);
        printf("Customer %d is ready and wants haircut \n", customer_id);

        if (sem_down_nowait(utils.sleeping_barbers, 0)) { //wake up barber, if no free skip
            printf("Customer %d wakes up barber \n", customer_id);
            msgsnd(utils.customer_msg, &message, sizeof(message.mvalue), 0);
            sem_down_wait(utils.customers_processing, customer_id);
        } else {
            if(sem_down_nowait(utils.queue_sem, 0)){
                //There is place in queue
                printf("Customer %d waits in queue \n", customer_id);
                msgsnd(utils.queue_msg , &message, sizeof(message.mvalue), 0);
                sem_down_wait(utils.customers_processing, customer_id); //Waits in queue and is given haircut
                sem_up(utils.customers_processing, customer_id);
            }
            printf("Skiping queue %d \n", customer_id);
        }


    }
}


int main() {

    int to_gen = n_of_barbers + n_of_customers;
    struct Utils utils = utils_initializer();
    //semget drugi argument to ilosc w tablicy

//    int customer_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600);
//
//    int sleeping_barbers_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600);
//
//    int chair_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); //wolne krzesła
//    semctl(chair_sem, 0, SETVAL, N);
//
//    int customers_sem = semget(IPC_PRIVATE, C, IPC_CREAT | IPC_EXCL | 0600); //0 podczas obługi
//    int queue_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); //Clients queue

    for (int i = 0; i < to_gen; i++) {
        sleep(1);
        if (fork() == 0) {

            if (i < n_of_barbers)
                barber(i, utils);

            else customer(i - n_of_barbers, utils);

            break;
        }
    }

    // Don't allow to leave orphans
    sleep(30000);
    //

    return 0;
}