#ifndef PLATFORM_H_
#define PLATFORM_H_
#include <stdint.h>
#include "sdk_config.h"

void sys_init_itf(void);

void arm_cm_delay_asm(uint32_t);

#define DELAY_US(a) arm_cm_delay_asm((a)*(SDK_HCLK_MHZ/4))

#endif
