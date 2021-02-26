#include "rtc_msp.h"
#include "reg_base_addr.h"
#include "platform.h"
#include "field_manipulate.h"
#include "le501x.h"
#include "reg_rcc.h"

void HAL_MSP_RTC_Init(void)
{
    REG_FIELD_WR(RCC->AHBRST, RCC_RTC, 1);
    REG_FIELD_WR(RCC->AHBRST, RCC_RTC, 0);
    REG_FIELD_WR(RCC->AHBEN, RCC_RTC, 1);
}

void HAL_MSP_RTC_DeInit(void)
{
    REG_FIELD_WR(RCC->AHBEN, RCC_RTC, 0);
}

