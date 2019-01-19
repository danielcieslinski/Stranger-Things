#include <stdbool.h> //Booleans
#include <stdlib.h>
#include <zconf.h> //Sleep
#include <time.h>
#include <sys/types.h>
#include <signal.h>

#include <stdio.h> //Printf

#include "Bears.h"
#include "Utilities.h"


void bear_attack(struct Utils *utils) {
    int i; //ANSI C come on, this is awful

    printf("Bear attack \n");

    //Take places in storage
    utils->bear_attack = true; //Priority
    for (i = 0; i < BEAR_STORAGE_DISPLACEMENT; i++)
        sem_down_wait(utils->storage_sem);
    utils->bear_attack = false;

    bool looses = (utils->game_status->warriors >= N_OF_BEES_KILLING_BEAR);

    // Holds info on how more bees to kill if not enough warriors
    int left_to_kill = (looses) ? 0 : N_OF_BEES_KILLING_BEAR - utils->game_status->warriors;


    /* Killing bees is ugly, but stable and doesn't cause problems if there are no enough bees at all */


    //Kill all warriors
    if (left_to_kill > 0) {
        for (i = utils->game_status->warriors - 1; i >= 0; i--) {
            kill(utils->bees[WARRIOR][i], SIGKILL); //If problem, change to SIGABRT
            perror("killing all warriors");
            utils->game_status->warriors--;
        }
    } else {
        int ptr = utils->game_status->warriors - 1;
        for (i = 0; i < N_OF_BEES_KILLING_BEAR; i++) {
            kill(utils->bees[WARRIOR][ptr], SIGKILL); //If problem, change to SIGABRT
            perror("killing some warriors");
            utils->game_status->warriors--;
            ptr--;
        }
    }

    //Kill workers if not looses
    if (!looses) {

        if (left_to_kill > utils->game_status->workers) {
            for (i = utils->game_status->workers - 1; i >= 0; i--) {
                kill(utils->bees[WORKER][i], SIGKILL); //If problem, change to SIGABRT
                perror("killing all workers");
                utils->game_status->workers--;
            }
        } else {
            int ptr = utils->game_status->workers - 1;
            for (i = 0; i < left_to_kill; i++) {
                kill(utils->bees[WORKER][ptr], SIGKILL); //If problem, change to SIGABRT
                perror("killing some workers");
                utils->game_status->workers--;
                ptr--;
            }
        }


        utils->game_status->honey -= BEAR_EATING;
    }

    printf("Bear looses %d \n", looses);
}


void bear(struct Utils *utils) {
    struct BearStats bear_stats;
    bear_stats.hunger = BEAR_HUNGER_INIT;
    bear_stats.calling_factor = BEAR_CALLING_FACTOR_INIT;

    srand(time(NULL)); //Fastest possible call after 10 seconds, so no problem with seed

    while (true) {
        sleep(BEAR_SLEEP);

        int calling_rand = rand() % 100;
        if (calling_rand < bear_stats.calling_factor) {
            //Call new bear
            if (fork() == 0)
                bear(utils);

            bear_stats.calling_factor = BEAR_CALLING_FACTOR_INIT;
        } else bear_stats.calling_factor += BEAR_CALLING_FACTOR_INC;

        int hunger_rand = rand() % 100;
        if (hunger_rand < bear_stats.hunger) {
            bear_attack(utils);
            bear_stats.hunger = BEAR_HUNGER_INIT; //It's always back to init, whether he looses or not
        } else bear_stats.hunger += BEAR_HUNGER_INC;


    }

}
