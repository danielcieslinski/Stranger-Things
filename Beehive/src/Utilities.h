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
};

struct Msgbuf {
    long mtype; //typ komunikatu
    int mvalue; //treść komunikatu
};


struct Utils utils_initializer();

#endif //BEEHIVE_UTILITIES_H
