#ifndef BEEHIVE_UTILITIES_H
#define BEEHIVE_UTILITIES_H

struct GameStatus{

    int honey, workers, warriors, queens;

};

struct Utils{
//    struct GameStatus * game_status;
    struct GameStatus * game_status;
};

struct Utils utils_initializer();

#endif //BEEHIVE_UTILITIES_H
