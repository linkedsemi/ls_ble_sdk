#include "ls_ble.h"
#include "platform.h"

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

