#ifndef BEEHIVE_BEARS_H
#define BEEHIVE_BEARS_H

#define BEAR_SLEEP 10

struct Bear{
    int hunger; /* Val beetwen 0 and 100 */
    int calling_factor; /* Val beetwen 0 and 100 */
};


//Function called every BEAR_SLEEP seconds
void bear_action();

//Bear process
void bear();


#endif //BEEHIVE_BEARS_H
