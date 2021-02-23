#include "lsrtc.h"
#include "le501x.h"
#include "platform.h"
#include "io_config.h"
#include <string.h>
#include "log.h"

static calendar_time_t calendar_time;
static calendar_cal_t calendar_cal;

static void rtc_test()
{
    calendar_cal.year = 24;
    calendar_cal.mon = 2;
    calendar_cal.date = 28;
    calendar_time.hour = 23;
    calendar_time.min = 59;
    calendar_time.sec = 55;
    calendar_time.week = 7;
    RTC_CalendarSet(&calendar_cal,&calendar_time);
} 

int main()
{
    sys_init_app();
    HAL_RTC_Init(RTC_CKSEL_LSI);
    rtc_test();
    while(1)
    {
        uint8_t status = RTC_CalendarGet(&calendar_cal,&calendar_time);
        if (HAL_OK == status)
        {
            LOG_I("%d : %d : %d  %d/%d/%d week = %d",calendar_time.hour,calendar_time.min,calendar_time.sec,calendar_cal.year,calendar_cal.mon,calendar_cal.date,calendar_time.week);
        }
        else
        {
            LOG_I("Get calendar error!");
        }
        DELAY_US(1000000);
    }

}


