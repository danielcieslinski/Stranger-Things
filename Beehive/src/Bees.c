#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <zconf.h> //Fork

#include "Utilities.h"
#include "Bees.h"
#include <time.h>
#include <math.h>

typedef enum {
    EATING, PRODUCING
} AccessType;


void storage(struct Utils *utils, AccessType type) {

    //Getting into magazine
    while (utils->bear_attack) { usleep(1);} //Change for nano

    sem_down_wait(utils->storage_sem);

    if (type == PRODUCING)
        utils->game_status->honey += WORKER_PRODUCTION_AMOUNT;

    else utils->game_status->honey -= EAT_AMOUNT;

    sem_up(utils->storage_sem);
}

void worker(struct Utils *utils){

    printf("in \n");

    AccessType type;
    time_t eating_clock, prod_clock, now;

    eating_clock = time(&eating_clock);
    prod_clock = time(&prod_clock);

    while (true) {
        now = time(&now);

        if (EAT_TIME - difftime(now, eating_clock) > WORKER_HONEY_PRODUCTION_TIME - difftime(now, prod_clock)) {
            sleep(WORKER_HONEY_PRODUCTION_TIME - (now - prod_clock));
            type = PRODUCING;
        } else {
            sleep(EAT_TIME - (now - eating_clock));
            type = EATING;
        }

        storage(utils, type);

        if (type == PRODUCING)
            prod_clock = time(&prod_clock);
        else eating_clock = time(&eating_clock);
    }
}

void warrior(struct Utils *utils) {

    //Eat sleep, get killed
    while (true){
        sleep(14);
        storage(utils, EATING);
    }

}

void queen(){
    while (true)
        sleep(1);

    /*It does nothing */
}


void produce_bee(struct Utils *utils, BeeType type) {
    if (fork() == 0) {
        switch (type) {

            case WORKER:
                utils->game_status->honey -= WORKER_COST;
                sleep(WORKER_PRODUCTION_TIME);
                utils->game_status->workers++;
                utils->bees[WORKER][utils->game_status->workers - 1] = getpid();
                worker(utils);
                break;

            case WARRIOR:
                utils->game_status->honey -= WARRIOR_COST;
                sleep(WARRIOR_PRODUCTION_TIME);
                utils->game_status->warriors++;
                utils->bees[WARRIOR][utils->game_status->warriors - 1] = getpid();
                warrior(utils);
                break;

            case QUEEN:
                utils->game_status->honey -= QUEEN_COST;
                sleep(QUEEN_PRODUCTION_TIME);
                utils->game_status->queens++;
                utils->bees[QUEEN][utils->game_status->queens - 1] = getpid();
                queen();
                break;

        }
    }
}
