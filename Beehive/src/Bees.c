#include <stdio.h>
#include <stdbool.h>
#include <zconf.h> //Fork

#include "Utilities.h"
#include "Bees.h"

void worker(struct Utils * utils){


    while (true){
        sleep(WORKER_HONEY_PRODUCTION_TIME);

        //Getting into magazine
        sem_down_wait(utils->storage_sem, 0);
        utils->game_status->honey += WORKER_PRODUCTION_AMOUNT;
        

    }


}

void warrior(struct Utils * utils){

}

void queen(struct Utils * utils){

}



void produce_bee(struct Utils * utils, BeeType type){
    if (fork() == 0) {
        switch (type) {

            case WORKER:
                utils->game_status->honey -= WORKER_COST;
                sleep(WORKER_PRODUCTION_TIME);
                utils->game_status->workers++;
//                refresh_monitor(utils);
                worker(utils);
                break;

            case WARRIOR:
                utils->game_status->honey -= WARRIOR_COST;
                sleep(WARRIOR_PRODUCTION_TIME);
                utils->game_status->warriors++;
//                refresh_monitor(utils);
                warrior(utils);
                break;

            case QUEEN:
                utils->game_status->honey -= QUEEN_COST;
                sleep(QUEEN_PRODUCTION_TIME);
                utils->game_status->queens++;
//                refresh_monitor(utils);
                queen(utils);
                break;

        }
    }
}
