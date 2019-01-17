//#include <sys/types.h>
//#include <sys/ipc.h>

#include <sys/msg.h> //Message Queue
#include <sys/sem.h> //Semaphores
#include <libzvbi.h> //NULL macro
#include <sys/shm.h> //Shared memo

#include "Utilities.h"

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
//    utils.queue_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Holds info on free chairs in waiting room
//    semctl(utils.queue_sem, 0, SETVAL, QUEUE_SIZE);
//
//    utils.queue_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Customers in queue
//
//    utils.cashbox_msg = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | 0600); // Customers in queue
//
//    utils.queue_lock = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Lock while queue check
//    semctl(utils.queue_lock, 0, SETVAL, 1);

    utils.game_status = (struct GameStatus *) shmat(shmget(IPC_PRIVATE, sizeof(struct GameStatus), IPC_CREAT | IPC_EXCL | 0660), NULL, 0);

//    utils.wallets = shmat(shmget(IPC_PRIVATE, sizeof(int[N_OF_CUSTOMERS][3]), IPC_CREAT | IPC_EXCL | 0660), 0, 0);

    return utils;

};