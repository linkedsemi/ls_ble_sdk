#ifndef TMALL_LIGHT_CFG_H_
#define TMALL_LIGHT_CFG_H_

#include "reg_timer.h"
#include "lstimer.h"
#include "le501x.h"
#include "platform.h"
#include "reg_rcc.h"
#include "lsgpio.h"
#include "field_manipulate.h"

#include <string.h>

#define TIMx   LSGPTIM3


#define GENERIC_LEVEL_MIN (-32767-1)

struct light_state
{
    uint8_t onoff_state;
    uint16_t level;
};

void sigmesh_pwm_init(void);
void sigmesh_set_lightness(uint16_t level);
void sigmesh_set_onoff(uint8_t on_off);
uint16_t sigmesh_get_lightness(void);
uint8_t sigmesh_get_onoff(void);
void light_button_init(void);

#endif
