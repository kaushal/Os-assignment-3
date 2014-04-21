#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10
#define NUM_MUTEXES 9
static pthread_mutex_t mutexes[NUM_MUTEXES];
pthread_t pids[NUM_MUTEXES];
int array[10] = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };

struct threadInfo {
    int leftPos;
};

void *checkNeighbors(void *arg) {
    struct threadInfo *ti = (struct threadInfo *)arg;
    int leftPos = ti -> leftPos;
    int tempVal = 0;
    printf("Left val of current thread: %d\n", ((struct threadInfo *)arg) -> leftPos);
    while(1) {
        if(array[leftPos] > array[leftPos + 1]) {
            if(pthread_mutex_trylock(&mutexes[leftPos])) {
                if(pthread_mutex_trylock(&mutexes[leftPos + 1])) {
                    tempVal = array[leftPos];
                    array[leftPos] = array[leftPos + 1];
                    array[leftPos + 1] = tempVal;
                    pthread_mutex_unlock(&mutexes[leftPos]);
                    pthread_mutex_unlock(&mutexes[leftPos + 1]);
                }
            }
        }
    }
    return NULL;
}

int main() {
    int i = 0;
    for(i = 0; i < NUM_MUTEXES; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }
    for(i = 0; i < NUM_MUTEXES; i++) {
        struct threadInfo *ti = malloc(sizeof(struct threadInfo));
        ti -> leftPos = i;
        printf("Starting thread with left Val: %d\n", ti -> leftPos);
        pthread_create(&(pids[i]), NULL, checkNeighbors, (void *)&ti);
        free(ti);
    }

    return 0;
}
