#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <zconf.h> //Fork

#include "Utilities.h"
#include "Bees.h"
#include <time.h>
#include <math.h>

typedef enum{
    EATING, PRODUCING
} AccessType;


void storage(struct Utils *utils, AccessType type) {
    if (type == PRODUCING)
        utils->game_status->honey += WORKER_PRODUCTION_AMOUNT;

    else utils->game_status->honey -= EAT_AMOUNT;
}

void worker(struct Utils *utils) {

    time_t eating_clock = clock();
    time_t prod_clock = clock();
    AccessType type;

    while (true) {

        sleep(1);

        clock_t now = clock();
        if ( EAT_TIME - (now - eating_clock) > WORKER_HONEY_PRODUCTION_TIME - (now - prod_clock) ) {
            printf("%f \n", (now - prod_clock));
            sleep(WORKER_HONEY_PRODUCTION_TIME - (now - prod_clock));  //CORRECT SLEEP
            type = PRODUCING;
        } else {
//            printf("%lf \n", EAT_TIME - (now - eating_clock));
            sleep(EAT_TIME - (now - eating_clock));  //CORRECT SLEEP
            type = EATING;
        }

        printf("smth \n");
        //Getting into magazine
        while (utils->bear_attack) {usleep(1);}

        sem_down_wait(utils->storage_sem, 0);
        storage(utils, type);
        sem_up(utils->storage_sem, 0);

        if(type == PRODUCING)
            prod_clock = clock();
        else eating_clock = clock();
    }
}

void warrior(struct Utils *utils) {

}

void queen(struct Utils *utils) {

}


void produce_bee(struct Utils *utils, BeeType type) {
    if (fork() == 0) {
        switch (type) {

            case WORKER:
                utils->game_status->honey -= WORKER_COST;
                sleep(WORKER_PRODUCTION_TIME);
                utils->game_status->workers++;
//                utils->bees[0][utils->game_status->workers - 1] = getpid();
                worker(utils);
                break;

            case WARRIOR:
                utils->game_status->honey -= WARRIOR_COST;
                sleep(WARRIOR_PRODUCTION_TIME);
                utils->game_status->warriors++;
//                utils->bees[1][utils->game_status->warriors - 1] = getpid();
                warrior(utils);
                break;

            case QUEEN:
                utils->game_status->honey -= QUEEN_COST;
                sleep(QUEEN_PRODUCTION_TIME);
                utils->game_status->queens++;
//                utils->bees[2][utils->game_status->queens - 1] = getpid();
                queen(utils);
                break;

        }
    }
}
