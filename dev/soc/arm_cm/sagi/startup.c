#include "ARMCM3.h"
#include "common.h"
void SystemInit(void);
void _start(void) __attribute__ ((noreturn));
extern uint32_t __data_lma__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;

__attribute__ ((naked)) void Reset_Handler()
{
    SystemInit();
    uint32_t *dst = &__data_lma__;
    uint32_t *src = &__data_start__;
    uint32_t *end = &__data_end__;
    memcpy32(dst,src,end-src);
    _start();
}

extern uint32_t __StackTop;
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler_ASM(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void RTC1_Handler(void);
void IWDT_Handler(void);
void RTC2_Handler(void);
void EXT_Handler(void);
void LVD33_Handler(void);
void MAC_LP_Handler(void);
void MAC1_Handler(void);
void DMA_Handler(void);
void QSPI_Handler(void);
void SWINT_Handler_ASM(void);
void CACHE_Handler(void);
void I8080_Handler(void);
void TRNG_Handler(void);
void ECC_Handler(void);
void CRYPT_Handler(void);
void BSTIM1_Handler(void);
void GPTIMA1_Handler(void);
void GPTIMB1_Handler(void);
void GPTIMC1_Handler(void);
void ADTIM1_Handler(void);
void I2C1_Handler(void);
void I2C2_Handler(void);
void I2C3_Handler(void);
void UART1_Handler(void);
void UART2_Handler(void);
void UART3_Handler(void);
void SPI1_Handler(void);
void SPI2_Handler(void);
void PDM_Handler(void);
void GPIO_Handler(void);
void WWDT_Handler(void);
void SWINT2_Handler(void);
void LCD_Handler(void);
void ADC_Handler(void);
void DAC_Handler(void);
void MAC2_Handler(void);
void ADC24_Handler(void);


__VECTOR_TABLE_ATTRIBUTE void const * const __isr_vector[]={
&__StackTop        ,
Reset_Handler      ,
NMI_Handler        ,
HardFault_Handler  ,
MemManage_Handler  ,
BusFault_Handler   ,
UsageFault_Handler ,
0                  ,
0                  ,
0                  ,
0                  ,
SVC_Handler_ASM    ,
DebugMon_Handler   ,
0                  ,
PendSV_Handler     ,
SysTick_Handler    ,
RTC1_Handler	   ,
IWDT_Handler	   ,
RTC2_Handler	   ,
EXT_Handler		   ,
LVD33_Handler	   ,
MAC_LP_Handler     ,
MAC1_Handler       ,
DMA_Handler	       ,
QSPI_Handler	   ,
SWINT_Handler_ASM  ,
CACHE_Handler	   ,
I8080_Handler      ,
TRNG_Handler	   ,
ECC_Handler		   ,
CRYPT_Handler	   ,
BSTIM1_Handler	   ,
GPTIMA1_Handler	   ,
GPTIMB1_Handler	   ,
GPTIMC1_Handler	   ,
ADTIM1_Handler	   ,
I2C1_Handler	   ,
I2C2_Handler	   ,
I2C3_Handler	   ,
UART1_Handler	   ,
UART2_Handler	   ,
UART3_Handler	   ,
SPI1_Handler	   ,
SPI2_Handler	   ,
PDM_Handler		   ,
GPIO_Handler	   ,
WWDT_Handler       ,
SWINT2_Handler     ,
LCD_Handler	       ,
ADC_Handler        ,
DAC_Handler        ,
MAC2_Handler       ,
ADC24_Handler      ,
};