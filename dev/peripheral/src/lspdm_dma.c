#include <stddef.h>
#include "lspdm.h"
#include "lsdmac.h"
#include "field_manipulate.h"
#include "lspdm_msp.h"

static void PDM_DMA_CH0_Callback(void *hdma,uint32_t param)
{
    PDM_HandleTypeDef *hpdm = (PDM_HandleTypeDef *)param;
    hpdm->Env.DMA.Channel_Done[0] = true;
    if(hpdm->Env.DMA.Channel_Done[1])
    {
        hpdm->Env.DMA.Callback();
    }
}

static void PDM_DMA_CH1_Callback(void *hdma,uint32_t param)
{
    PDM_HandleTypeDef *hpdm = (PDM_HandleTypeDef *)param;
    hpdm->Env.DMA.Channel_Done[1] = true;
    if(hpdm->Env.DMA.Channel_Done[0])
    {
        hpdm->Env.DMA.Callback();
    }
}

HAL_StatusTypeDef HAL_PDM_Transfer_Config_DMA(PDM_HandleTypeDef *hpdm,uint16_t *pFrameBuffer0,uint16_t *pFrameBuffer1,uint16_t FrameNum,void (*Callback)())
{
    hpdm->Env.DMA.Callback = Callback;
    struct DMA_Channel_Config prim = {
        .ctrl_data = {
            .cycle_ctrl = DMA_Cycle_Basic,
            .next_useburst = 0,
            .n_minus_1 = FrameNum - 1,
            .R_power = 0,
            .src_prot_ctrl = 0,
            .dst_prot_ctrl = 0,
            .src_size = 1,
            .src_inc = 3,
            .dst_size = 1,
            .dst_inc = 1,
        },
    };
    hpdm->Env.DMA.Channel_Done[0] = false;
    prim.src_data_end_ptr = (uint32_t)&hpdm->Instance->DATA0;
    prim.dst_data_end_ptr = (uint32_t)&pFrameBuffer0[FrameNum-1];
    prim.dummy = (uint32_t)PDM_DMA_CH0_Callback,
    HAL_DMA_Channel_Start_IT(hpdm->DMAC_Instance,hpdm->Env.DMA.Channel[0],HAL_PDM_CH0_Handshake_Get(hpdm),&prim,NULL,(uint32_t)hpdm);
    if(REG_FIELD_RD(hpdm->Instance->CR,PDM_CR_CHN))
    {
        hpdm->Env.DMA.Channel_Done[1] = false;
        prim.src_data_end_ptr = (uint32_t)&hpdm->Instance->DATA1;
        prim.dst_data_end_ptr = (uint32_t)&pFrameBuffer1[FrameNum-1];
        prim.dummy = (uint32_t)PDM_DMA_CH1_Callback,
        HAL_DMA_Channel_Start_IT(hpdm->DMAC_Instance,hpdm->Env.DMA.Channel[1],HAL_PDM_CH1_Handshake_Get(hpdm),&prim,NULL,(uint32_t)hpdm);
    }else
    {
        hpdm->Env.DMA.Channel_Done[1] = true;
    }
    REG_FIELD_WR(hpdm->Instance->CR,PDM_CR_DMAEN,1);
    return HAL_OK;
}
