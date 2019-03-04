//
// Created by daniel on 1/4/19.
//

#ifndef STRANGER_THINGS_UTILITIES_H
#define STRANGER_THINGS_UTILITIES_H


#define N_OF_BARBERS 3
#define N_OF_CHAIRS 2
#define QUEUE_SIZE 5
#define N_OF_CUSTOMERS 8
#define HAIRCUT_PRICE 8

//Time constants | time shall be given in seconds
#define MAX_TIME_OF_CUSTOMER_WORKING 8
#define TIME_OF_HAIRCUT 6


struct Utils {
    int queue_msg, customer_msg, sleeping_barbers, free_chairs, customers_processing, queue_sem, queue_lock, cashbox_msg, cashbox_lock;

    //shared memories
    int *cashbox;
    int (*wallets)[N_OF_CUSTOMERS];
};

struct msgbuf {
    long mtype; //typ komunikatu
    int mvalue; //treść komunikatu
};


bool sem_down_nowait(int semid, int semnum);

void sem_down_wait(int semid, int semnum);

void sem_up(int semid, int semnum);

struct Utils utils_initializer();

void copy_arr(int *copy, const int *to_copy, int elems);

void msg_clear(int id);

void queue_lock_reset(int semid);

void queue_lock_down(int semid, int semnum);


#endif //STRANGER_THINGS_UTILITIES_H
