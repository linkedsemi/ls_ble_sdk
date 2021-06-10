#include "lsqspiv2_msp.h"
#include "reg_sysc_cpu_type.h"
void lsqspiv2_msp_init()
{
    SYSC_CPU->PD_CPU_CLKG = SYSC_CPU_CLKG_SET_QSPI_MASK;
}