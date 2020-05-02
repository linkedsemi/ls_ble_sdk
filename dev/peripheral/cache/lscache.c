#include "lscache.h"
#include "lscache_param.h"
#include "reg_lscache.h"
#include "field_manipulate.h"
#include "compile_flag.h"
XIP_BANNED void lscache_cache_ctrl(uint8_t prefetch,uint8_t enable)
{
    if(enable)
    {
        lscache_clk_set(1);
    }
    LSCACHE->CCR = FIELD_BUILD(LSCACHE_SET_PREFETCH, prefetch) | FIELD_BUILD(LSCACHE_EN, enable);
    if(!enable)
    {
        lscache_clk_set(0);
    }
}


