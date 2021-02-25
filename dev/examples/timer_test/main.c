#include "lstimer.h"
#include <string.h>
#include "io_config.h"
#include "platform.h"
#include "log.h"

#define TEST_CASE  0

#if(TEST_CASE==0)
#define PWM_TEST
#elif(TEST_CASE==1)
#define BASIC_TIM_TEST
#elif(TEST_CASE==2)
#define DTC_PWM_TEST
#elif(TEST_CASE==3)
#define IC_TEST
#endif

#define TIM_PRESCALER     (SDK_HCLK_MHZ-1)
#define TIM_PERIOD        (250 - 1) /* Period Value  */
#define TIM_PULSE1        125       /* Capture Compare 1 Value  */
#define TIM_PULSE2        200       /* Capture Compare 2 Value  */
#define TIM_PULSE3        100       /* Capture Compare 3 Value  */
#define TIM_PULSE4        50        /* Capture Compare 4 Value  */

TIM_HandleTypeDef TimHandle;
typedef struct
{
    uint8_t ucStartFlag; // Capture start flag bit
    uint16_t usCtr;      // Capture reg value
    uint16_t usPeriod;   // Automatically reload register update flags
}capture_t;
capture_t TIM_ICUserValueStructure = {0};

static void Basic_PWM_Output_Cfg(void)
{
#ifdef PWM_TEST
    TIM_OC_InitTypeDef sConfig;

    gptimb1_ch1_io_init(PA00, true, 0);
    gptimb1_ch2_io_init(PA01, true, 0);
    gptimb1_ch3_io_init(PB14, true, 0);
    gptimb1_ch4_io_init(PB15, true, 0);
    /*##-1- Configure the TIM peripheral #######################################*/
    TimHandle.Instance = LSGPTIMB;
    TimHandle.Init.Prescaler = TIM_PRESCALER; 
    TimHandle.Init.Period = TIM_PERIOD;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Init(&TimHandle);

    /*##-2- Configure the PWM channels #########################################*/
    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;

    sConfig.Pulse = TIM_PULSE1;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);

    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.Pulse = TIM_PULSE2;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_2);

    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.Pulse = TIM_PULSE3;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3);

    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.Pulse = TIM_PULSE4;
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_4);

    /*##-3- Start PWM signals generation #######################################*/
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_4);
#endif
}

void Basic_Timer_Cfg(void)
{
#ifdef BASIC_TIM_TEST  
    io_cfg_output(PA00);
    io_write_pin(PA00,0);

    TimHandle.Instance           = LSBSTIM;
    TimHandle.Init.Prescaler     = TIM_PRESCALER;
    TimHandle.Init.Period        = TIM_PERIOD;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
    HAL_TIM_Init(&TimHandle);
    HAL_TIM_Base_Start_IT(&TimHandle);
#endif
}

static void DeadTime_Complementary_PWM_Cfg(void)
{
#ifdef DTC_PWM_TEST
    adtim1_ch1_io_init(PA00,true,0);
    adtim1_ch1n_io_init(PA01);
    TIM_BreakDeadTimeConfigTypeDef BDT_Config;
    TIM_OC_InitTypeDef sConfig;
    TimHandle.Instance = LSADTIM;
    TimHandle.Init.Prescaler = TIM_PRESCALER;
    TimHandle.Init.Period = TIM_PERIOD;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Init(&TimHandle);

    BDT_Config.OffStateRunMode = TIM_OSSR_DISABLE;
    BDT_Config.OffStateIDLEMode = TIM_OSSI_DISABLE;
    BDT_Config.LockLevel = TIM_LOCKLEVEL_OFF;
    BDT_Config.DeadTime = 0x3f;//0~0xFF
    BDT_Config.BreakState = TIM_BREAK_DISABLE;
    BDT_Config.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    BDT_Config.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    HAL_TIMEx_ConfigBreakDeadTime(&TimHandle,&BDT_Config);

    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;
    sConfig.Pulse = TIM_PULSE3;
        
    HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&TimHandle, TIM_CHANNEL_1);
#endif
}

static void Input_Capture_Measurement_Cfg(void)
{
#ifdef IC_TEST
    TIM_IC_InitTypeDef ICConfig;
    //Toggle IO Cfg
    io_cfg_output(PA01);
    io_write_pin(PA01,0);
    //Input Capture IO Cfg 
    gptimc1_ch1_io_init(PA00,false,0);
    io_pull_write(PA00,IO_PULL_DOWN);

    TimHandle.Instance = LSGPTIMC;
    TimHandle.Init.Prescaler = TIM_PRESCALER;
    TimHandle.Init.Period = 0xFFFF;
    TimHandle.Init.ClockDivision = 0;
    TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Init(&TimHandle);

    ICConfig.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    ICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
    ICConfig.ICPrescaler = TIM_ICPSC_DIV1;
    ICConfig.ICFilter = 0;
    HAL_TIM_IC_ConfigChannel(&TimHandle, &ICConfig, TIM_CHANNEL_1);

    HAL_TIM_Base_Start_IT(&TimHandle);  
    HAL_TIM_IC_Start_IT(&TimHandle,TIM_CHANNEL_1);
#endif
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == LSBSTIM)
    {
        #ifdef BASIC_TIM_TEST 
        io_toggle_pin(PA00);
        #endif
    }
    else if(htim->Instance == LSGPTIMC)
    {
        #ifdef IC_TEST
        TIM_ICUserValueStructure.usPeriod++;
        #endif
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
#ifdef IC_TEST
    TIM_IC_InitTypeDef IC_Config;
    if (TIM_ICUserValueStructure.ucStartFlag == 0)
    {
        io_toggle_pin(PA01);
        // clear timer counts
        __HAL_TIM_SET_COUNTER(htim, 0); 
        TIM_ICUserValueStructure.usPeriod = 0;
        TIM_ICUserValueStructure.usCtr = 0;

        // Configure the input capture parameters, modify the trigger level
        IC_Config.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
        IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
        IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
        IC_Config.ICFilter = 0;
        HAL_TIM_IC_ConfigChannel(&TimHandle, &IC_Config, TIM_CHANNEL_1);
        // Clear interrupt flag bits
        __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
        // Start input capture and enable interrupts
        HAL_TIM_IC_Start_IT(&TimHandle, TIM_CHANNEL_1);
        TIM_ICUserValueStructure.ucStartFlag = 1;
    }
    else
    {
        io_toggle_pin(PA01);
        // get timer counter
        TIM_ICUserValueStructure.usCtr = HAL_TIM_ReadCapturedValue(&TimHandle, TIM_CHANNEL_1);
        // Configure the input capture parameters, modify the trigger level
        IC_Config.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
        IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
        IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
        IC_Config.ICFilter = 0;
        HAL_TIM_IC_ConfigChannel(&TimHandle, &IC_Config, TIM_CHANNEL_1);

        // Clear interrupt flag bits
        __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
        // Start input capture and enable interrupts
        HAL_TIM_IC_Start_IT(&TimHandle, TIM_CHANNEL_1);
        TIM_ICUserValueStructure.ucStartFlag = 0;

        LOG_I("IC Time:%d us",TIM_ICUserValueStructure .usPeriod*65535+TIM_ICUserValueStructure.usCtr);
    }
#endif
}

int main(void)
{
    sys_init_app();
    Basic_PWM_Output_Cfg();
    Basic_Timer_Cfg();
    DeadTime_Complementary_PWM_Cfg();
    Input_Capture_Measurement_Cfg();
    while (1);
}

