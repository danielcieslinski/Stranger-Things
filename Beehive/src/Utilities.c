//#include <sys/types.h>
//#include <sys/ipc.h>

#include <sys/msg.h> //Message Queue
#include <sys/sem.h> //Semaphores
#include <libzvbi.h> //NULL macro
#include <sys/shm.h> //Shared memo
#include <zconf.h> //Fork
#include <stdbool.h>

#include "Utilities.h"
#include "Bees.h"

static struct sembuf buf;

struct Utils utils_initializer() {
    struct Utils utils;

//    utils.customer_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Customers that want haircut
//
//    utils.sleeping_barbers = semget(IPC_PRIVATE, 1,
//                                    IPC_CREAT | IPC_EXCL | 0600); // Holds info on how many barbers are free
//    semctl(utils.sleeping_barbers, 0, SETVAL, 0);
//
//
//    utils.free_chairs = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Holds info on how many chairs are
//    semctl(utils.free_chairs, 0, SETVAL, N_OF_CHAIRS);
//
//    utils.customers_processing = semget(IPC_PRIVATE, N_OF_CUSTOMERS, IPC_CREAT | IPC_EXCL |
//                                                                     0600); // Holds info about giving haircut to every customer
//    semctl(utils.customers_processing, 0, SETALL, 0); //Second arg is ignored
//
    utils.storage_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Holds info on free chairs in waiting room
    semctl(utils.storage_sem, 0, SETVAL, STORAGE_CAPACITY);
//
    utils.out_monitor_notifications = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Notify change to out monitor
    utils.input_monitor_notifications = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Notify change to input monitor
//
//    utils.cashbox_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Customers in queue
//
//    utils.queue_lock = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Lock while queue check
//    semctl(utils.queue_lock, 0, SETVAL, 1);

    utils.game_status = (struct GameStatus *) shmat(shmget(0, sizeof(struct GameStatus), IPC_CREAT | IPC_EXCL | 0660), NULL, 0);

    utils.bear_attack = (bool *) shmat(shmget(0, sizeof(bool), IPC_CREAT | IPC_EXCL | 0660), NULL, 0);
    utils.bear_attack = false;

    utils.bees = shmat(shmget(IPC_PRIVATE, sizeof(int[MAX_BEES_OF_ONE_KIND][3]), IPC_CREAT | IPC_EXCL | 0660), 0, 0);

    return utils;

};

void refresh_monitor(struct Utils * utils){
    if(fork() == 0)
        output_monitor(utils);
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

//bool sem_down_nowait(int semid, int semnum) {
//    buf.sem_num = (unsigned short) semnum;
//    buf.sem_op = -1;
//    buf.sem_flg = IPC_NOWAIT;
//    return ((semop(semid, &buf, 1) != -1));
//    //-1 when cant push down
//    //Returns true if sem was decremented, otherwise false
//}
