#include "lsqspi_param.h"
#include "field_manipulate.h"
#include "compile_flag.h"
#include "reg_sysc_cpu.h"
#include "ARMCM3.h"
#include "platform.h"
XIP_BANNED void lsqspi_sw_rst(struct lsqspi_instance *inst)
{
    lsqspi_clk_set(inst,false);
    SYSC_CPU->PD_CPU_SRST = SYSC_CPU_SRST_CLR_LSQSPI0_N_MASK;
    SYSC_CPU->PD_CPU_SRST = SYSC_CPU_SRST_SET_LSQSPI0_N_MASK;
}

XIP_BANNED void lsqspi_clk_set(struct lsqspi_instance *inst,bool enable)
{
    if(enable)
    {
        SYSC_CPU->PD_CPU_CLKG = SYSC_CPU_CLKG_SET_LSQSPI0_MASK;
    }else
    {
        SYSC_CPU->PD_CPU_CLKG = SYSC_CPU_CLKG_CLR_LSQSPI0_MASK;
    }
    DELAY_US(1);
}

#if (defined(BOOT_ROM))
XIP_BANNED uint8_t lsqspi_rd_cap_dly_get(struct lsqspi_instance *inst)
{
    return 0;
}
#else
XIP_BANNED uint8_t lsqspi_rd_cap_dly_get(struct lsqspi_instance *inst)
{
    return 2;
}
#endif

XIP_BANNED uint8_t lsqspi_baudrate_get(struct lsqspi_instance *inst)
{
    return 0;
}

XIP_BANNED uint32_t lsqspi_dly_get(struct lsqspi_instance *inst)
{
    return FIELD_BUILD(LSQSPI_DLY_NSS,4) | FIELD_BUILD(LSQSPI_DLY_AFTER,2) | FIELD_BUILD(LSQSPI_DLY_INIT, 2);
}
