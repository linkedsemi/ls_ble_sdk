#include "uart.h"
#include "lsgpio.h"
#include "le501x.h"
#include "platform.h"
#include "io_config.h"
#include <string.h>

#define TEST_ZONE_SIZE 512
static void uart_test_init(void);
static void gpio_init();

UART_HandleTypeDef UART_Config; 
uint8_t test_zone_a[TEST_ZONE_SIZE * 2] ;
uint8_t test_zone_b[TEST_ZONE_SIZE * 2] ;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart,void *tx_arg)
{
    HAL_UART_DeInit(huart);
    uart3_io_deinit();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart,void *rx_arg)
{
    HAL_UART_Transmit_IT(&UART_Config,test_zone_b,8,NULL);
}

static void gpio_init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
//  PB11 UART3_RX
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF;
    GPIO_InitStruct.AF_Type = AF_UART3_RXD;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);
//  PB15  UART3_TX
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF;
    GPIO_InitStruct.AF_Type = AF_UART3_TXD;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);
    uart3_io_init(LSGPIOB_15,LSGPIOB_11);

}

static void uart_test_init(void)
{
    UART_Config.UARTX = UART3;
    UART_Config.Init.BaudRate = UART_BAUDRATE_115200;
    UART_Config.Init.MSBEN = 0;
    UART_Config.Init.Parity = UART_NOPARITY;
    UART_Config.Init.StopBits = UART_STOPBITS1;
    UART_Config.Init.WordLength = UART_BYTESIZE8;
    HAL_UART_Init(&UART_Config);
}

static void uart_test()
{
    HAL_UART_Receive_IT(&UART_Config,test_zone_b,8,NULL);
}

int main()
{
    sys_init_app();
    gpio_init();
    uart_test_init();
    uart_test();
    while(1)
    {
    }
}




