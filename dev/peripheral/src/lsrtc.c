#include <stdio.h>
#include "platform.h"
#include "lsrtc.h"
#include "rtc_msp.h" 
#include "HAL_def.h"
#include "field_manipulate.h"
#include "cpu.h"
#include "ls_dbg.h"

enum
{
    WKSEL_CLOSE = 0,      // close RTC wakeup function
    WKSEL_START_NORMAL,   // set RTC wakeup function as normal timer
    WKSEL_START_WAKEUP,   // start RTC wakeup function when enter sleep
};

void HAL_RTC_Init(uint8_t cksel)
{
    LS_ASSERT(cksel == RTC_CKSEL_LSE || cksel == RTC_CKSEL_LSI);
    HAL_MSP_RTC_Init();
    REG_FIELD_WR(RTC->CTRL,RTC_CTRL_CKSEL,cksel);
    REG_FIELD_WR(RTC->CTRL,RTC_CTRL_RTCEN,1);
}

void HAL_RTC_DeInit(void)
{
    HAL_MSP_RTC_DeInit();
}

void RTC_CalendarSet(calendar_cal_t *calendar_cal,calendar_time_t *calendar_time)
{
    uint32_t cpu_stat = enter_critical();
    REG_FIELD_WR(RTC->CTRL,RTC_CTRL_RTCEN,0);
    
    REG_FIELD_WR(RTC->CAL,RTC_CAL_YEAR_T,(calendar_cal->year/10)%100);
    REG_FIELD_WR(RTC->CAL,RTC_CAL_YEAR_U,(calendar_cal->year%100)%10);
    REG_FIELD_WR(RTC->CAL,RTC_CAL_MON_T,calendar_cal->mon/10);
    REG_FIELD_WR(RTC->CAL,RTC_CAL_MON_U,calendar_cal->mon%10);
    REG_FIELD_WR(RTC->CAL,RTC_CAL_DATE_T,calendar_cal->date/10);
    REG_FIELD_WR(RTC->CAL,RTC_CAL_DATE_U,calendar_cal->date%10);
    REG_FIELD_WR(RTC->TIME,RTC_TIME_WEEK,calendar_time->week);
    REG_FIELD_WR(RTC->TIME,RTC_TIME_HOUR_T,calendar_time->hour/10);
    REG_FIELD_WR(RTC->TIME,RTC_TIME_HOUR_U,calendar_time->hour%10);
    REG_FIELD_WR(RTC->TIME,RTC_TIME_MIN_T,calendar_time->min/10);
    REG_FIELD_WR(RTC->TIME,RTC_TIME_MIN_U,calendar_time->min%10);
    REG_FIELD_WR(RTC->TIME,RTC_TIME_SEC_T,calendar_time->sec/10);
    REG_FIELD_WR(RTC->TIME,RTC_TIME_SEC_U,calendar_time->sec%10);

    REG_FIELD_WR(RTC->CTRL,RTC_CTRL_RTCEN,1);
    exit_critical(cpu_stat);
}

HAL_StatusTypeDef RTC_CalendarGet(calendar_cal_t *calendar_cal, calendar_time_t *calendar_time)
{
    HAL_StatusTypeDef result = HAL_OK;
    REG_FIELD_WR(RTC->BKEN, RTC_BKEN_BKEN, 1); // set bken to update data immediately
    DELAY_US(1);
    calendar_cal->year  = REG_FIELD_RD(RTC->CAL,RTC_CAL_YEAR_T)*10 + REG_FIELD_RD(RTC->CAL,RTC_CAL_YEAR_U);
    calendar_cal->mon   = REG_FIELD_RD(RTC->CAL,RTC_CAL_MON_T)*10 + REG_FIELD_RD(RTC->CAL,RTC_CAL_MON_U); 
    calendar_cal->date  = REG_FIELD_RD(RTC->CAL,RTC_CAL_DATE_T)*10 + REG_FIELD_RD(RTC->CAL,RTC_CAL_DATE_U);     
    calendar_time->week = REG_FIELD_RD(RTC->TIME,RTC_TIME_WEEK);
    calendar_time->hour = REG_FIELD_RD(RTC->TIME,RTC_TIME_HOUR_T)*10 + REG_FIELD_RD(RTC->TIME,RTC_TIME_HOUR_U); 
    calendar_time->min  = REG_FIELD_RD(RTC->TIME,RTC_TIME_MIN_T)*10 + REG_FIELD_RD(RTC->TIME,RTC_TIME_MIN_U);     
    calendar_time->sec  = REG_FIELD_RD(RTC->TIME,RTC_TIME_SEC_T)*10 + REG_FIELD_RD(RTC->TIME,RTC_TIME_SEC_U);
    
    return result;
}

void RTC_wkuptime_set(uint32_t second)
{
    LS_ASSERT(second > 0);
    REG_FIELD_WR(RTC->WKUP, RTC_WKUP_WKCAL, second);
    REG_FIELD_WR(RTC->WKUP, RTC_WKUP_WKSEL, WKSEL_START_WAKEUP);
    REG_FIELD_WR(RTC->CTRL, RTC_CTRL_RTCEN, 1);
}
__attribute__((weak)) void rtc_wkup_callback(void){}
