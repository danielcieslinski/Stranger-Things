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


static struct sembuf buf;

#include "Utilities.h"


struct Utils utils_initializer() {
    struct Utils utils;
    utils.customer_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Customers that want haircut

    utils.sleeping_barbers = semget(IPC_PRIVATE, 1,
                                    IPC_CREAT | IPC_EXCL | 0600); // Holds info on how many barbers are free
    semctl(utils.sleeping_barbers, 0, SETVAL, 0);


    utils.free_chairs = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Holds info on how many chairs are
    semctl(utils.free_chairs, 0, SETVAL, N_OF_CHAIRS);

    utils.customers_processing = semget(IPC_PRIVATE, N_OF_CUSTOMERS, IPC_CREAT | IPC_EXCL |
                                                                     0600); // Holds info about giving haircut to every customer
    semctl(utils.customers_processing, 0, SETALL, 0); //Second arg is ignored

    utils.queue_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Holds info on free chairs in waiting room
    semctl(utils.queue_sem, 0, SETVAL, QUEUE_SIZE);

    utils.queue_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Customers in queue

    utils.cashbox_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Customers in queue

    utils.queue_lock = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Lock while queue check
    semctl(utils.queue_lock, 0, SETVAL, 1);

    utils.cashbox = (int *) shmat(shmget(IPC_PRIVATE, 3 * sizeof(int), IPC_CREAT | IPC_EXCL | 0660), NULL, 0);

    utils.wallets = shmat(shmget(IPC_PRIVATE, sizeof(int[N_OF_CUSTOMERS][3]), IPC_CREAT | IPC_EXCL | 0660), 0, 0);

    return utils;

};

void queue_lock_reset(int semid) {
    semctl(semid, 0, SETVAL, 0);
}

void queue_lock_down(int semid, int semnum) {
    buf.sem_num = (unsigned short) semnum;
    buf.sem_op = 0;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

void sem_down_wait(int semid, int semnum) {
    buf.sem_num = (unsigned short) semnum;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

void sem_up(int semid, int semnum) {
    buf.sem_num = (unsigned short) semnum;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

bool sem_down_nowait(int semid, int semnum) {
    buf.sem_num = (unsigned short) semnum;
    buf.sem_op = -1;
    buf.sem_flg = IPC_NOWAIT;
    return ((semop(semid, &buf, 1) != -1));
    //-1 when cant push down
    //Returns true if sem was decremented, otherwise false
}

void copy_arr(int *copy, const int *to_copy, int elems) {
    for (int i = 0; i < elems; i++)
        copy[i] = to_copy[i];
}

void msg_clear(int id) {
    struct msgbuf message;
    int cleared = 0;
    while (msgrcv(id, &message, sizeof(message.mvalue), 1, IPC_NOWAIT) != -1) { cleared++; }
//    printf("Cleared, %d \n", cleared);
}
