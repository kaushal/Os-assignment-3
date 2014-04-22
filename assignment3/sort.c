#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int array[10] = {9, 8, 8097, 6, 5000, 4, 3, 2, 1, 1000};
#define ARRAY_SIZE 10
#define NUM_MUTEXES 9
static pthread_mutex_t mutexes[NUM_MUTEXES + 1];
pthread_t pids[NUM_MUTEXES];
int arraySorted = 0;

struct threadInfo {
    int leftPos;
};

void *checkSorted(void *arg) {
    int i = 0;
    int sorted = 0;
    while(sorted == 0) {
        for(i = 0; i < ARRAY_SIZE - 1; i++) {
            if(array[i] > array[i + 1]) {
                sorted = 0;
                break;
            }
            else
                sorted = 1;
        }
    }
    printf("I think the array is sorted\n");

    arraySorted = 1;
}

void *checkNeighbors(void *arg) {
    struct threadInfo *ti = (struct threadInfo *)arg;
    int leftPos = ti -> leftPos;
    int tempVal = 0;
    while(arraySorted == 0) {
        if(array[leftPos] > array[leftPos + 1]) {
            if(pthread_mutex_trylock(&mutexes[leftPos])) {
                if(pthread_mutex_trylock(&mutexes[leftPos + 1])) {
                    tempVal = array[leftPos];
                    array[leftPos] = array[leftPos + 1];
                    array[leftPos + 1] = tempVal;
                    pthread_mutex_unlock(&mutexes[leftPos]);
                    pthread_mutex_unlock(&mutexes[leftPos + 1]);
                }
                else {
                    pthread_mutex_unlock(&mutexes[leftPos]);
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
        pthread_create(&(pids[i]), NULL, checkNeighbors, (void *)ti);
    }
    pthread_t master;
    pthread_create(&master, NULL, checkSorted, NULL);
    while(arraySorted == 0) {}
    for(i = 0; i < ARRAY_SIZE; i++) {
        printf("%d \n", array[i]);
    }
    for(i = 0; i < ARRAY_SIZE; i++) {
        if (i < ARRAY_SIZE - 1)
            pthread_join(pids[i], NULL);
        else
            pthread_join(master, NULL);
    }
    return 0;
}
