#ifndef BEEHIVE_UTILITIES_H
#define BEEHIVE_UTILITIES_H


#include "constants.h"

struct GameStatus {
    int honey, workers, warriors, queens;
};

struct Utils {
    struct GameStatus *game_status;
    int storage_sem;
    bool bear_attack;
    pid_t (* bees)[MAX_BEES_OF_ONE_KIND];
};

typedef enum {
    WORKER, WARRIOR, QUEEN
} BeeType;


struct Utils utils_initializer();

void output_monitor(struct Utils * utils);

void sem_down_wait(int semid);

void game_check(struct Utils * utils);

void sem_up(int semid);

#endif
