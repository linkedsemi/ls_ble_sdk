#ifndef LSPDM_H_
#define LSPDM_H_
#include <stdbool.h>
#include "reg_pdm_type.h"
#include "reg_base_addr.h"
#include "HAL_def.h"

/* The basic address of the PDM peripheral.                                   */
#define LSPDM ((reg_pdm_t *)LSPDM_BASE_ADDR)

/* Data configuration of PDM filter controller is derived from lspdm.c        */
/* Assign fir_coef_8khz[64] or fir_coef_16khz[64] value to the PDM COEFX register       */
extern const uint32_t fir_coef_8khz[64];
extern const uint32_t fir_coef_16khz[64];
#define PDM_FIR_COEF_8KHZ ((const struct pdm_fir *)fir_coef_8khz)
#define PDM_FIR_COEF_16KHZ ((const struct pdm_fir *)fir_coef_16khz)

/**
  * @brief Calculate the clock rate of the PDM and assign this value to the PDMCLK_RATIO bit of the PDM_CFG  register
  * @param kHz: PDM clock rate set value    Unit: khz
  * @note eg: if pdm clock is 1.024MHz, configure khz is 1024, the PDMCLK_RATIO bit value is (128000/1024 - 1)
  */
#define PDM_CLK_RATIO(kHz) (128000/(kHz) - 1)

/**
  * @brief Calculates the sample rate of the PDM and assigns this value to the SAMPLE_RATE bit of the PDM_CFG register
  * @param Clk_kHz: PDM clock rate set value    Unit: khz
  * @param Sample_Rate_Hz: PDM sample rate set value    Unit: hz
  * @note eg: if pdm sample rare is 16KHz, configure Clk_kHz is 1024, configure Sample_Rate_Hz is 16000, the SAMPLE_RATE bit value is (1024*1000/16000 - 1)
  */
#define PDM_SAMPLE_RATE(Clk_kHz,Sample_Rate_Hz) ((Clk_kHz)*1000/(Sample_Rate_Hz) - 1)

/**
  * @enum PDM_MODE_TypeDef
  * @brief This enumerated variable type is used in PDM initialization and writes the corresponding variable value to the PDM_CHN bit of the PDM_CR register
  */
typedef enum __PDM_MODE_TypeDef
{
    PDM_MODE_Mono = 0,    /*!< "0" means "use single channel". Collect voice data only on the rising or falling edge  */
    PDM_MODE_Stereo = 1   /*!< "1" means "use stereo channel". Collect voice data on the rising and falling edge */
} PDM_MODE_TypeDef;

/**
  * @struct PDM_CFG_TypeDef
  * @brief This structure variable type is used in PDM initialization and writes the corresponding variable value to the PDM_CFG registe
  */
typedef struct __PDM_CFG_TypeDef
{
    uint8_t clk_ratio;      /*!< pdm clock rate configure. Calculated by PDM_CLK_RATIO macro definition */
    uint8_t cap_delay;      /*!< pdm capture delay configure.  data capture after this value cycles of 128Mhz */
    uint8_t sample_rate;    /*!< pdm sampling rate configure. Calculated by PDM_SAMPLE_RATE macro definition */
    uint8_t data_gain:4;    /*!< configure the gain adjustment */
} PDM_CFG_TypeDef;

/**
  * @struct PDM_Init_TypeDef
  * @brief Configure the variable types used for full PDM initialization and write the corresponding values to the corresponding registers
  * @note  This structure variable types is the complete PDM initialization configuration. 
  *         The parameters to configure are as follows:
  *         1.Configure the pdm filter controller. This value is obtained by fir_coef_8khz[64] or fir_coef_16khz[64]
  *         2.Configure the value of the PDM_CFG register, obtained through the variable cfg of type PDM_CFG_TYPEDEF.
  *         3.Configure the PDM channel mode and write this value to the PDM_CHN bit of the PDM_CR register
  */
typedef struct __PDM_Init_TypeDef
{
    const struct pdm_fir *fir;  /*!< pdm filter controller configure */
    PDM_CFG_TypeDef cfg;        /*!< pdm clock rate, capture delay, sampling rate, and data gain configure */
    PDM_MODE_TypeDef mode;      /*!< pdm channel mode configure: mono or stereo  */
} PDM_Init_TypeDef;

/**
  * @struct PDM_DMA_Env
  * @brief This structure variable type is a configuration that uses DMA to move PDM data
  */
struct PDM_DMA_Env
{
    uint32_t PingPong_Ctrl_Data;    /*!< configure the data control register configuration for DMA ping-pong mode, this value is written to a variable of type ctrl_data_config */
    uint8_t Channel[2];             /*!< configure DMA channel number, 1 DMA channel for mono channel, 2 DMA channels for stereo channel */
    bool Channel_Done[2];           /*!< DMA channel configuration complete flag */
};

/**
  * @struct PDM_Interrupt_Env
  * @brief This structure is an event configuration that accepts PDM data through PDM interrupts
  */
struct PDM_Interrupt_Env
{
    uint16_t *pFrameBuffer[2];  /*!< configure an pointer array to accept PDM data */
    uint16_t FrameNum;          /*!< configure the size to accept PDM data */
};

/**
  * @struct PDM_PingPong_Bufptr
  * @brief This structure is an pointer array configured to accept PDM data in DMA Ping-Pong mode
  * @note Accepting PDM data streams in DMA ping-pong mode requires array Pointers to be exchanged for acceptance
  */
struct PDM_PingPong_Bufptr
{
    uint16_t *Bufptr[2];    /*!< configure pointer array to accept PDM data */
};

/**
  * @struct PDM_HandleTypeDef
  * @brief PDM handle variable type definition,any operation on the PDM needs to use the contents contained in this structure variable.
  * @note This handle is used as a parameter when using the PDM function, and can be used to operate on the PDM register
  */
typedef struct __PDM_HandleTypeDef
{
    reg_pdm_t *Instance;                        /*!< pdm registers base address */
    void *DMAC_Instance;                        /*!< select the initialized DMA object */
    union{
        struct PDM_DMA_Env DMA;
        struct PDM_Interrupt_Env Interrupt;
    }Env;                                       /*!< The PDM chooses the way to move data: DMA or interrupt mode  */
} PDM_HandleTypeDef;

/* operation functions *******************************************************/
/**
  * @brief  The PDM is initialized with the parameters specified in PDM_Init_TypeDef and an associated handle is created with the parameters specified in PDM_HandleTypeDef.
  * @param  hpdm  Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle.       
  * @param  Init  Pointer to the PDM_Init_TypeDef structure that contains all the information for the specified PDM initialization configuration.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PDM_Init(PDM_HandleTypeDef *hpdm,PDM_Init_TypeDef *Init);

/**
  * @brief  DeInitializes the PDM peripheral.
  * @param  hpdm  Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle. 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PDM_DeInit(PDM_HandleTypeDef *hpdm);

/**
  * @brief  Configure the PDM interrupt mode parameters and call the PDM interrupt handler after receiving One frame of PDM data
  *                 PDM uses mono mode to sample  mode: use pFrameBuffer0 or pFrameBuffer1 to accept PDM data
  *                 PDM uses stereo mode to sample mode: use pFrameBuffer0 and pFrameBuffer1 to accept PDM data.
  * @param  hpdm    Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle.       
  * @param  pFrameBuffer0   The BUF address to accept PDM data.
  * @param  pFrameBuffer1   The BUF address that accepts PDM data when using stereo channels. NULL when using mono channels.
  * @param  FrameNum      Accept the size of PDM data  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PDM_Transfer_Config_IT(PDM_HandleTypeDef *hpdm,uint16_t *pFrameBuffer0,uint16_t *pFrameBuffer1,uint16_t FrameNum);

/**
  * @brief  The PDM data callback function of Framenum size is received in the interrupt handler function.
  * @param  hpdm  Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle. 
  * @retval HAL status
  */
void HAL_PDM_CpltCallback(PDM_HandleTypeDef *hpdm);

/**
  * @brief  Configure the parameters of the DMA and accept PDM data in DMA mode, and call the HAL_PDM_DMA_CpltCallback function after the DMA has moved the FrameNum size data
  *                 PDM uses mono mode to sample  mode: use pFrameBuffer0 or pFrameBuffer1 to accept PDM data
  *                 PDM uses stereo mode to sample mode: use pFrameBuffer0 and pFrameBuffer1 to accept PDM data.
  * @param  hpdm    Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle.       
  * @param  pFrameBuffer0   The BUF address to accept PDM data.
  * @param  pFrameBuffer1   The BUF address that accepts PDM data when using stereo channels. NULL when using mono channels.
  * @param  FrameNum      Accept the size of PDM data  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PDM_Transfer_Config_DMA(PDM_HandleTypeDef *hpdm,uint16_t *pFrameBuffer0,uint16_t *pFrameBuffer1,uint16_t FrameNum);

/**
  * @brief  Configure the parameters of DMA Ping-Pang mode and accept PDM data in DMA Ping-Pang mode, and call the HAL_PDM_DMA_CpltCallback Callback function after DMA has moved the FrameNum size data
  *                 PDM uses mono mode to sample data: use CH0_Buf or CH1_Buf to accept PDM data
  *                 PDM uses stereo mode to sample data: use CH0_Buf and CH1_Buf to accept PDM data.
  * @param  hpdm    Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle.       
  * @param  CH0_Buf   An array of Pointers to the PDM_PingPong_Bufptr structure that contains two user-defined buf addresses to alternately receive PDM data streams.
  * @param  CH1_Buf     An array of Pointers to the PDM_PingPong_Bufptr structure that contains two user-defined buf addresses to alternately receive PDM data streams.
  * @param  FrameNum    Accept the size of PDM data  
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PDM_PingPong_Transfer_Config_DMA(PDM_HandleTypeDef *hpdm,struct PDM_PingPong_Bufptr *CH0_Buf,struct PDM_PingPong_Bufptr *CH1_Buf,uint16_t FrameNum);

/**
  * @brief  The PDM data callback function of Framenum size is received in the DMA mode.
  * @param  hpdm  Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle. 
  * @retval HAL status
  */
void HAL_PDM_DMA_CpltCallback(PDM_HandleTypeDef *hpdm,uint8_t buf_idx);

/**
  * @brief  The PDM EN bit is enabled, and the PDM peripheral start working.
  * @param  hpdm  Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle. 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PDM_Start(PDM_HandleTypeDef *hpdm);

/**
  * @brief  The PDM EN bit is disabled, and the PDM peripheral stop working.
  * @param  hpdm  Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle. 
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_PDM_Stop(PDM_HandleTypeDef *hpdm);

/**
  * @brief  PDM interrupt handler function.
  * @param  hpdm  Pointer to the PDM_HandleTypeDef structure, which contains configuration information for the specified PDM handle. 
  * @retval none
  */
void HAL_PDM_IRQHandler(PDM_HandleTypeDef *hpdm);

#endif
