#include <sys/msg.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <zconf.h>
#include <stdbool.h>

#include "Utilities.h"
#include "Bees.h"

static struct sembuf buf;

struct Utils utils_initializer() {
    struct Utils utils;

    utils.storage_sem = semget(IPC_PRIVATE, 1, IPC_CREAT | IPC_EXCL | 0600); /* Holds info on free chairs in waiting room */
    semctl(utils.storage_sem, 0, SETVAL, STORAGE_CAPACITY);

    utils.game_status = (struct GameStatus *) shmat(shmget(0, sizeof(struct GameStatus), IPC_CREAT | IPC_EXCL | 0660), NULL, 0);

    utils.bear_attack = (bool *) shmat(shmget(0, sizeof(bool), IPC_CREAT | IPC_EXCL | 0660), NULL, 0);
    utils.bear_attack = false;

    utils.bees = shmat(shmget(IPC_PRIVATE, sizeof(pid_t[3][MAX_BEES_OF_ONE_KIND]), IPC_CREAT | IPC_EXCL | 0660), 0, 0);

    return utils;

};


/* Check if game is finished */
void game_check(struct Utils * utils){
    if (utils->game_status->workers == 0 && utils->game_status->honey == 0){
        system("clear");
        printf("------GAME OVER ------ \n");
        system("pkill -f beehive");
    }

    if (utils->game_status->queens == 3){
        system("clear");
        printf("------YOU WIN------ \n");
        system("pkill -f beehive");
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

