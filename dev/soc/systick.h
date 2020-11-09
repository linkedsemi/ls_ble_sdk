#ifndef SYSTICK_H_
#define SYSTICK_H_
#include <stdint.h>
#include <stdbool.h>

void systick_start(void);

uint32_t systick_get_value(void);

/* if a is later than b return >0 else return <=0 */ 
int32_t systick_time_diff(uint32_t a,uint32_t b);

bool systick_poll_timeout(uint32_t start,uint32_t timeout,bool (*poll)(void *),void *param);

#endif
