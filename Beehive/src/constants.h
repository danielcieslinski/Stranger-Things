#ifndef BEEHIVE_CONSTANTS_H
#define BEEHIVE_CONSTANTS_H

/* Bees constants */
#define WORKER_PRODUCTION_TIME 3
#define WARRIOR_PRODUCTION_TIME 5
#define QUEEN_PRODUCTION_TIME 9
#define WORKER_HONEY_PRODUCTION_TIME 3
#define WORKER_PRODUCTION_AMOUNT 2


/* How long a bee waits until eating and how much eats */
#define EAT_AMOUNT 1
#define EAT_TIME 14

#define STORAGE_CAPACITY 100

/* Cost constants */
#define WORKER_COST 7
#define WARRIOR_COST 10
#define QUEEN_COST 500

/* Bear constants */
#define BEAR_SLEEP 30
#define BEAR_CALLING_FACTOR_INIT 5
#define BEAR_HUNGER_INIT 100
#define BEAR_CALLING_FACTOR_INC 5
#define BEAR_HUNGER_INC 5
#define BEAR_STORAGE_DISPLACEMENT 10 /* How many places bear takes in storage */
#define N_OF_BEES_KILLING_BEAR 10

/*Other constants */
#define BEAR_EATING 50 /*How much honey to eat if winning */
#define HONEY_INIT 10
#define MAX_BEES_OF_ONE_KIND 1024

#endif
