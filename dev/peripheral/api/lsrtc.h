#ifndef LSRTC_H_
#define LSRTC_H_
#include <stdbool.h>
#include "reg_rtc_type.h"
#include "HAL_def.h"
#include "field_manipulate.h"
#include "reg_base_addr.h"

#ifdef LSRTC_BASE_ADDR
#define RTC ((reg_rtc_t *)LSRTC_BASE_ADDR)
#endif

/**
 *  @brief structure definition of calendar date & month & year.
 */
typedef struct 
{	
    uint32_t  date:5,
              mon:4,
              year:7;
}calendar_cal_t;	

/**
 *  @brief structure definition of calendar second & minute & hour & week.
 */
typedef struct 
{
    uint32_t  sec:6,
              min:6,
              hour:5,
              week:3;
}calendar_time_t;

/**
  * @brief  RTC clock source enumeration definition
  */
enum 
{
    // RTC_CKSEL_HSI,
    RTC_CKSEL_LSE = 1,
    RTC_CKSEL_LSI,
    // RTC_CKSEL_PLL,
};

/**
 ****************************************************************************************
 * @brief RTC initialization function
 *
 * @param[in]  cksel         clock source selection
 *
 ****************************************************************************************
 */
void HAL_RTC_Init(uint8_t cksel);
/**
 ****************************************************************************************
 * @brief RTC de-initialization function
 *
 ****************************************************************************************
 */
void HAL_RTC_DeInit(void);
/**
 ****************************************************************************************
 * @brief RTC calendar set function
 *
 * @param[in]  calendar_cal         parameter containing date/month/year infor to set
 * @param[in]  calendar_time        parameter containing second/minute/hour/week infor to set
 *
 ****************************************************************************************
 */
void RTC_CalendarSet(calendar_cal_t *calendar_cal, calendar_time_t *calendar_time);
/**
 ****************************************************************************************
 * @brief RTC calendar get function
 *
 * @param[out]  calendar_cal        returned date/month/year information
 * @param[out]  calendar_time       returned second/minute/hour/week information
 *
 * @return status            0: success | others: error
 ****************************************************************************************
 */
HAL_StatusTypeDef RTC_CalendarGet(calendar_cal_t *calendar_cal, calendar_time_t *calendar_time);

/**
 ****************************************************************************************
 * @brief RTC wakeup time set function
 *
 * @param[in]  second         wakeup time(the unit is second)
 *
 ****************************************************************************************
 */
void RTC_wkuptime_set(uint16_t second);

/**
 ****************************************************************************************
 * @brief RTC wakeup callback function(in LP0 mode)
 *
 ****************************************************************************************
 */
void rtc_wkup_callback(void);

#endif 

