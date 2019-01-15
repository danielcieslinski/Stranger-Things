//
// Created by daniel on 1/4/19.
//

#ifndef STRANGER_THINGS_UTILITIES_H
#define STRANGER_THINGS_UTILITIES_H


#define n_of_barbers 3
#define n_of_chairs 2
#define queue_size 5
#define n_of_customers 8
#define haircut_price 8

//Time constants | time shall be given in seconds
#define max_time_of_customer_working 10
#define time_of_haircut 5


struct Utils {
    int queue_msg, customer_msg, sleeping_barbers, free_chairs, customers_processing, queue_sem, queue_lock, cashbox_msg;

    //shared memories
    int *cashbox;
    int (*wallets)[n_of_customers];
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

#endif //STRANGER_THINGS_UTILITIES_H
