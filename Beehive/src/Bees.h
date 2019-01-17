#ifndef BEEHIVE_BEES_H
#define BEEHIVE_BEES_H

// Time constants
#define WORKER_PRODUCTION_TIME 3
#define WARRIOR_PRODUCTION_TIME 5
#define QUEEN_PRODUCTION_TIME 9


#define WORKER_HONEY_PRODUCTION_TIME 3
#define WORKER_PRODUCTION_AMOUNT 2


#define EAT_AMOUNT 1
#define EAT_TIME 14

#define STORAGE_CAPACITY 100

// Cost constants
#define WORKER_COST 7
#define WARRIOR_COST 10
#define QUEEN_COST 500

void worker(struct Utils * utils);

void warrior(struct Utils * utils);

void queen(struct Utils * utils);

void produce_bee(struct Utils * utils, BeeType type);


#endif //BEEHIVE_BEES_H
