#ifndef PLATFORM_H_
#define PLATFORM_H_
#include <stdint.h>
#include <stdbool.h>
#include "sdk_config.h"
bool clk_check(void);

void clk_switch(void);

void sys_init_itf(void);

void sys_init_app(void);

void stack_assert_asm(uint32_t,uint32_t,uint32_t);

void platform_reset(uint32_t error);

void arm_cm_set_int_isr(uint8_t type,void (*isr)());

void arm_cm_delay_asm(uint32_t);

uint32_t get_app_image_base();

void sys_init_none();

void pll_enable();

void dcdc_init();

void dcdc_switch_to_pwm();

void dcdc_switch_to_pfm();

void dcdc_switch_to_bypass();

#define DELAY_US(a) arm_cm_delay_asm((a)*(SDK_HCLK_MHZ/4))

#endif
