#ifndef PTHREAD_H
#define PTHREAD_H

#include <stddef.h>  
#include <stdlib.h>  
#include <stdint.h>
#include "common.h" 

typedef struct pcb_t* pthread_t;
struct pcb_t* get_current_process(void); 


typedef struct pthread_mutex_t {
    volatile int locked;  
    pthread_t owner;       
} pthread_mutex_t;

#define PTHREAD_MUTEX_INITIALIZER {0, NULL}

typedef struct {
    int detachstate;       
    int schedpolicy;       
    void *stackaddr;      
    size_t stacksize;      
} pthread_attr_t;

typedef struct {
    pthread_mutex_t mutex;
} pthread_cond_t;

int pthread_mutex_init(pthread_mutex_t *mutex, const void *attr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void*), void *arg);
int pthread_join(pthread_t thread, void **retval);
int pthread_detach(pthread_t thread);

int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);

int pthread_mutex_init(pthread_mutex_t *mutex, const void *attr) {
    mutex->locked = 0;    
    mutex->owner = NULL;  
    return 0;             
}

int pthread_mutex_lock(pthread_mutex_t *mutex) {
    while (__sync_lock_test_and_set(&mutex->locked, 1)) {
    }
    mutex->owner = get_current_process(); 
    return 0; 
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
    if (mutex->owner != get_current_process()) {
        return 1;
    }
    mutex->owner = NULL; 
    __sync_lock_release(&mutex->locked); 
    return 0; 
}

#endif // PTHREAD_H
