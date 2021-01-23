#include "lsdmac.h"
#include "dmac_msp.h"
#include "field_manipulate.h"
#include "cpu.h"

static void DMA_Handshake_Config(DMA_Controller_HandleTypeDef *hdma,uint8_t ch_idx,uint8_t handshake)
{
    MODIFY_REG(hdma->Instance->CHSEL[ch_idx/4],0xff<< 8 * (ch_idx % 4),handshake<< 8 * (ch_idx%4));
}

void HAL_DMA_Controller_Init(DMA_Controller_HandleTypeDef *hdma,void *cs_ptr)
{
    HAL_DMA_Controller_MSP_Init(hdma);
    HAL_DMA_Controller_MSP_Busy_Set(hdma);
    hdma->Instance->CFG = 1;
    hdma->Instance->BPTR = (uint32_t)cs_ptr;
}

void HAL_DMA_Controller_DeInit(DMA_Controller_HandleTypeDef *hdma)
{
    hdma->Instance->CFG = 0;
    HAL_DMA_Controller_MSP_Idle_Set(hdma);
    HAL_DMA_Controller_MSP_DeInit(hdma);
}

void HAL_DMA_Channel_Start_IT(DMA_Controller_HandleTypeDef *hdma,uint8_t ch_idx,uint8_t handshake,struct DMA_Channel_Config *prim,struct DMA_Channel_Config *alt,uint32_t param)
{
    hdma->channel_param[ch_idx] = param;
    DMA_Handshake_Config(hdma,ch_idx,handshake);
    struct DMA_Channel_Config *bptr = (struct DMA_Channel_Config *)hdma->Instance->BPTR;
    if(prim) bptr[ch_idx] = *prim;
    struct DMA_Channel_Config *altbptr = (struct DMA_Channel_Config *)hdma->Instance->ALTBPTR;
    if(alt) altbptr[ch_idx] = *alt;
    hdma->Instance->DONEICF = 1<<ch_idx;
    enter_critical();
    hdma->Instance->DONEIEF |= 1<<ch_idx;
    exit_critical();
    hdma->Instance->ENSET = 1<<ch_idx;
}

void HAL_DMA_Controller_IRQHandler(DMA_Controller_HandleTypeDef *hdma)
{
    uint8_t i;
    uint32_t irq = hdma->Instance->DONERIF & hdma->Instance->DONEIEF;
    uint8_t dma_ch_num = hdma->Instance->SR >> 16 & 0x1f;
    for(i=0;i<dma_ch_num;++i)
    {
        if(irq&1<<i)
        {
            enter_critical();
            hdma->Instance->DONEIEF &= ~(1<<i);
            exit_critical();
            struct DMA_Channel_Config *ptr;
            if(hdma->Instance->PRIALTSET)
            {
                ptr = (struct DMA_Channel_Config *)hdma->Instance->ALTBPTR;
            }else
            {
                ptr = (struct DMA_Channel_Config *)hdma->Instance->BPTR;
            }
            void (*cb)(DMA_Controller_HandleTypeDef *,uint32_t) = (void (*)(DMA_Controller_HandleTypeDef *,uint32_t))ptr[i].dummy;
            cb(hdma,hdma->channel_param[i]);
        }
    }
}
