#include <stdio.h> //Printf
#include <stdlib.h> //System
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <zconf.h> //Fork
#include <sys/wait.h>
#include <stdbool.h> //Bools
#include <time.h>
#include <math.h>

#include "Utilities.h"
#include "Bees.h"



void input_monitor(struct Utils * utils){

    int costs[3] = {WORKER_COST, WARRIOR_COST, QUEEN_COST};

    struct Msgbuf msg;
    BeeType input;

    printf("\n \n");
    printf("-------ACTIONS------- \n");
    printf("0) Worker, cost: %d H \n", WORKER_COST);
    printf("1) Warrior, cost: %d H \n", WARRIOR_COST);
    printf("2) Queen, cost: %d H \n", QUEEN_COST);
    printf("3) Refresh \n");
    printf("\n \n Enter action number \n");

    scanf("%d", &input);

    if(input > 2)
        return;

    if (utils->game_status->honey - costs[input] < 0)
        return;

    produce_bee(utils, input);

}

void output_monitor(struct Utils * utils){

    struct Msgbuf msg;

    while(true){
        system("clear");

        printf("-------STATISTICS ------- \n");
        printf("Honey: %d \n", utils->game_status->honey);
        printf("Workers: %d \n", utils->game_status->workers);
        printf("Warriors: %d \n", utils->game_status->warriors);
        printf("Queens: %d \n", utils->game_status->queens);

        int pid = fork();
        if(pid == 0){
            input_monitor(utils);
            exit(0);
        }
        waitpid(pid, NULL,0);

    }
}

void setup(struct Utils * utils){

//    if(fork() == 0)
//        input_monitor(utils);


    if (fork() == 0)
        output_monitor(utils);

}



int main() {

    struct Utils utils = utils_initializer();
    utils.game_status->honey = HONEY_INIT;

    setup(&utils);

    wait(0);
    return 0;
}


//https://docs.google.com/document/d/18qK-LDqTdCfkSwHXYAKuolfmTBdQeMraAxqfz3qNTUk/edit?fbclid=IwAR2LFSU5ehwvw0qpMPEhCJi8Fi881O5LvGKBlSLXa6Fycsx8sOqGHoRt8R4