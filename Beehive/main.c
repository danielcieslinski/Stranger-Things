#include <stdio.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include "Utilities.h"

int main() {

    struct Utils utils = utils_initializer();
    utils.game_status->honey = 10;

    printf("%d ", utils.game_status->honey);

    return 0;
}