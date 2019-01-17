#include <stdio.h> //Printf
#include <stdlib.h> //System
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <zconf.h> //Fork
#include <sys/wait.h>
#include <stdbool.h> //Bools

#include "Utilities.h"
#include "Bees.h"



void input_monitor(struct Utils * utils){

    struct Msgbuf msg;
    BeeType input;

    printf("\n \n");
    printf("-------ACTIONS------- \n");
    printf("0) Worker, cost: 7 H \n");
    printf("1) Warrior, cost: 10 H \n");
    printf("2) Queen, cost: 500 H \n");
    printf("3) Refresh \n");
    printf("\n \n Enter action number \n");

    scanf("%d", &input);

    if(input == 3)
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

    struct Msgbuf msg; msg.mvalue = 0; msg.mtype = 1;
    msgsnd(utils.out_monitor_notifications, &msg, sizeof(msg.mvalue), 0);

    setup(&utils);

    wait(0);

    return 0;
}