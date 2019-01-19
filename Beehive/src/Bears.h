#ifndef BEEHIVE_BEARS_H
#define BEEHIVE_BEARS_H

#define BEAR_SLEEP 30

#define BEAR_CALLING_FACTOR_INIT 5
#define BEAR_HUNGER_INIT 100

#define BEAR_CALLING_FACTOR_INC 5
#define BEAR_HUNGER_INC 5

#define BEAR_STORAGE_DISPLACEMENT 10 //How many places bear takes in storage

#define N_OF_BEES_KILLING_BEAR 10

#define BEAR_EATING 50 //How much honey to eat if winning

#include "Utilities.h"

struct BearStats{
    int hunger; /* Val beetwen 0 and 100 */
    int calling_factor; /* Val beetwen 0 and 100 */
};


void bear_attack(struct Utils * utils);

//Bear process
void bear(struct Utils * utils);


#endif //BEEHIVE_BEARS_H
