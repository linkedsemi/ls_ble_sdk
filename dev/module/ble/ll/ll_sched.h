#ifndef LL_SCHED_H_
#define LL_SCHED_H_
#include "fifo.h"

struct ll_sched_env
{
    void (*func)(void *);
    void *param;
};

void ll_invoke(void (*func)(void *),void *param);

void ll_schedule(void);

void swint2_set(void);

void swint2_post(void (*func)(void *),void *param);

void swint2_process(void);


#endif

