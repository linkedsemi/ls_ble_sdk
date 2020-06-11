/*
 * @Author: your name
 * @Date: 2020-05-03 16:00:06
 * @LastEditTime: 2020-05-05 21:19:03
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \ble_sdk_app\dev\examples\sbl\sbl.c
 */
#include "spi_flash.h"
#include "le501x.h"
#include "lscache.h"
#include "compile_flag.h"
#include "reg_rcc.h"
#include "field_manipulate.h"
#include "sdk_config.h"
#include "platform.h"
#include "reg_lsgpio.h"
#include "reg_syscfg.h"
#include "reg_rf.h"
#define APP_IMAGE_BASE (0x18008000)

XIP_BANNED void swd_pull_down()
{
    MODIFY_REG(LSGPIOB->PUPD,GPIO_PUPD5_MASK|GPIO_PUPD6_MASK,2<<GPIO_PUPD5_POS | 2<<GPIO_PUPD6_POS);
}


uint16_t trim_head_load()
{
    uint16_t head[8];
    uint16_t version;
    spi_flash_read_security_area(1, 0,(void *)head, sizeof(head));
    if(head[0]==~head[1] && head[2]==~head[3])
    {
        version = head[0];
        return version;
    }else
    {
        return 0;
    }
    
}

void trim_version_4100_load()
{
    uint16_t buf[10];
    spi_flash_read_security_area(1, 0x10,(void *)buf, sizeof(buf));
    uint8_t i;
    for(i=0;i<10;i+=2)
    {
        if(buf[i]!=~buf[i+1])
        {
            while(1);
        }
    }
    struct {
        uint16_t bg_vref_fine:2,
                bg_reg_trim:6,
                dcdc_vbg_vctl:4,
                lvd_ref:3;
    } *cal_0 = (void *)&buf[0];
    struct {
        uint16_t hpldo_trim:2,
                res:6,
                ldo_tx_trim:3,
                res2:1,
                ldo_rx_trim:3;
    } *cal_1 = (void *)&buf[2];
    struct {
        uint16_t xo16m_cap_trim:6,
                xo16m_adj:2;
    } *cal_2 = (void *)&buf[4];
    struct {
        uint16_t osc_rc24m_cal:15;
    } *cal_3 = (void *)&buf[6];
    struct {
        uint16_t lpldo_trim:3;
    } *cal_4 = (void *)&buf[8];
    cal_2->xo16m_cap_trim = 0x20;
    cal_4->lpldo_trim = 5;
    REG_FIELD_WR(SYSCFG->DCDC, SYSCFG_VBG_VCTL, cal_0->dcdc_vbg_vctl);
    MODIFY_REG(SYSCFG->ANACFG0, SYSCFG_BG_RES_TRIM_MASK | SYSCFG_BG_VREF_FINE_MASK | SYSCFG_LDO_DG_TRIM_MASK | SYSCFG_LVD_REF_MASK
        , cal_0->bg_reg_trim<<SYSCFG_BG_RES_TRIM_POS | cal_0->bg_vref_fine<<SYSCFG_BG_VREF_FINE_POS 
        | cal_1->hpldo_trim << SYSCFG_LDO_DG_TRIM_POS | cal_0->lvd_ref << SYSCFG_LVD_REF_POS);
    MODIFY_REG(SYSCFG->ANACFG1, SYSCFG_XO16M_ADJ_MASK | SYSCFG_XO16M_CAP_TRIM_MASK, 
        cal_2->xo16m_adj << SYSCFG_XO16M_ADJ_POS | cal_2->xo16m_cap_trim << SYSCFG_XO16M_CAP_TRIM_POS);
    MODIFY_REG(SYSCFG->CFG, SYSCFG_HAI_SEL_MASK | SYSCFG_HAI_IBIAS_SEL_MASK | SYSCFG_HAI_CAL_MASK | SYSCFG_HAI_CAP_MASK,
        cal_3->osc_rc24m_cal);
    REG_FIELD_WR(SYSCFG->PMU_TRIM,SYSCFG_LDO_LP_TRIM,cal_4->lpldo_trim);

    MODIFY_REG(RF->REG08,RF_LDO_TX_TRIM_MASK | RF_LDO_RX_TRIM_MASK,
        cal_1->ldo_tx_trim<<RF_LDO_TX_TRIM_POS | cal_1->ldo_rx_trim<<RF_LDO_RX_TRIM_POS);
}

void trim_val_load()
{
    RCC->APB1EN |= 1<<RCC_RF_POS | 1<<RCC_MDM2_POS;
    uint16_t version = trim_head_load();
    switch(version)
    {
    case 0x4100:
        trim_version_4100_load();
    break;
    default:
        REG_FIELD_WR(SYSCFG->ANACFG1, SYSCFG_XO16M_CAP_TRIM,0x20);
        REG_FIELD_WR(SYSCFG->PMU_TRIM,SYSCFG_LDO_LP_TRIM,5);
        MODIFY_REG(RF->REG08,RF_LDO_TX_TRIM_MASK | RF_LDO_RX_TRIM_MASK,
            4<<RF_LDO_TX_TRIM_POS | 4<<RF_LDO_RX_TRIM_POS);
    break;
    }
}


XIP_BANNED int main()
{
    switch_to_rc32k();
    clk_switch();
    __disable_irq();
    swd_pull_down();
    spi_flash_drv_var_init(false,false);
    spi_flash_init();
    spi_flash_qe_status_read_and_set();
    spi_flash_xip_start();
    lscache_cache_ctrl(0,1);
    trim_val_load();
    uint32_t *msp = (void *)APP_IMAGE_BASE;
    void (**reset_handler)(void) = (void *)(APP_IMAGE_BASE + 4);
    __set_MSP(*msp);
    (*reset_handler)();
    return 0;
}
