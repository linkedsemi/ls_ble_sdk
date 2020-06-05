#include "lsgpio.h"
#include "le501x.h"
#include <string.h>

#define GPIO_TEST 0
#define EXTI_TEST 1

#if (GPIO_TEST)
void gpio_test(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin*/
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_INPUT_NOPULL;

    HAL_GPIO_Init(LSGPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OT = GPIO_OUTPUT_PUSHPLL;
    GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_MAX_DRIVER;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);

    while (1)
    {
        HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_8, HAL_GPIO_ReadPin(LSGPIOA, GPIO_PIN_0));
        HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_9, HAL_GPIO_ReadPin(LSGPIOA, GPIO_PIN_7));
    }
}
#endif //(GPIO_TEST)

#if (EXTI_TEST)
void exti_test(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OT = GPIO_OUTPUT_PUSHPLL;
    GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_MAX_DRIVER;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin*/
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FLT;
    GPIO_InitStruct.Pull = GPIO_INPUT_NOPULL;
    GPIO_InitStruct.Filter = GPIO_INPUT_FLI_20NS_EN;

    GPIO_InitStruct.Edbc_filter.Cnt = 7;
    GPIO_InitStruct.Edbc_filter.Pre = 255;

    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);

    NVIC_SetPriority(EXTI_IRQn, 1);
    NVIC_EnableIRQ(EXTI_IRQn);
}
void EXTI_Handler(void)
{
    /*clear Exti*/
    WRITE_REG(EXTI->EICR, READ_REG(EXTI->ERIF)); 

    HAL_GPIO_TogglePin(LSGPIOB, GPIO_PIN_8);
}
#endif //(EXTI_TEST)
int main(void)
{
    #if (GPIO_TEST)
    gpio_test();
    #endif

    #if (EXTI_TEST)
    exti_test();
    #endif

    while (1)
    {
        ;
    }
}
