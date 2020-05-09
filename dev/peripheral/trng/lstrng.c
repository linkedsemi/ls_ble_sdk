#include "le501x.h"
#include "field_manipulate.h"
#include "reg_lstrng.h"
#include "reg_rcc.h"

 static void lstrng_WaitStart(void)
{
    while (REG_FIELD_RD(LSTRNG->SR,TRNG_START) == 0){;}
}

static void lstrng_WaitData(void)
{
    while (REG_FIELD_RD(LSTRNG->SR,TRNG_DAVLD) == 0){;}
}

void lstrng_init(void)
{
    REG_FIELD_WR(RCC->APB1EN, RCC_TRNG,1);
 //   REG_FIELD_WR(RCC->APB1SL, RCC_TRNG,0);
 //   REG_FIELD_WR(RCC->APB1DSL, RCC_TRNG,0);
}

void lstrng_deinit(void)
{
    REG_FIELD_WR(RCC->APB1EN, RCC_TRNG,0);
//    REG_FIELD_WR(RCC->APB1SL, RCC_TRNG,0);
//    REG_FIELD_WR(RCC->APB1DSL, RCC_TRNG,0);
}

 void lstrng_start(void)
{
    REG_FIELD_WR(LSTRNG->CR, TRNG_TRNGEN, 1);
    REG_FIELD_WR(LSTRNG->CR, TRNG_TRNGSEL, 1);
    REG_FIELD_WR(LSTRNG->CR, TRNG_DSEL, 3);
    REG_FIELD_WR(LSTRNG->CR, TRNG_POSTEN, 0);
    REG_FIELD_WR(LSTRNG->CR, TRNG_ADJC, 1);
    REG_FIELD_WR(LSTRNG->CFGR, TRNG_TSTART, 7);
    lstrng_WaitStart();
    lstrng_WaitData();
}

void lstrng_stop(void)
{
    REG_FIELD_WR(LSTRNG->CR, TRNG_TRNGEN, 0);
}

uint32_t lstrng_getdata(void)
{
    uint32_t trng_result = REG_FIELD_RD(LSTRNG->DR, TRNG_DATA);
    return trng_result;
}
