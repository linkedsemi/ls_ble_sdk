#include "lsrtc.h"
#include "le501x.h"
#include "platform.h"
#include "io_config.h"
#include <string.h>
#include "ls_ble.h"
#include "sleep.h"
#include "io_config.h"
#include "reg_syscfg.h"
#include "log.h"

#define RTC_CALENDAR_TEST 1
#define RTC_WAKEUP_LP0_TEST 2
#define RTC_WAKEUP_LP2_TEST 3

#define RTC_TEST_CASE 1

#if RTC_TEST_CASE == RTC_CALENDAR_TEST
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
        RTC_CalendarGet(&calendar_cal,&calendar_time);
        LOG_I("%d : %d : %d  %d/%d/%d week = %d", calendar_time.hour,
                                                  calendar_time.min,
                                                  calendar_time.sec,
                                                  calendar_cal.year,
                                                  calendar_cal.mon,
                                                  calendar_cal.date,
                                                  calendar_time.week);
        DELAY_US(1000000);
    }

}

#elif RTC_TEST_CASE == RTC_WAKEUP_LP0_TEST

#define WAKEUP_SECOND 2
#define LED_IO PA01

static void dev_manager_callback(enum dev_evt_type type,union dev_evt_u *evt)
{
    switch(type)
    {
    case STACK_INIT:
    {
        struct ble_stack_cfg cfg = {
            .private_addr = false,
            .controller_privacy = false,
        };
        dev_manager_stack_init(&cfg);
    }
    break;
    case STACK_READY:
    {
        uint8_t addr[6];
        bool type;
        dev_manager_get_identity_bdaddr(addr,&type);
        LOG_I("type:%d,addr:",type);
        LOG_HEX(addr,sizeof(addr));
    }
    break;
    default:

    break;
    }
}

void rtc_wkup_callback(void)
{
    LOG_I("RTC wakeup!");
    io_cfg_output(LED_IO);
    io_set_pin(LED_IO);
    DELAY_US(200*1000);
    io_clr_pin(LED_IO);
    io_cfg_input(LED_IO);
    io_cfg_disable(LED_IO);
}

int main(void)
{
    sys_init_app();
    ble_init();
    dev_manager_init(dev_manager_callback);
    HAL_RTC_Init(RTC_CKSEL_LSI);
    RTC_wkuptime_set(WAKEUP_SECOND);
    ble_loop();
}
#elif RTC_TEST_CASE == RTC_WAKEUP_LP2_TEST
#define WAKEUP_SECOND 2
#define SLEEP_CNT_MAX 3
#define LED_IO PA01

static calendar_time_t calendar_time;
static calendar_cal_t calendar_cal;
// set initial value of calendar to 2024/2/28 23:59:57 week = 7
static void calendar_init(void)
{
    calendar_cal.year = 24;
    calendar_cal.mon = 2;
    calendar_cal.date = 28;
    calendar_time.hour = 23;
    calendar_time.min = 59;
    calendar_time.sec = 57;
    calendar_time.week = 7;
    RTC_CalendarSet(&calendar_cal,&calendar_time);
} 

static void dev_manager_callback(enum dev_evt_type type,union dev_evt_u *evt)
{
    switch(type)
    {
    case STACK_INIT:
    {
        struct ble_stack_cfg cfg = {
            .private_addr = false,
            .controller_privacy = false,
        };
        dev_manager_stack_init(&cfg);
    }
    break;
    case STACK_READY:
    {
        uint8_t addr[6];
        bool type;
        dev_manager_get_identity_bdaddr(addr,&type);
        LOG_I("type:%d,addr:",type);
        LOG_HEX(addr,sizeof(addr));

        RTC_CalendarGet(&calendar_cal,&calendar_time);
        LOG_I("%d : %d : %d  %d/%d/%d week = %d",calendar_time.hour,calendar_time.min,calendar_time.sec,calendar_cal.year,calendar_cal.mon,calendar_cal.date,calendar_time.week);

        uint8_t wkup_source = get_wakeup_source();
        if ((RTC_WKUP & wkup_source) != 0)
        {
            io_cfg_output(LED_IO);
            io_set_pin(LED_IO);
            DELAY_US(200*1000); // toggle LED IO and delay
            io_clr_pin(LED_IO);
            io_cfg_input(LED_IO);
            io_cfg_disable(LED_IO);

            SYSCFG->BKD[7]++; // record times of entering sleep
        }
        else
        {
            calendar_init(); // set calender at first time
            SYSCFG->BKD[7] = 0;
        }

        if (SYSCFG->BKD[7] == SLEEP_CNT_MAX)
        {
            LOG_I("RTC LP2 calendar test end here~");
            while (1);
        }   

        struct deep_sleep_wakeup wakeup;
        memset(&wakeup,0,sizeof(wakeup));
        wakeup.rtc = 1;
        enter_deep_sleep_mode_lvl2_lvl3(&wakeup);
    }
    break;
    default:
    break;
    }
}

int main(void)
{
    sys_init_app();
    ble_init();
    dev_manager_init(dev_manager_callback);

    HAL_RTC_Init(RTC_CKSEL_LSI);
    RTC_wkuptime_set(WAKEUP_SECOND);
    ble_loop();
}
#endif
