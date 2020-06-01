#include "lsgpio.h"
#include "lsadc.h"
#include "le501x.h"
#include "reg_lsadc.h"
#include "lsiwdt.h"

#include <string.h>

void Error_Handler(void);
void software_trig(void);
ADC_HandleTypeDef hadc;
static void lsgpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LSGPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

    /*Configure GPIO pin : PA00 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.ANA_Func = ANA_FUNC1;
    HAL_GPIO_Init(LSGPIOA, &GPIO_InitStruct);

    __HAL_RCC_GPIOA_CLK_DISABLE();
}

void lsadc_init(void)
{
    __HAL_RCC_ADC_CLK_ENABLE();

    ADC_ChannelConfTypeDef sConfig = {0};

    /** Common config 
  */
    hadc.Instance = LSADC;
    hadc.Init.DiscontinuousConvMode = ENABLE;
    hadc.ConvCpltCallback = HAL_ADC_ConvCpltCallback;
    if (HAL_ADC_Init(&hadc) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Regular Channel 
  */
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

int main(void)
{
    lsgpio_init();
    lsiwdt_Init();
    lsadc_init();
    NVIC_EnableIRQ(ADC_IRQn); //ADC_IRQHandler
    HAL_ADC_Start_IT(&hadc);
    while (1)
    {
        HAL_ADC_Start_IT(&hadc);
    }
    return 0;
}

/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */
uint32_t rd_adc_data = 0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    /* Prevent unused argument(s) compilation warning */
    //UNUSED(hadc);
    /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvCpltCallback must be implemented in the user file.
   */
    rd_adc_data = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop_IT(hadc);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

void ADC_Handler(void)
{
    HAL_ADC_IRQHandler(&hadc);
}
