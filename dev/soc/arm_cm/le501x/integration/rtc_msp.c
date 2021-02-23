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
    // arm_cm_set_int_isr(RTC_IRQn, RTC_Handler);
    // NVIC_ClearPendingIRQ(RTC_IRQn);
    // NVIC_EnableIRQ(RTC_IRQn);
    NVIC_DisableIRQ(RTC_IRQn);
    REG_FIELD_WR(RCC->AHBEN, RCC_RTC, 1);
}

void HAL_MSP_RTC_DeInit(void)
{
    REG_FIELD_WR(RCC->AHBEN, RCC_RTC, 0);
    NVIC_DisableIRQ(RTC_IRQn);
}
