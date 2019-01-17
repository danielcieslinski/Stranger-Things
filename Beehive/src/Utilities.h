#ifndef BEEHIVE_UTILITIES_H
#define BEEHIVE_UTILITIES_H

#define HONEY_INIT 10

struct GameStatus {

    int honey, workers, warriors, queens;

};

struct Utils {
    struct GameStatus *game_status;
    int out_monitor_notifications;
    int input_monitor_notifications;
    int storage_sem;
    bool bear_attack;
    int ** bees;
};

struct Msgbuf {
    long mtype; //typ komunikatu
    int mvalue; //treść komunikatu
};

typedef enum {
    WORKER, WARRIOR, QUEEN
} BeeType;



struct Utils utils_initializer();

void refresh_monitor(struct Utils * utils);

void output_monitor(struct Utils * utils);

//bool sem_down_nowait(int semid, int semnum);

void sem_down_wait(int semid, int semnum);

void sem_up(int semid, int semnum);

#endif //BEEHIVE_UTILITIES_H
