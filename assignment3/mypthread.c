#include <ucontext.h>
#include <setjmp.h>
#include "mypthread.h"

int mypthread_create(mpthread_t *thread, void *(*start_routine)(void *), void *arg){
    char stack[STACK_SIZE];
    getcontext(&thread->context);
}

void mypthread_exit (void *retval) {

}

void mpthread_yield (void) {

}

int mpthread_join (mpthread_t thread, void **retval) {

}
