#include "ls_ble.h"
#include "platform.h"


void uart_eif_read(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy)
{


}

void uart_eif_write(uint8_t *bufptr, uint32_t size, void (*callback)(void *,uint8_t), void* dummy)
{


}

void uart_eif_flow_on(void)
{

}

bool uart_eif_flow_off(void)
{
    return true;
}


int main()
{
    sys_init_itf();
    ble_init();
    while(1)
    {
        ble_loop();
    }
    return 0;
}

