//
// Created by daniel on 1/4/19.
//

#ifndef STRANGER_THINGS_UTILITIES_H
#define STRANGER_THINGS_UTILITIES_H


#define n_of_barbers 3
#define n_of_chairs 2
#define capacity_of_waiting_room 5
#define n_of_customers 5

//Time constants | time shall be given in seconds
#define max_time_of_customer_working 5
#define time_of_haircut 3

struct Utils {
    int queue_msg, customer_msg, sleeping_barbers, free_chairs, customers_processing;
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
