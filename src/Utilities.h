//
// Created by daniel on 1/4/19.
//

#ifndef STRANGER_THINGS_UTILITIES_H
#define STRANGER_THINGS_UTILITIES_H


#define n_of_barbers 3
#define n_of_chairs 2
#define queue_size 5
#define n_of_customers 10

//Time constants | time shall be given in seconds
#define max_time_of_customer_working 10
#define time_of_haircut 5

struct Utils {
    int queue_msg, customer_msg, sleeping_barbers, free_chairs, customers_processing, queue_sem, queue_lock;
};

struct msgbuf {
    long mtype; //typ komunikatu
    int mvalue; //treść komunikatu
};


bool sem_down_nowait(int semid, int semnum);

bool sem_down_wait(int semid, int semnum);

bool sem_up(int semid, int semnum);

struct Utils utils_initializer();

#endif //STRANGER_THINGS_UTILITIES_H
