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


int n_of_barbers = 5;
int n_of_chairs = 5;
int capacity_of_waiting_room = 5;
int n_of_customers = 5;

//Time constants | time shall be given in seconds
int max_time_of_customer_working = 5;

struct Utils {
    int queue_msg, customer_msg;
};

struct msgbuf {
    long mtype; //typ komunikatu
    int mvalue; //treść komunikatu
};

struct Utils utils_initializer() {

    struct Utils utils;
    utils.customer_msg=msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); //

    printf("utils initislized \n");

    return utils;

};

void barber(int barber_id, struct Utils utils) {

    printf("New barber %d \n", barber_id);
    struct msgbuf message;

    while (true) {
        while(msgrcv(utils.customer_msg, &message, sizeof(message.mvalue), 1, IPC_NOWAIT) == -1) {}
        printf("Barber %d gives new haircut to %d customer \n",barber_id,  message.mvalue);
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
        msgsnd(utils.customer_msg, &message, sizeof(message.mvalue), 0);
    }
}


int main() {

    int to_gen = n_of_barbers + n_of_customers;
    struct Utils utils = utils_initializer();

//    int customer_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600);
//
//    int sleeping_barbers_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); //wolni fryzjerzy
//    semctl(sleeping_barbers_sem, 0, SETVAL, B);
//
//    int chair_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); //wolne krzesła
//    semctl(chair_sem, 0, SETVAL, N);
//
//    int customers_sem = semget(IPC_PRIVATE, C, IPC_CREAT | IPC_EXCL | 0600); //0 podczas obługi
//    int queue_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); //Clients queue

    for (int i = 0; i < to_gen; i++) {
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