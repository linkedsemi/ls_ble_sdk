#include "ls_ble.h"
#include "platform.h"
#include "uart.h"
UART_HandleTypeDef UART_Config; 
static void (*eif_read_callback)(void *,uint8_t);
static void (*eif_write_callback)(void *,uint8_t);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart,void *tx_arg)
{
    if(huart == &UART_Config)
    {
        eif_write_callback(tx_arg,0);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart,void *rx_arg)
{
    if(huart == &UART_Config)
    {
        eif_read_callback(rx_arg,0);
    }
}

void uart_eif_read(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy)
{
    eif_read_callback = callback;
    HAL_UART_Receive_IT(&UART_Config,bufptr,size,dummy);
}

void uart_eif_write(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy)
{
    eif_write_callback = callback;
    HAL_UART_Transmit_IT(&UART_Config,bufptr,size,dummy);
}

void uart_eif_flow_on(void)
{

}

bool uart_eif_flow_off(void)
{
    return true;
}

static void uart_test_init(void)
{
    UART_Config.UARTX = UART1;
    UART_Config.Init.BaudRate = UART_BAUDRATE_9600;
    UART_Config.Init.MSBEN = 0;
    UART_Config.Init.Parity = UART_NOPARITY;
    UART_Config.Init.StopBits = UART_STOPBITS1;
    UART_Config.Init.WordLength = UART_BYTESIZE8;
    HAL_UART_Init(&UART_Config);
}

#include "reg_rcc.h"
#include "field_manipulate.h"
static void switch_to_rc32k()
{
    REG_FIELD_WR(RCC->CFG, RCC_SYSCLK_SW, 2);
    REG_FIELD_WR(RCC->CFG, RCC_CKCFG, 1);
}

static void switch_to_pll64m()
{
    REG_FIELD_WR(RCC->CFG, RCC_SYSCLK_SW, 4);
    REG_FIELD_WR(RCC->CFG, RCC_CKCFG, 1);
}

static void switch_to_xo16m()
{
    REG_FIELD_WR(RCC->CFG, RCC_SYSCLK_SW, 1);
    REG_FIELD_WR(RCC->CFG, RCC_CKCFG, 1);
}

static void rc24m_switch_to_pll64m()
{
    switch_to_rc32k();
    switch_to_pll64m();
}


int main()
{
    switch_to_xo16m();
    //rc24m_switch_to_pll64m();
    sys_init_itf();
    uart_test_init();
    ble_init();
    while(1)
    {
        ble_loop();
    }
    return 0;
}

