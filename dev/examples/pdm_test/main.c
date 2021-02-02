#include "platform.h"
#include "lspdm.h"
#include "lsdmac.h"
#include "log.h"
#include "io_config.h"
#define PDM_CLK_KHZ 1024
#define PDM_SAMPLE_RATE_HZ 16000
#define FRAME_BUF_SIZE 256
DEF_DMA_CONTROLLER(dmac1_inst,DMAC1);
PDM_HandleTypeDef pdm;
DMA_RAM_ATTR uint16_t Buf0[FRAME_BUF_SIZE];
DMA_RAM_ATTR uint16_t Buf1[FRAME_BUF_SIZE];

void HAL_PDM_CpltCallback(PDM_HandleTypeDef *hpdm)
{
    LOG_I("%s",__FUNCTION__);
    HAL_PDM_Transfer_Config_IT(&pdm,Buf0,Buf1,FRAME_BUF_SIZE);
}

void pdm_interrupt_test()
{
    HAL_PDM_Transfer_Config_IT(&pdm,Buf0,Buf1,FRAME_BUF_SIZE);
}

static void pdm_dma_callback(uint8_t idx)
{
    LOG_I("%s",__FUNCTION__);
    HAL_PDM_Transfer_Config_DMA(&pdm,Buf0,Buf1,FRAME_BUF_SIZE,pdm_dma_callback);
}

void pdm_dma_test()
{
    DMA_CONTROLLER_INIT(dmac1_inst);
    pdm.DMAC_Instance = &dmac1_inst;
    pdm.Env.DMA.Channel[0] = 2;
    pdm.Env.DMA.Channel[1] = 3;
    HAL_PDM_Transfer_Config_DMA(&pdm,Buf0,Buf1,FRAME_BUF_SIZE,pdm_dma_callback);
    HAL_PDM_Start(&pdm);
}

void pdm_init()
{
    pdm_clk_io_init(PB09);
    pdm_data0_io_init(PB08);
    pdm.Instance = LSPDM;
    PDM_Init_TypeDef Init = 
    {
        .fir = PDM_FIR_COEF_16KHZ,
        .cfg = {
            .clk_ratio = PDM_CLK_RATIO(PDM_CLK_KHZ),
            .sample_rate = PDM_SAMPLE_RATE(PDM_CLK_KHZ,PDM_SAMPLE_RATE_HZ),
            .cap_delay = 30,
            .data_gain = 5,
        },
        .mode = PDM_MODE_Mono,
    };
    HAL_PDM_Init(&pdm,&Init);
}

int main()
{
    sys_init_app();
    pdm_init();
    pdm_interrupt_test();
//    pdm_dma_test();
    while(1);
}