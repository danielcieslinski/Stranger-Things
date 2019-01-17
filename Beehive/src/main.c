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




void input_monitor(struct Utils * utils){

    struct Msgbuf msg;
    int input;

    while (true){
        msgrcv(utils->input_monitor_notifications, &msg, sizeof(msg.mvalue), 0, 0);
        printf("\n \n");
        printf("-------ACTIONS------- \n");
        printf("1) Worker, cost: 7 H \n");
        printf("2) Worrior, cost: 10 H \n");
        printf("3) Queen, cost: 500 H \n");
        printf("\n \n Enter action number \n");

        scanf("%d", &input);
        msgsnd(utils->out_monitor_notifications, &msg, sizeof(msg.mvalue), 0);
    }

}

void output_monitor(struct Utils * utils){

    struct Msgbuf msg;

    while(true){
        system("clear");

        printf("-------STATISCTICS ------- \n");
        printf("Honey: %d \n", utils->game_status->honey);
        printf("Warriors: %d \n", utils->game_status->warriors);
        printf("Workers: %d \n", utils->game_status->workers);
        printf("Queens: %d \n", utils->game_status->queens);

        msgsnd(utils->input_monitor_notifications, &msg, sizeof(msg.mvalue), 0);
        msgrcv(utils->out_monitor_notifications, &msg, sizeof(msg.mvalue), 0, 0);
    }


}

void setup(struct Utils * utils){

    if(fork() == 0)
        input_monitor(utils);

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