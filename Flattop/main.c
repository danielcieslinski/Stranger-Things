#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/shm.h>
#include "Utilities.h"

struct Args {
    struct Utils *utils;
    int plane_id;
};

struct Utils {
    pthread_mutex_t mut;
    pthread_cond_t cond;

    int *on_flattop;
    int *on_air;
};

struct Utils utils_initializer() {
    struct Utils utils;

//    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
//    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//    utils.mut = mut;
//    utils.cond = cond;

    utils.on_air = shmat(shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0660), 0, 0);
    utils.on_flattop = shmat(shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0660), 0, 0);

    return utils;
}

void critical_section(int plane_id, int on_air, struct Utils *utils) {

    if (on_air) {

        printf("Plane %d is landing \n", plane_id);
        sleep(LANDING_TIME);
        utils->on_air--;
        utils->on_flattop++;
        printf("Plane %d landed \n", plane_id);

    } else {

        printf("Plane %d is taking off \n", plane_id);
        sleep(TAKING_OFF_TIME);
        utils->on_air++;
        utils->on_flattop--;
        printf("Plane %d took off \n", plane_id);

    }


}

void *plane(void *args) {
    //Some cleanup
    struct Args *args1 = (struct Args *) args;
    struct Utils utils = *args1->utils;
    int plane_id = args1->plane_id;

    //Setup
    srand(time(NULL) + plane_id);
//    int on_air = rand() % 2;
    int on_air = true;

    if (on_air) *utils.on_air += 1;
    else *utils.on_flattop += 1;

    printf("New plane %d, on air %d \n", plane_id, on_air);

    //Plane loop
    while (true) {
//        pthread_cond_broadcast(&utils.cond);
        if (on_air){
            pthread_mutex_lock(&utils.mut);
            while (! (* utils.on_flattop < FLATTOP_CAPACITY)){
                pthread_cond_wait(&utils.cond, &utils.mut);
            }
            critical_section(plane_id, on_air, &utils);
            pthread_mutex_unlock(&utils.mut);

        } else {

            pthread_mutex_lock(&utils.mut);
            while (* utils.on_flattop < FLATTOP_CAPACITY) {
                pthread_cond_wait(&utils.cond, &utils.mut);
            }
            critical_section(plane_id, on_air, &utils);
            pthread_mutex_unlock(&utils.mut);
        }
        on_air = !on_air;
    }
    return NULL;
}

int main() {
    pthread_t threads[PLANES];
    struct Utils utils = utils_initializer();

    for (int i = 0; i < PLANES; ++i) {
        sleep(1);

        struct Args args;
        args.utils = &utils;
        args.plane_id = i;

        pthread_create(&threads[i], NULL, plane, &args);
    }

    for (int i = 0; i < PLANES; ++i)
        pthread_join(threads[i], NULL);


    return 0;
}