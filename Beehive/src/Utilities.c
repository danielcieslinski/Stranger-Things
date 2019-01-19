//#include <sys/types.h>
//#include <sys/ipc.h>

#include <sys/msg.h> //Message Queue
#include <sys/sem.h> //Semaphores
//#include <libzvbi.h> //NULL macro
#include <sys/shm.h> //Shared memo
#include <zconf.h> //Fork
#include <stdbool.h>

#include "Utilities.h"
#include "Bees.h"

static struct sembuf buf;

struct Utils utils_initializer() {
    struct Utils utils;

    utils.storage_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); // Holds info on free chairs in waiting room
    semctl(utils.storage_sem, 0, SETVAL, STORAGE_CAPACITY);

    utils.game_status = (struct GameStatus *) shmat(shmget(0, sizeof(struct GameStatus), IPC_CREAT | IPC_EXCL | 0660), NULL, 0);

    utils.bear_attack = (bool *) shmat(shmget(0, sizeof(bool), IPC_CREAT | IPC_EXCL | 0660), NULL, 0);
    utils.bear_attack = false;

    utils.bees = shmat(shmget(IPC_PRIVATE, sizeof(pid_t[3][MAX_BEES_OF_ONE_KIND]), IPC_CREAT | IPC_EXCL | 0660), 0, 0);

    return utils;

};


//Check if game is finished
void game_over(struct Utils * utils){
    if (utils->game_status->workers == 0 && utils->game_status->honey ==0){

        

    }



}


void sem_down_wait(int semid) {
    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

void sem_up(int semid) {
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
}

