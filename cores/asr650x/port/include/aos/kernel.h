/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef AOS_KERNEL_H
#define AOS_KERNEL_H

#include <stddef.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AOS_WAIT_FOREVER    0xffffffffu
#define AOS_NO_WAIT         0x0
#define AOS_DEFAULT_APP_PRI 32

typedef struct {
    void *hdl;
} aos_hdl_t;

typedef aos_hdl_t aos_task_t;
typedef aos_hdl_t aos_mutex_t;
typedef aos_hdl_t aos_sem_t;
typedef aos_hdl_t aos_queue_t;
typedef aos_hdl_t aos_timer_t;
typedef aos_hdl_t aos_work_t;
typedef aos_hdl_t aos_event_t;
    
//mm   
void *aos_malloc(unsigned int size);   
void aos_free(void *mem);

//task
int aos_task_new(const char *name, void (*fn)(void *), void *arg, int stack_size);
void aos_task_exit(int code);

//mutex
int aos_mutex_new(aos_mutex_t *mutex);
void aos_mutex_free(aos_mutex_t *mutex);
int aos_mutex_lock(aos_mutex_t *mutex, unsigned int timeout);
int aos_mutex_unlock(aos_mutex_t *mutex);

//sem
int aos_sem_new(aos_sem_t *sem, int count);
void aos_sem_free(aos_sem_t *sem);
int aos_sem_wait(aos_sem_t *sem, unsigned int timeout);
void aos_sem_signal(aos_sem_t *sem);
int aos_sem_is_valid(aos_sem_t *sem);
void aos_sem_signal_all(aos_sem_t *sem);

#ifdef __cplusplus
}
#endif

#endif /* AOS_KERNEL_H */

