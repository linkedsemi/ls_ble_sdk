#ifndef TIMER_WRAPPER_H_
#define TIMER_WRAPPER_H_
#include "sw_timer.h"
#include "ll_config.h"
typedef struct sw_timer_env timer_env_t;
typedef struct sw_timer_env *timer_t;

#define MS_2_PERIOD(x) ((x)*MAC_CLK_MHZ*1000000/1000)

#define PERIOD_2_MS(x) ((x)/MAC_CLK_MHZ/1000)

void timer_config(timer_t timer,void (*callback)(void *),void *param,uint32_t period);

void timer_callback_set(timer_t timer,void (*callback)(void *),void *param);

void timer_period_set(timer_t timer,uint32_t period);

uint32_t timer_period_get(timer_t timer);

void timer_start(timer_t timer);

bool timer_stop(timer_t timer);

#endif

