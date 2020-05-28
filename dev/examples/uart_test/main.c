#include "uart.h"
#include "lsgpio.h"
#include "le501x.h"
#include "platform.h"
#include <string.h>


#define TEST_ZONE_SIZE 512


UART_HandleTypeDef UART_Config; 
uint8_t test_zone_a[TEST_ZONE_SIZE * 2] ;
uint8_t test_zone_b[TEST_ZONE_SIZE * 2] ;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart,void *tx_arg)
{
    HAL_UART_Receive_IT(&UART_Config,test_zone_b,1,NULL);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart,void *rx_arg)
{
    HAL_UART_Transmit_IT(&UART_Config,test_zone_b,1,NULL);
}

static void gpio_init()
{
//  PB08 UART3_RX
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF;
    GPIO_InitStruct.AF_Type = AF_UART3_RXD;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_11, GPIO_PIN_SET);
//  PB09  UART3_TX
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF;
    GPIO_InitStruct.AF_Type = AF_UART3_TXD;
    HAL_GPIO_Init(LSGPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LSGPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}

static void uart_test_init(void)
{
    uint16_t i ;
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
//    HAL_UART_Transmit(&UART_Config,test_zone_a,0x10,0);
//    HAL_UART_Transmit_IT(&UART_Config,test_zone_a,10,NULL);
//    HAL_UART_Receive(&UART_Config,test_zone_b,0x10,0);
    HAL_UART_Receive_IT(&UART_Config,test_zone_b,1,NULL);    

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




