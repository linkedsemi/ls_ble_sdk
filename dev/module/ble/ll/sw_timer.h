#ifndef SW_TIMER_H_
#define SW_TIMER_H_
#include <stdint.h>
#include <stdbool.h>
#include "circular_doubly_linked_list.h"

struct sw_timer_env
{
	struct cdll_hdr hdr;
	void (*callback)(void *);
	void *cb_param;
	uint32_t period;
	uint32_t target;
};

void sw_timer_module_init(void);

void sw_timer_callback_set(struct sw_timer_env *timer,void (*func)(void *),void *param);

void sw_timer_period_set(struct sw_timer_env *timer,uint32_t period);

uint32_t sw_timer_period_get(struct sw_timer_env *timer);

void sw_timer_start(struct sw_timer_env *timer);

void sw_timer_stop(struct sw_timer_env *timer);

bool sw_timer_active(struct sw_timer_env *timer);

void sw_timer_update(void);

#endif
