#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/shm.h>

#define PLANES 10
#define LANDING_TIME 0
#define TAKING_OFF_TIME 0
#define FLATTOP_CAPACITY 5
#define MAX_TIME_WITHOUT_ACTION 5

pthread_mutex_t mut, check_mut;
pthread_cond_t free_space_cond, priority_cond;

struct Args {
    struct Utils *utils;
    int plane_id;
};

typedef enum {
    LANDING, TAKING_OFF
} Priority;

struct Utils {
    int * on_flattop;
    int * on_air;
    Priority * priority;
};


struct Utils utils_initializer() {
    struct Utils utils;

    pthread_mutex_init(&mut,NULL);
    pthread_mutex_init(&check_mut,NULL);
    pthread_cond_init(&free_space_cond, NULL);
    pthread_cond_init(&priority_cond, NULL);

    utils.on_air = shmat(shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0660), 0, 0);
    utils.on_flattop = shmat(shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0660), 0, 0);
    utils.priority = shmat(shmget(IPC_PRIVATE, sizeof(Priority), IPC_CREAT | IPC_EXCL | 0660), 0, 0);

    * utils.priority = LANDING;


    return utils;
}

void critical_section(int plane_id, int on_air, struct Utils *utils) {

    if (on_air) {

        printf("Plane %d is landing \n", plane_id);
        sleep(LANDING_TIME);
        (* utils->on_air)--;
        (* utils->on_flattop)++;
        printf("Plane %d landed \n", plane_id);

    } else {

        printf("Plane %d is taking off \n", plane_id);
        sleep(TAKING_OFF_TIME);
        (* utils->on_air)++;
        (* utils->on_flattop)--;
        printf("Plane %d took off \n", plane_id);
    }

    printf("On air: %d, On Flattop %d \n", * utils->on_air, * utils->on_flattop);
}

void *plane(void *args) {
    //Some cleanup
    struct Args *args1 = (struct Args *) args;
    struct Utils utils = *args1->utils;
    int plane_id = args1->plane_id;

    //Setup
    srand(time(NULL) + plane_id);
    int on_air = true;
    (* utils.on_air)++;

    printf("New plane %d, on air %d \n", plane_id, on_air);

    //Plane loop
    while (true) {
        if (on_air){ //Landing

            pthread_mutex_lock(&check_mut); * utils.wants_to_land = true; pthread_mutex_unlock(&check_mut);

            pthread_mutex_lock(&mut);
            if (* utils.on_flattop == FLATTOP_CAPACITY){
                pthread_mutex_lock(&check_mut); * utils.wants_to_land = false; pthread_mutex_unlock(&check_mut);
                pthread_cond_wait(&free_space_cond, &mut);
            }
            critical_section(plane_id, on_air, &utils);
            pthread_mutex_unlock(&mut);

        } else {
            pthread_mutex_lock(&mut);
            pthread_mutex_lock(&check_mut);
            if (* utils.wants_to_land == true) {
                pthread_mutex_unlock(&check_mut);
                pthread
            }
//            while (* utils.on_flattop < FLATTOP_CAPACITY) {
//                pthread_cond_wait(&cond, &mut);
//            }
            critical_section(plane_id, on_air, &utils);
            pthread_mutex_unlock(&mut);
            pthread_cond_broadcast(&free_space_cond);
        }
        on_air = !on_air;

//        if (* utils.on_air == * utils.on_flattop)
//            pthread_cond_broadcast(&cond);

        //Do nothing for some time
//        sleep(rand() % MAX_TIME_WITHOUT_ACTION + 1);
    }
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