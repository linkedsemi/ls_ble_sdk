#ifndef LL_CONFIG_H_
#define LL_CONFIG_H_
#include <stdint.h>
#include "ll_port.h"
#include "ll_external.h"
#include "log.h"
#define MINIMUM_SLEEP_TIME_US (5000)
#define WAKEUP_IN_ADVANCE_US (1000)
#define WAKEUP_DELAY_LPCYCLES (30)
#define INFINITE_SLEEP_TIME (~0)
extern void (*stack_assert_asm_fn)(uint32_t,uint32_t,uint32_t);
#define LL_ASSERT_P(e,lvl,param0,param1) if(!(e)) stack_assert_asm_fn((lvl),(uint32_t)(param0),(uint32_t)(param1)) 
#define LL_ASSERT(e,p0,p1) LL_ASSERT_P(e,LVL_ERROR,p0,p1)
#define US2HCLK(x) (MAC_CLK_MHZ * (x))
#define LL_PKT_ISR
#define LL_EVT_ISR
#endif
