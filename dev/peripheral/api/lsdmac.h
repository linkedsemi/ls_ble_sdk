#ifndef LSDMAC_H_
#define LSDMAC_H_
#include <stdbool.h>
#include "reg_dmac_type.h"
#include "reg_base_addr.h"
#include "dmac_config.h"

#ifdef DMAC1_BASE_ADDR
#define DMAC1 (reg_dmac_t *)DMAC1_BASE_ADDR
#endif
#define DMA_RAM_ATTR __attribute__((section("DMARAM")))
typedef struct __DMA_Controller_HandleTypeDef
{
    reg_dmac_t *Instance;
    uint32_t *channel_param;
}DMA_Controller_HandleTypeDef;

struct ctrl_data_config
{
    uint32_t cycle_ctrl:3,
            next_useburst:1,
            n_minus_1:10,
            R_power:4,
            src_prot_ctrl:3,
            dst_prot_ctrl:3,
            src_size:2,
            src_inc:2,
            dst_size:2,
            dst_inc:2;
};

struct DMA_Channel_Config
{
    uint32_t src_data_end_ptr;
    uint32_t dst_data_end_ptr;
    struct ctrl_data_config ctrl_data;
    uint32_t dummy;
};

enum DMA_Cycle_Type {
    DMA_Cycle_Stop,
    DMA_Cycle_Basic,
    DMA_Cycle_Auto,
    DMA_Cycle_PingPong,
    DMA_Cycle_Mem_Scatter_Gather_Prim,
    DMA_Cycle_Mem_Scatter_Gather_Alt,
    DMA_Cycle_Periph_Scatter_Gather_Prim,
    DMA_Cycle_Periph_Scatter_Gather_Alt,
};

#define DEF_DMA_CONTROLLER(dmac_inst,dmac_reg) \
    __attribute__((section(#dmac_reg),aligned(dmac_reg##_MAX_CHANNEL_NUM * 32))) struct dmac_inst##_cs_env {\
        struct DMA_Channel_Config prim[dmac_reg##_MAX_CHANNEL_NUM];\
        struct DMA_Channel_Config alt[dmac_reg##_MAX_CHANNEL_NUM];\
    } dmac_inst##_cs;\
    static uint32_t dmac_inst##_ch_param[dmac_reg##_MAX_CHANNEL_NUM]; \
    DMA_Controller_HandleTypeDef dmac_inst = { \
        .Instance = dmac_reg,\
        .channel_param = dmac_inst##_ch_param, \
    };

#define DMA_CONTROLLER_INIT(dmac_inst) HAL_DMA_Controller_Init(&dmac_inst,&dmac_inst##_cs)
#define DMA_CONTROLLER_DEINIT(dmac_inst) HAL_DMA_Controller_DeInit(&dmac_inst)

void HAL_DMA_Controller_Init(DMA_Controller_HandleTypeDef *hdma,void *cs_ptr);

void HAL_DMA_Controller_DeInit(DMA_Controller_HandleTypeDef *hdma);

void HAL_DMA_Channel_Start_IT(DMA_Controller_HandleTypeDef *hdma,uint8_t ch_idx,uint8_t handshake,uint32_t param);

void HAL_DMA_Channel_Config_Set(DMA_Controller_HandleTypeDef *hdma,uint8_t ch_idx,bool alt,struct DMA_Channel_Config *cfg);

void HAL_DMA_Channel_Config_Get(DMA_Controller_HandleTypeDef *hdma,uint8_t ch_idx,bool alt,struct DMA_Channel_Config *cfg);

void HAL_DMA_Channel_Abort(DMA_Controller_HandleTypeDef *hdma,uint8_t ch_idx);

void HAL_DMA_Controller_IRQHandler(DMA_Controller_HandleTypeDef *hdma);


#endif
