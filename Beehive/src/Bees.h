#ifndef BEEHIVE_BEES_H
#define BEEHIVE_BEES_H

#include "Utilities.h"

void worker(struct Utils * utils);

void warrior(struct Utils * utils);

void queen();

void produce_bee(struct Utils * utils, BeeType type);

#endif
