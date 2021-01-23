#include "lsiwdg.h"
#include "reg_lsiwdg_type.h"
#include "reg_base_addr.h"
#include "field_manipulate.h"
#include "cpu.h"
#include "reg_rcc.h"
#define LSIWDG ((reg_iwdg_t *)LSIWDG_BASE_ADDR)

HAL_StatusTypeDef HAL_IWDG_Init(uint32_t LoadValue)
{
    REG_FIELD_WR(RCC->AHBEN, RCC_IWDT, 1);
    LSIWDG->IWDG_LOAD = LoadValue;
    LSIWDG->IWDG_CON = FIELD_BUILD(IWDG_CLKS,2) | FIELD_BUILD(IWDG_RSTEN,1) | FIELD_BUILD(IWDG_IE,0) | FIELD_BUILD(IWDG_EN, 1);
    return HAL_OK;
}

HAL_StatusTypeDef HAL_IWDG_Refresh(void)
{
    enter_critical();
    REG_FIELD_WR(LSIWDG->IWDG_CON,IWDG_EN,0);
    REG_FIELD_WR(LSIWDG->IWDG_CON,IWDG_EN,1);
    exit_critical();
    return HAL_OK;
}