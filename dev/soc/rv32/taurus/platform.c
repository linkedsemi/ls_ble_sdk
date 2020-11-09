#include "taurus.h"
#include "core_rv32.h"

__attribute__((weak)) void SystemInit(){}

void sys_init_itf()
{
    
    
}

void ble_pkt_irq_mask()
{
    csi_vic_disable_irq(MAC1_IRQn);
}

void ble_pkt_irq_unmask()
{
    csi_vic_enable_irq(MAC1_IRQn);
}

void swint2_set()
{
    csi_vic_set_pending_irq(SWINT2_IRQn);
}