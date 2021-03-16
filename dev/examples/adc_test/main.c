#include "lsadc.h"
#include "le501x.h"
#include "platform.h"
#include "io_config.h"
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "lsdmac.h"

/* ADC work mode -------------------------------------------------------------*/
#define DMA_EN
//#define Injected_Mode
/* Private function prototypes -----------------------------------------------*/

static void Error_Handler(void);

/* Buffer used for reception */
#ifdef DMA_EN
DMA_RAM_ATTR uint16_t aRxBuffer[16];
DEF_DMA_CONTROLLER(dmac1_inst,DMAC1);
#else
uint16_t aRxBuffer[16];
#endif

ADC_HandleTypeDef hadc;


volatile uint8_t recv_flag = 0;

/* Private functions ---------------------------------------------------------*/

static void adc_io_init(void)
{
   adc12b_in5_io_init();
}

void lsadc_init(void)
{

#ifdef DMA_EN
    DMA_CONTROLLER_INIT(dmac1_inst);
    hadc.Env.DMA.DMA_Channel        = 0;
    hadc.DMAC_Instance              = &dmac1_inst;
    /** Common config  */
    hadc.Instance = LSADC;
    hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode          = ADC_SCAN_DISABLE;              /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
    hadc.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode to have maximum conversion speed (no delay between conversions) */
    hadc.Init.NbrOfConversion       = 12;                             /* Parameter discarded because sequencer is disabled */
    hadc.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
    hadc.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
    hadc.Init.TrigType      = ADC_REGULAR_SOFTWARE_TRIGT;            /* Trig of conversion start done by which event */
    hadc.Init.Vref          = ADC_VREF_INSIDE;                       /* The reference voltage uses an internal reference */
#else
    /** Common config  */
    hadc.Instance = LSADC;
    hadc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode          = ADC_SCAN_DISABLE;              /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
    hadc.Init.NbrOfConversion       = 12;                            /* Parameter discarded because sequencer is disabled */
    hadc.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
    hadc.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
    #ifndef Injected_Mode
    hadc.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode to have maximum conversion speed (no delay between conversions) */
    hadc.Init.TrigType      = ADC_REGULAR_SOFTWARE_TRIGT;            /* Trig of conversion start done by which event */
    #else
    hadc.Init.ContinuousConvMode    = DISABLE;                        /* Continuous mode to have maximum conversion speed (no delay between conversions) */
    hadc.Init.TrigType      = ADC_INJECTED_SOFTWARE_TRIGT;            /* The reference voltage uses an internal reference */
    hadc.Init.Vref          = ADC_VREF_INSIDE;
    #endif    

#endif
    if (HAL_ADC_Init(&hadc) != HAL_OK)
    {
        Error_Handler();
    }

#ifndef Injected_Mode
    ADC_ChannelConfTypeDef sConfig = {0};
    /** 
     * Configure Regular Channel 
    */
    sConfig.Channel      = ADC_CHANNEL_5;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
		sConfig.Channel  = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank         = ADC_REGULAR_RANK_2;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }
		sConfig.Channel  = ADC_CHANNEL_5;
    sConfig.Rank         = ADC_REGULAR_RANK_3;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }	
		sConfig.Channel  = ADC_CHANNEL_VBAT;
    sConfig.Rank         = ADC_REGULAR_RANK_4;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }	
		sConfig.Channel  = ADC_CHANNEL_5;
    sConfig.Rank         = ADC_REGULAR_RANK_5;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }	
		sConfig.Channel  = ADC_CHANNEL_VBAT;
    sConfig.Rank         = ADC_REGULAR_RANK_6;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }			
		sConfig.Channel  = ADC_CHANNEL_5;
    sConfig.Rank         = ADC_REGULAR_RANK_7;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }	
		sConfig.Channel  = ADC_CHANNEL_VBAT;
    sConfig.Rank         = ADC_REGULAR_RANK_8;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }	
		sConfig.Channel  = ADC_CHANNEL_5;
    sConfig.Rank         = ADC_REGULAR_RANK_9;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }	
		sConfig.Channel  = ADC_CHANNEL_VBAT;
    sConfig.Rank         = ADC_REGULAR_RANK_10;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }	
		sConfig.Channel  = ADC_CHANNEL_5;
    sConfig.Rank         = ADC_REGULAR_RANK_11;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }	
		sConfig.Channel  = ADC_CHANNEL_VBAT;
    sConfig.Rank         = ADC_REGULAR_RANK_12;
    sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;//ADC_SAMPLETIME_15CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
    {
       Error_Handler();
    }
#else
    ADC_InjectionConfTypeDef sConfigInjected = {0};
    /* Configure ADC injected channel */
    sConfigInjected.InjectedChannel = ADC_CHANNEL_VBAT;
    sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
    sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_15CYCLES;
    sConfigInjected.InjectedOffset = 0;
    sConfigInjected.InjectedNbrOfConversion = 1;
    sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
    sConfigInjected.AutoInjectedConv = DISABLE;
  if (HAL_ADCEx_InjectedConfigChannel(&hadc, &sConfigInjected) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
#endif

}

int main(void)
{
    sys_init_app();
    __enable_irq();
	adc_io_init();
	lsadc_init();

#ifdef DMA_EN
  /* Start ADC conversion on regular group with transfer by DMA */
   if (HAL_ADC_Start_DMA(&hadc, (uint16_t *)aRxBuffer, 12, HAL_ADC_ConvCpltCallback) != HAL_OK)
   {
     /* Start Error */
     Error_Handler();
   }
#else
  /* Start ADC conversion on regular group with transfer by IT */
    #ifndef Injected_Mode
	HAL_ADC_Start_IT(&hadc);
    #else
    HAL_ADCEx_InjectedStart_IT(&hadc);
    #endif
#endif
    recv_flag = 0;
	while(1)
	{
#ifdef DMA_EN
		if(recv_flag == 1) 
		{
		  	if (HAL_ADC_Start_DMA(&hadc,(uint16_t *)aRxBuffer,12, HAL_ADC_ConvCpltCallback ) != HAL_OK)
            {
                /* Start Error */
                Error_Handler();
            }
            recv_flag = 0;

		}
#else
		if(recv_flag >= 12) 
		{
            LOG_I("ADC finish!triger again!");
            DELAY_US(1000);
            recv_flag = 0;
			HAL_ADC_Start_IT(&hadc);
		}
#endif		
	};
	
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

/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  hadc: ADC handle
  * @retval None
  */

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
#ifdef DMA_EN
    recv_flag = 1;
#else
   if(recv_flag<12)
    aRxBuffer[recv_flag++] = HAL_ADC_GetValue(hadc);
#endif	
   // HAL_ADC_Stop_IT(hadc);
}
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    aRxBuffer[recv_flag++] = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
}

