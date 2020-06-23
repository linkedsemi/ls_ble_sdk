#ifndef REG_TIMER_H_
#define REG_TIMER_H_
#include "reg_timer_type.h"

#define LSADTIM1 ((reg_timer_t *)(0x40012C00)) // Adv TIMER, 16bit, 4 channel
#define LSGPTIM1 ((reg_timer_t *)(0x40014000)) // General purpose TIMERC, 16bit, 2 channel, dead-zone
#define LSGPTIM2 ((reg_timer_t *)(0x40000000)) // General purpose TIMERA, 32bit, 4 channel, no dead-zone
#define LSGPTIM3 ((reg_timer_t *)(0x40000400)) // General purpose TIMERB, 16bit, 4 channel, no dead-zone
#define LSBSTIM1 ((reg_timer_t *)(0x40001000)) // Basic TIMER, 16bit

#endif //(REG_TIMER_H_)
