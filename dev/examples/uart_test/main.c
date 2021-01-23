#include "lsuart.h"
#include "le501x.h"
#include "platform.h"
#include "io_config.h"
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "lsdmac.h"

DEF_DMA_CONTROLLER(dmac1_inst,DMAC1);
UART_HandleTypeDef UART_Config; 
#define BUF_SIZE 256
DMA_RAM_ATTR uint8_t test_zone_a[BUF_SIZE];

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    //UART disable
    // HAL_UART_DeInit(huart);
    // uart1_io_deinit();
    /*code */

    /* user code end */

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit_IT(&UART_Config,test_zone_a,1);
    HAL_UART_Receive_IT(&UART_Config,test_zone_a,1);
}

static void uart_init(void)
{
    DMA_CONTROLLER_INIT(dmac1_inst);
    UART_Config.UARTX = UART1;
    UART_Config.DMAC_Instance = &dmac1_inst;
    UART_Config.Init.BaudRate = UART_BAUDRATE_115200;
    UART_Config.Init.MSBEN = 0;
    UART_Config.Init.Parity = UART_NOPARITY;
    UART_Config.Init.StopBits = UART_STOPBITS1;
    UART_Config.Init.WordLength = UART_BYTESIZE8;
    HAL_UART_Init(&UART_Config);
}

static void uart_dma_tx_callback()
{
    LOG_I("%s",__FUNCTION__);
}

static void uart_tx_dma_test()
{
    UART_Config.Tx_Env.DMA.DMA_Channel = 0;
    HAL_UART_Transmit_DMA(&UART_Config,test_zone_a,5,uart_dma_tx_callback);

}

static void uart_dma_rx_callback()
{
    LOG_I("%s",__FUNCTION__);
}

static void uart_rx_dma_test()
{
    UART_Config.Rx_Env.DMA.DMA_Channel = 1;
    HAL_UART_Receive_DMA(&UART_Config,test_zone_a,5,uart_dma_rx_callback);
}

int main()
{
    sys_init_app();
    uart1_io_init(PB00,PB01);
    uart_init();
    uint16_t i;
    for(i=0;i<BUF_SIZE;++i)
    {
        test_zone_a[i] = i;
    }
    uart_rx_dma_test();
    uart_tx_dma_test();
    while(1)
    {
    }
}




