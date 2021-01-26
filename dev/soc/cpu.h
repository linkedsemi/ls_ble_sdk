#ifndef CPU_H_
#define CPU_H_
#include <stdbool.h>
#include <stdint.h>

uint32_t enter_critical(void);

void exit_critical(uint32_t);

bool in_interrupt(void);

void disable_global_irq(void);

void enable_global_irq(void);

#endif

