RTC
=======

本节主要介绍RTC的常用功能和配置。RTC的操作API详见 ``lsrtc.h`` 。
RTC的主要功能包括万年历功能和唤醒功能（LP0 & LP2模式）。

一、初始化
------------

使用RTC的功能，首先需要调用RTC的初始化函数：

.. code ::

    void HAL_RTC_Init(uint8_t cksel);

其中参数cksel表示时钟源。如果使用外部32K晶振输入，该参数设置为RTC_CKSEL_LSE，否则需要设置成RTC_CKSEL_LSI。

二、功能配置
-------------

2.1 万年历功能
+++++++++++++++

使用万年历功能之前，首先需要配置当前的年月日时分秒，参考代码如下：

.. code ::

    static calendar_time_t calendar_time;
    static calendar_cal_t calendar_cal;
    static void rtc_calendar_set()
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

万年历能配置的最早年份为2020年，即year为0。如上所示，初始化的时间点为2024年2月28日23点59分55秒周日，然后调用RTC_CalendarSet函数进行设置。
之后万年历便开始运行，在休眠模式（LP0 & LP2）下也是如此。
获取万年历当前数值的接口为：

.. code ::

    HAL_StatusTypeDef RTC_CalendarGet(calendar_cal_t *calendar_cal, calendar_time_t *calendar_time);

调用返回值为HAL_OK时，当前的万年历信息会从参数里返回出来。如果返回其他值，则表示返回值异常，需要重新调用此接口获取。

2.2 唤醒功能（LP0模式）
++++++++++++++++++++++++

在LP0模式下，可以通过配置RTC wakeup中断对系统进行定时唤醒。配置系统时间的接口如下：

.. code ::

    void RTC_wkuptime_set(uint32_t second);

参数second为唤醒时间，单位为秒。
当系统唤醒后，会调用rtc_wkup_callback函数：

.. code ::

    void rtc_wkup_callback(void);

**注意：该函数为全局函数，需要严格按照如上述形式实现，不可以加static**

2.3 唤醒模式（LP2模式）
++++++++++++++++++++++++

在LP2模式下，RTC唤醒系统的接口和LP0模式一样：

.. code ::

    void RTC_wkuptime_set(uint32_t second);

另外，进入LP2模式需要调用enter_deep_sleep_mode_lvl2_lvl3()，如下：

.. code ::

    struct deep_sleep_wakeup wakeup;
    memset(&wakeup,0,sizeof(wakeup));
    wakeup.rtc = 1;
    enter_deep_sleep_mode_lvl2_lvl3(&wakeup);

wakeup.rtc = 1是配置RTC为唤醒源。唤醒源还可以是某些GPIO，NRST或者WDT等。类似配置，此处不再赘述。
与LP0模式不同的是，LP2模式下RTC唤醒系统后不会调用rtc_wkup_callback函数。应用里，需要通过调用get_wakeup_source()获取唤醒源信息：

.. code ::

    uint8_t wkup_source = get_wakeup_source();
    if ((RTC_WKUP & wkup_source) != 0)
    {
    	// do something for rtc wakeup
    }

三、注意事项
-------------

#. RTC定时唤醒系统不会很准，尤其是LP2模式。LP0模式下RTC上中断到进入rtc_wkup_callback会有通常不超过10ms的误差，而LP2模式下由于系统会重启，因此误差往往在100ms以上