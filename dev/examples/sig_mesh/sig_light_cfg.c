#include "sig_light_cfg.h"

TIM_HandleTypeDef light_tim_hdl;
TIM_OC_InitTypeDef light_tim_cfg;
static struct light_state sigmesh_light_state;

static void light_control(bool lightness_flag)
{
    if((!lightness_flag) && (sigmesh_light_state.onoff_state == 0))
    {
        light_tim_cfg.Pulse = 0;

        HAL_TIM_PWM_ConfigChannel(&light_tim_hdl, &light_tim_cfg, TIM_CHANNEL_1); 
        HAL_TIM_PWM_Start(&light_tim_hdl, TIM_CHANNEL_1);

        HAL_TIM_PWM_ConfigChannel(&light_tim_hdl, &light_tim_cfg, TIM_CHANNEL_2); 
        HAL_TIM_PWM_Start(&light_tim_hdl, TIM_CHANNEL_2);
    }
    else
    {
        light_tim_cfg.Pulse = sigmesh_light_state.level;
        
        HAL_TIM_PWM_ConfigChannel(&light_tim_hdl, &light_tim_cfg, TIM_CHANNEL_1); 
        HAL_TIM_PWM_Start(&light_tim_hdl, TIM_CHANNEL_1);
        
        HAL_TIM_PWM_ConfigChannel(&light_tim_hdl, &light_tim_cfg, TIM_CHANNEL_2); 
        HAL_TIM_PWM_Start(&light_tim_hdl, TIM_CHANNEL_2);
    }
}

void sigmesh_set_lightness(uint16_t level)
{
    if(sigmesh_light_state.level != level)
    {
        sigmesh_light_state.onoff_state = 1;
        sigmesh_light_state.level = level;
        light_control(true);
    }
}

void sigmesh_set_onoff(uint8_t on_off)
{
    if(sigmesh_light_state.onoff_state != on_off)
    {
        if(sigmesh_light_state.level == 0)
        {
            sigmesh_light_state.level = 0x5ff;
        }
        sigmesh_light_state.onoff_state = on_off;
        light_control(false);
    }
}

uint8_t sigmesh_get_onoff(void)
{
    return sigmesh_light_state.onoff_state;
}

uint16_t sigmesh_get_lightness(void)
{
    return sigmesh_light_state.level;
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef   GPIO_InitStruct;
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* TIMx Peripheral clock enable */
    REG_FIELD_WR(RCC->APB1EN, RCC_GPTIMB1, 1);
    /* Enable all GPIO Channels Clock requested */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /* Configure PA00, PA01 for PWM output*/
    GPIO_InitStruct.Mode = GPIO_MODE_AF;
    GPIO_InitStruct.AF_Type = AF_GPTIMB1_CH1;

    GPIO_InitStruct.Pin = LED_PIN_0;
    HAL_GPIO_Init(LED_PORT_0, &GPIO_InitStruct);

    GPIO_InitStruct.AF_Type = AF_GPTIMB1_CH2;

    GPIO_InitStruct.Pin = LED_PIN_1;
    HAL_GPIO_Init(LED_PORT_1, &GPIO_InitStruct);
}

void sigmesh_pwm_init(void)
{
    uint16_t level_t = 0x5FF;
    light_tim_hdl.Instance = TIMx;
  
    light_tim_hdl.Init.Prescaler = 0;
    light_tim_hdl.Init.Period = 0xffff -1;
    light_tim_hdl.Init.ClockDivision = 0;
    light_tim_hdl.Init.CounterMode = TIM_COUNTERMODE_UP;
    light_tim_hdl.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_PWM_Init(&light_tim_hdl);

    light_tim_cfg.OCMode = TIM_OCMODE_PWM1;
    light_tim_cfg.OCPolarity = TIM_OCPOLARITY_HIGH;
    light_tim_cfg.OCFastMode = TIM_OCFAST_DISABLE;

    sigmesh_set_lightness(level_t);
}

