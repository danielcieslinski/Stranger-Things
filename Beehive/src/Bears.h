#ifndef BEEHIVE_BEARS_H
#define BEEHIVE_BEARS_H



#include "Utilities.h"

struct BearStats{
    int hunger; /* Val beetwen 0 and 100 */
    int calling_factor; /* Val beetwen 0 and 100 */
};


void bear_attack(struct Utils * utils);

void bear(struct Utils * utils);

#endif
