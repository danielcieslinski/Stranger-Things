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
    semctl(utils.free_chairs, 0, SETVAL, n_of_chairs);

    utils.customers_processing = semget(IPC_PRIVATE, n_of_customers, IPC_CREAT | IPC_EXCL |
                                                                     0600); // Holds info about giving haircut to every customer
    semctl(utils.customers_processing, 0, SETALL, 0); //Second arg is ignored

    utils.queue_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Holds info on free chairs in waiting room
    semctl(utils.queue_sem, 0, SETVAL, queue_size);

    utils.queue_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Customers in queue

    utils.queue_lock = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Holds info on free chairs in waiting room
    semctl(utils.queue_lock, 0, SETVAL, 0);

    return utils;

};

bool sem_down_wait(int semid, int semnum) {
    buf.sem_num = (unsigned short) semnum;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1) {
        printf("Błąd opuszczenia semafora \n");
    }
}

bool sem_down_nowait(int semid, int semnum) {
    buf.sem_num = (unsigned short) semnum;
    buf.sem_op = -1;
    buf.sem_flg = IPC_NOWAIT;
    return ((semop(semid, &buf, 1) != -1));
    //-1 when cant push down
    //Returns true if sem was decremented, otherwise false
}

bool sem_up(int semid, int semnum) {
    buf.sem_num = (unsigned short) semnum;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    if (semop(semid, &buf, 1) == -1) {
        printf("Błąd podniesienia semafora \n");
        perror(" Opuszczenie semafora ");
        perror(errno);
    }
}
