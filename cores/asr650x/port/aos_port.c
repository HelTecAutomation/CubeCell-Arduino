#include <stdio.h>
#include <stdlib.h>
#include "aos/aos.h"

void *aos_malloc(unsigned int size)
{
    return malloc(size);
}

void aos_free(void *mem)
{
    free(mem);
}

int aos_task_new(const char *name, void (*fn)(void *), void *arg, int stack_size)
{
    (void)name;
    (void)stack_size;
    
    fn(arg);
    return 0;
}

void aos_task_exit(int code)
{
    (void)code;
}

int aos_mutex_new(aos_mutex_t *mutex) 
{
    (void)mutex;
    return 0;
}
void aos_mutex_free(aos_mutex_t *mutex)
{
    (void)mutex;
}
int aos_mutex_lock(aos_mutex_t *mutex, unsigned int timeout)
{
    (void)mutex;
    (void)timeout;
    return 0;
}
int aos_mutex_unlock(aos_mutex_t *mutex)
{
    (void)mutex;
    return 0;
}

int aos_sem_new(aos_sem_t *sem, int count)
{
    (void)sem;
    (void)count;
    return 0;
}
void aos_sem_free(aos_sem_t *sem)
{
    (void)sem;
}
int aos_sem_wait(aos_sem_t *sem, unsigned int timeout)
{
    (void)sem;
    (void)timeout;
    return 0;
}
void aos_sem_signal(aos_sem_t *sem)
{
    (void)sem;
}
int aos_sem_is_valid(aos_sem_t *sem)
{
    (void)sem;
    return 1;
}
void aos_sem_signal_all(aos_sem_t *sem)
{
    (void)sem;
}