#ifndef LSPDM_H_
#define LSPDM_H_
#include <stdbool.h>
#include "reg_pdm_type.h"
#include "reg_base_addr.h"
#include "HAL_def.h"

#define LSPDM ((reg_pdm_t *)LSPDM_BASE_ADDR)

extern const uint32_t fir_coef_8khz[64];
extern const uint32_t fir_coef_16khz[64];

#define PDM_FIR_COEF_8KHZ ((const struct pdm_fir *)fir_coef_8khz)
#define PDM_FIR_COEF_16KHZ ((const struct pdm_fir *)fir_coef_16khz)
#define PDM_CLK_RATIO(kHz) (128000/(kHz) - 1)
#define PDM_SAMPLE_RATE(Clk_kHz,Sample_Rate_Hz) ((Clk_kHz)*1000/(Sample_Rate_Hz) - 1)

typedef enum __PDM_MODE_TypeDef
{
    PDM_MODE_Mono = 0,    /* use single channel */
    PDM_MODE_Stereo = 1     /* use two channel */
} PDM_MODE_TypeDef;

typedef struct __PDM_CFG_TypeDef
{
    uint8_t clk_ratio;
    uint8_t cap_delay;
    uint8_t sample_rate;
    uint8_t data_gain:4;
} PDM_CFG_TypeDef;

typedef struct __PDM_Init_TypeDef
{
    const struct pdm_fir *fir;
    PDM_CFG_TypeDef cfg;
    PDM_MODE_TypeDef mode;              // mono or stereo
} PDM_Init_TypeDef;

struct PDM_DMA_Env
{
    uint32_t PingPong_Ctrl_Data;
    uint8_t Channel[2];
    bool Channel_Done[2];
};

struct PDM_Interrupt_Env
{
    uint16_t *pFrameBuffer[2];
    uint16_t FrameNum;
};

struct PDM_PingPong_Bufptr
{
    uint16_t *Bufptr[2];
};

typedef struct __PDM_HandleTypeDef
{
    reg_pdm_t *Instance;
    void *DMAC_Instance;
    union{
        struct PDM_DMA_Env DMA;
        struct PDM_Interrupt_Env Interrupt;
    }Env;
} PDM_HandleTypeDef;

HAL_StatusTypeDef HAL_PDM_Init(PDM_HandleTypeDef *hpdm,PDM_Init_TypeDef *Init);

HAL_StatusTypeDef HAL_PDM_DeInit(PDM_HandleTypeDef *hpdm);

HAL_StatusTypeDef HAL_PDM_Transfer_Config_IT(PDM_HandleTypeDef *hpdm,uint16_t *pFrameBuffer0,uint16_t *pFrameBuffer1,uint16_t FrameNum);

void HAL_PDM_CpltCallback(PDM_HandleTypeDef *hpdm);

HAL_StatusTypeDef HAL_PDM_Transfer_Config_DMA(PDM_HandleTypeDef *hpdm,uint16_t *pFrameBuffer0,uint16_t *pFrameBuffer1,uint16_t FrameNum);

HAL_StatusTypeDef HAL_PDM_PingPong_Transfer_Config_DMA(PDM_HandleTypeDef *hpdm,struct PDM_PingPong_Bufptr *CH0_Buf,struct PDM_PingPong_Bufptr *CH1_Buf,uint16_t FrameNum);

void HAL_PDM_DMA_CpltCallback(PDM_HandleTypeDef *hpdm,uint8_t buf_idx);

HAL_StatusTypeDef HAL_PDM_Start(PDM_HandleTypeDef *hpdm);

HAL_StatusTypeDef HAL_PDM_Stop(PDM_HandleTypeDef *hpdm);

void HAL_PDM_IRQHandler(PDM_HandleTypeDef *hpdm);

#endif
