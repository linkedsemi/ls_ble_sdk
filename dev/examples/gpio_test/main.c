#include "lsgpio.h"
#include "le501x.h"
#include <string.h>

uint32_t dbg_cnt_num1 = 0;
uint32_t dbg_cnt_num2 = 0;
#if 1//(DEG_OUTPUT)
int main()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    //  __HAL_RCC_GPIOB_CLK_ENABLE();
    //  __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(LSGPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

    /*Configure GPIO pin : PB1 */
       GPIO_InitStruct.Pin = GPIO_PIN_8;
       GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
       GPIO_InitStruct.OT = GPIO_OUTPUT_OPENDRAIN;
       GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_1_4_MAX_DRIVER;
       HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);
       HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_8, GPIO_PIN_SET);

       GPIO_InitStruct.Pin = GPIO_PIN_9;
       GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
       GPIO_InitStruct.OT = GPIO_OUTPUT_OPENDRAIN;
       GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_1_4_MAX_DRIVER;
       HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);
        HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_9, GPIO_PIN_SET);

       GPIO_InitStruct.Pin = GPIO_PIN_10;
       GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
       GPIO_InitStruct.OT = GPIO_OUTPUT_OPENDRAIN;
       GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_1_4_MAX_DRIVER;
       HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);
       HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_10, GPIO_PIN_SET);

       GPIO_InitStruct.Pin = GPIO_PIN_11;
       GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
       GPIO_InitStruct.OT = GPIO_OUTPUT_OPENDRAIN;
       GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_1_4_MAX_DRIVER;
       HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);
        HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_11, GPIO_PIN_SET);
    //
    //   /*Configure GPIO pin : PC1 */
    //   GPIO_InitStruct.Pin = GPIO_PIN_0;
    //   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    //   GPIO_InitStruct.OT = GPIO_OUTPUT_OPENDRAIN;
    //   GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_1_4_MAX_DRIVER;
    //   HAL_GPIO_Init(LSGPIOC, &GPIO_InitStruct);

    /*Configure GPIO pin : PA1 */
   //GPIO_InitStruct.Pin = GPIO_PIN_14;
   //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
   //GPIO_InitStruct.OT = GPIO_OUTPUT_OPENDRAIN;
   //GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_1_4_MAX_DRIVER;
   //HAL_GPIO_Init(LSGPIOA, &GPIO_InitStruct);

   //GPIO_InitStruct.Pin = GPIO_PIN_13;
   //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
   //GPIO_InitStruct.OT = GPIO_OUTPUT_PUSHPLL;
   //GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_MAX_DRIVER;
   //HAL_GPIO_Init(LSGPIOA, &GPIO_InitStruct);
    volatile uint32_t i = 0;
    volatile uint32_t j = 0;
    while (1)
    {
#if DBG_TOGGLEPIN
        HAL_GPIO_TogglePin(LSGPIOA, GPIO_PIN_14);
#endif //(DBG_TOGGLEPIN)
#if DBG_WRITEPIN
        dbg_num = 0;
        for (i = 0; i < 10000; i++)
        {
            dbg_num++;
        }
        HAL_GPIO_WritePin(LSGPIOA, GPIO_PIN_14, GPIO_PIN_SET);
        dbg_num = 0;
        for (j = 0; j < 10000; j++)
        {
            dbg_num++;
        }
        HAL_GPIO_WritePin(LSGPIOA, GPIO_PIN_13, GPIO_PIN_SET);
#endif // (DBG_WRITEPIN)

#if DBG_Deinit output
        for (i = 0; i < 1000; i++)
        {
            dbg_cnt_num2++;
        }
        dbg_cnt_num1++;
        HAL_GPIO_TogglePin(LSGPIOA, GPIO_PIN_13);
        HAL_GPIO_TogglePin(LSGPIOA, GPIO_PIN_14);
        if (dbg_cnt_num1 > 2000)
        {
            dbg_cnt_num1 = 0;
            GPIO_InitStruct.Pin = GPIO_PIN_13;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
            GPIO_InitStruct.OT = GPIO_OUTPUT_PUSHPLL;
            GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_MAX_DRIVER;
            HAL_GPIO_Init(LSGPIOA, &GPIO_InitStruct);
        }
        else if (dbg_cnt_num1 > 1000)
        {
            HAL_GPIO_DeInit(LSGPIOA, GPIO_PIN_13);
        }

#endif // (DBG_Deinit)
    }
    return 0;
}
#endif //(DEG_OUTPUT)

#if 0//(DBG_INPUT)
int main()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    //  __HAL_RCC_GPIOC_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LSGPIOA, GPIO_PIN_14, GPIO_PIN_RESET);

    /*Configure GPIO pin : PB3 */
       GPIO_InitStruct.Pin = GPIO_PIN_13;
       GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
       GPIO_InitStruct.Pull = GPIO_INPUT_NOPULL;
       GPIO_InitStruct.Filter = GPIO_OUTPUT_1_4_MAX_DRIVER;
       HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);


    /*Configure GPIO pin : PA14 */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OT = GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_1_4_MAX_DRIVER;
    HAL_GPIO_Init(LSGPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
    GPIO_InitStruct.OT = GPIO_OUTPUT_PUSHPLL;
    GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_MAX_DRIVER;
    HAL_GPIO_Init(LSGPIOA, &GPIO_InitStruct);
    volatile uint32_t i = 0;
    volatile uint32_t j = 0;
    while (1)
    {
#if DBG_TOGGLEPIN
        HAL_GPIO_TogglePin(LSGPIOA, GPIO_PIN_14);
#endif //(DBG_TOGGLEPIN)
#if DBG_WRITEPIN
        dbg_num = 0;
        for (i = 0; i < 10000; i++)
        {
            dbg_num++;
        }
        HAL_GPIO_WritePin(LSGPIOA, GPIO_PIN_14, GPIO_PIN_SET);
        dbg_num = 0;
        for (j = 0; j < 10000; j++)
        {
            dbg_num++;
        }
        HAL_GPIO_WritePin(LSGPIOA, GPIO_PIN_13, GPIO_PIN_SET);
#endif // (DBG_WRITEPIN)

#if 1 //DBG_Deinit output
        for (i = 0; i < 1000; i++)
        {
            dbg_cnt_num2++;
        }
        dbg_cnt_num1++;
        HAL_GPIO_TogglePin(LSGPIOA, GPIO_PIN_13);
        HAL_GPIO_TogglePin(LSGPIOA, GPIO_PIN_14);
        if (dbg_cnt_num1 > 2000)
        {
            dbg_cnt_num1 = 0;
            GPIO_InitStruct.Pin = GPIO_PIN_13;
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
            GPIO_InitStruct.OT = GPIO_OUTPUT_PUSHPLL;
            GPIO_InitStruct.Driver_Pwr = GPIO_OUTPUT_MAX_DRIVER;
            HAL_GPIO_Init(LSGPIOA, &GPIO_InitStruct);
        }
        else if (dbg_cnt_num1 > 1000)
        {
            HAL_GPIO_DeInit(LSGPIOA, GPIO_PIN_13);
        }

#endif // (DBG_Deinit)
    }
    return 0;
}
#endif //(DBG_INPUT)