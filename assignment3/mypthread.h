#ifndef MPTHREAD_H
#define MPTHREAD_H
#include <ucontext.h>
#include <signal.h>

#define STACK_SIZE 8192

typedef struct {
    ucontext_t context;
} mpthread_t;

int mpthread_create (mpthread_t *thread, void*(*start_routine)(void *), void *arg);
void mpthread_exit (void *retval);
void mpthread_yield (void);
int mpthread_join  (mpthread_t thread, void **retval);

#endif
