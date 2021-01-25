
#include <stdio.h>
#include "lsuart.h"
#include "uart_msp.h" 
#include "field_manipulate.h"
#include "log.h"
#include "systick.h"

static void UART_SetConfig(UART_HandleTypeDef *huart);

/**
  * @brief  Initializes the UART mode according to the specified parameters in
  *         the UART_InitTypeDef and create the associated handle.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart)
{
    /* Check the UART handle allocation */
    if (huart == NULL)
    {
        return HAL_INVALIAD_PARAM;
    }

    /* Check the parameters */
    //TODO
    HAL_UART_MSP_Init(huart);
    HAL_UART_MSP_Busy_Set(huart);
    /* Set the UART Communication parameters */
    UART_SetConfig(huart);

    /* Initialize the UART state */
    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->gState = HAL_UART_STATE_READY;
    huart->RxState = HAL_UART_STATE_READY;

    return HAL_OK;
}

//HAL_StatusTypeDef HAL_HalfDuplex_Init(UART_HandleTypeDef *huart)
//HAL_StatusTypeDef HAL_LIN_Init(UART_HandleTypeDef *huart, uint32_t BreakDetectLength)
//HAL_StatusTypeDef HAL_Smartcard_Init(UART_HandleTypeDef *huart, uint8_t Address, uint32_t WakeUpMethod)
//TODO
/**
  * @brief  DeInitializes the UART peripheral.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart)
{
    /* Check the UART handle allocation */
    if (huart == NULL)
    {
        return HAL_INVALIAD_PARAM;
    }

    /* Check the parameters */
    //  assert_param(IS_UART_INSTANCE(huart->UARTX));
    HAL_UART_MSP_DeInit(huart);
    HAL_UART_MSP_Idle_Set(huart);

    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->gState = HAL_UART_STATE_RESET;
    huart->RxState = HAL_UART_STATE_RESET;
    return HAL_OK;
}

static bool uart_flag_poll(va_list va)
{
      UART_HandleTypeDef *huart = va_arg(va,UART_HandleTypeDef *);
      uint32_t flag = va_arg(va,uint32_t);
      if (huart->UARTX->SR & flag)
      {
        return true;
      }
      else
      {
        return false;
      }
}

/**
  * @brief  Sends an amount of data in blocking mode.
  * @note   
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (uint8_t data elements).
  * @param  Size  Amount of data elements (uint8_t) to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t timeout = Timeout * SDK_PCLK_MHZ * 1000;
    uint32_t tickstart = systick_get_value();
    /* Check that a Tx process is not already ongoing */
    if (huart->gState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return  HAL_INVALIAD_PARAM;
        }

        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->gState = HAL_UART_STATE_BUSY_TX;
        /* Init tickstart for timeout managment */
        //TODO
        huart->Tx_Env.Interrupt.XferCount = Size;
        
        while (huart->Tx_Env.Interrupt.XferCount > 0U )
        {
            if(systick_poll_timeout(tickstart,timeout,uart_flag_poll,huart,UART_SR_TFNF))
            {
                huart->gState = HAL_UART_STATE_READY;
                return HAL_TIMEOUT;
            }
           else
            {
                //Transmit FIFO Not Full
                huart->Tx_Env.Interrupt.XferCount--;
                huart->UARTX->TBR = (*pData++ & (uint8_t)0xFF);
                if (huart->Tx_Env.Interrupt.XferCount == 0)
                {
                    break;
                }
            }
        }
        // Wait until TX Finish
        if(systick_poll_timeout(tickstart,timeout,uart_flag_poll,huart,UART_SR_TEMT))
        {
            huart->gState = HAL_UART_STATE_READY;
            return HAL_TIMEOUT;
        }
        /* At end of Tx process, restore huart->gState to Ready */
        huart->gState = HAL_UART_STATE_READY;
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Receives an amount of data in blocking mode.
  * @note   
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (uint8_t data elements).
  * @param  Size  Amount of data elements (uint8_t) to be received.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{

    uint32_t timeout = Timeout * SDK_PCLK_MHZ * 1000;
    uint32_t tickstart = systick_get_value();

    /* Check that a Rx process is not already ongoing */
    if (huart->RxState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return  HAL_INVALIAD_PARAM;
        }
        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->RxState = HAL_UART_STATE_BUSY_RX;

        /* Init tickstart for timeout managment */
        //todo
    
        huart->Rx_Env.Interrupt.XferCount = Size;

        /* Check the remain data to be received */
        while (huart->Rx_Env.Interrupt.XferCount > 0U)
        {
            if(systick_poll_timeout(tickstart,timeout,uart_flag_poll,huart,UART_SR_RFNE))
            {
                huart->RxState = HAL_UART_STATE_READY;
                return HAL_TIMEOUT;
            }
           else
            {
                huart->Rx_Env.Interrupt.XferCount--;
                *pData++ = (uint8_t)(huart->UARTX->RBR & (uint8_t)0x00FF);
            }
        }
        /* At end of Rx process, restore huart->RxState to Ready */
        huart->RxState = HAL_UART_STATE_READY;
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Sends an amount of data in non blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the sent data is handled as a set of uint16_t. In this case, Size must indicate the number
  *         of u16 provided through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.int_t
  * @param  pData Pointer to data buffer (uint8_t  data elements).
  * @param  Size  Amount of data elements (uint8_t) to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)

{
    /* Check that a Tx process is not already ongoing */
    if (huart->gState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return HAL_INVALIAD_PARAM;
        }
        REG_FIELD_WR(huart->UARTX->MCR,UART_MCR_DMAEN,0);
        huart->Tx_Env.Interrupt.pBuffPtr = pData;
        huart->Tx_Env.Interrupt.XferCount = Size;
        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->gState = HAL_UART_STATE_BUSY_TX;
        REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_TXTL,UART_FIFO_TL_0);
    /* Enable the UART Transmit data register empty Interrupt */
        huart->UARTX->ICR = UART_IT_TC| UART_IT_TXS;
        huart->UARTX->IER = UART_IT_TXS;
        huart->Tx_Env.Interrupt.XferCount--; 
        huart->UARTX->TBR = (*huart->Tx_Env.Interrupt.pBuffPtr++ & (uint8_t)0xFF);
        if(huart->Tx_Env.Interrupt.XferCount == 0)
        {
            huart->UARTX->IER = UART_IT_TC;
        }
        return HAL_OK;
    }
    else
    {
        LOG_I("UART_Transmit_busy");
        return HAL_BUSY;
    }
}

/**
  * @brief  Receives an amount of data in non blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the received data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 available through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (uint8_t data elements).
  * @param  Size  Amount of data elements (uint8_t) to be received.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
    /* Check that a Rx process is not already ongoing */
    if (huart->RxState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return HAL_INVALIAD_PARAM;
        }
        REG_FIELD_WR(huart->UARTX->MCR,UART_MCR_DMAEN,0);
        huart->Rx_Env.Interrupt.pBuffPtr = pData;
        huart->Rx_Env.Interrupt.XferCount = Size;
        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->RxState = HAL_UART_STATE_BUSY_RX;
        if(huart->Rx_Env.Interrupt.XferCount<8)
        {
            REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_1);
        }
        else
        {
            REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_8);
        } 
        /* Enable the UART Data Register not empty Interrupt */
        huart->UARTX->IER = UART_IT_RXRD;
        return HAL_OK;
    }
    else
    {
        return HAL_BUSY;
    }
}

/**
  * @brief  Configures the UART peripheral.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
static void UART_SetConfig(UART_HandleTypeDef *huart)
{
    /* Check the parameters */
    //TODO
    /* Configure the UART Stop Bits: Set STOP[13:12] bits
        Configure the UART Word Length, Parity and mode:
        Set the M bits according to huart->Init.WordLength value
        Set PCE and PS bits according to huart->Init.Parity value
        Set TE and RE bits according to huart->Init.Mode value
        Set OVER8 bit according to huart->Init.OverSampling value*/
    REG_FIELD_WR(huart->UARTX->LCR,UART_LCR_BRWEN,1);
    huart->UARTX->BRR  =  huart->Init.BaudRate;
    REG_FIELD_WR(huart->UARTX->LCR,UART_LCR_BRWEN,0);
    huart->UARTX->FCR = UART_FCR_TFRST_MASK | UART_FCR_RFRST_MASK | UART_FCR_FIFOEN_MASK;
    huart->UARTX->LCR = FIELD_BUILD(UART_LCR_DLS,huart->Init.WordLength)|FIELD_BUILD(UART_LCR_STOP,huart->Init.StopBits)
                                  |FIELD_BUILD(UART_LCR_PS,huart->Init.Parity)|FIELD_BUILD(UART_LCR_MSB,huart->Init.MSBEN)
                                  |FIELD_BUILD(UART_LCR_RXEN,1)|FIELD_BUILD(UART_LCR_BRWEN,0);
}



__attribute__((weak)) void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__attribute__((weak)) void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
}

/**
  * @brief  Sends an amount of data in non blocking mode.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static void UART_Transmit_IT(UART_HandleTypeDef *huart)
{
    /* Check that a Tx process is ongoing */
    if (huart->gState == HAL_UART_STATE_BUSY_TX)
    {
        if(!REG_FIELD_RD(huart->UARTX->FCR, UART_FCR_TXFL))
        {
            if (huart->Tx_Env.Interrupt.XferCount == 0U)
            {
                huart->UARTX->IDR = UART_IT_TXS;
                huart->UARTX->IER = UART_IT_TC;
            }
            else
            {
                while(REG_FIELD_RD(huart->UARTX->SR,  UART_SR_TFNF))
                {
                    if (huart->Tx_Env.Interrupt.XferCount == 0U)
                    {
                        break;
                    }
                    huart->Tx_Env.Interrupt.XferCount--;
                    huart->UARTX->TBR = (*huart->Tx_Env.Interrupt.pBuffPtr++ & (uint8_t)0xFF);
                }
            }
        }
    }
}

/**
  * @brief  Wraps up transmission in non blocking mode.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static void UART_EndTransmit_IT(UART_HandleTypeDef *huart)
{
    /* Disable the UART Transmit Complete Interrupt */
        huart->UARTX->IDR = UART_IT_TC ;
        /* Tx process is ended, restore huart->gState to Ready */
        huart->gState = HAL_UART_STATE_READY;
        HAL_UART_TxCpltCallback(huart);

    
}

/**
  * @brief  Receives an amount of data in non blocking mode
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static void UART_Receive_IT(UART_HandleTypeDef *huart)
{
    uint8_t fifo_level;
    /* Check that a Rx process is ongoing */
    if (huart->RxState == HAL_UART_STATE_BUSY_RX)
    {
        fifo_level = REG_FIELD_RD(huart->UARTX->FCR, UART_FCR_RXFL);
        for (;fifo_level>0;fifo_level--)
        {
            *huart->Rx_Env.Interrupt.pBuffPtr++ = (uint8_t)(huart->UARTX->RBR & (uint8_t)0x00FF);
            if (--huart->Rx_Env.Interrupt.XferCount == 0U)
            {
                REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_8);
                huart->RxState = HAL_UART_STATE_READY;
                huart->UARTX->IDR = UART_IT_RXRD;
                HAL_UART_RxCpltCallback(huart);
                return;
            }
            if(huart->Rx_Env.Interrupt.XferCount<0x08)
            {
                REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_1);
            }
            else
            {
                REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_8);
            }
        }
    }
}

__attribute__((weak)) void UART_EndTransmit_IT_DMA(UART_HandleTypeDef *huart){}

__attribute__((weak)) void UART_Transmit_IT_DMA(UART_HandleTypeDef *huart){}

/**
  * @brief  This function handles UARTx interrupt request.
  * @param  huart  Pointer to a UARTx_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */

void HAL_UARTx_IRQHandler(UART_HandleTypeDef *huart)
{
    uint32_t isrflags   = huart->UARTX->IFM;  
    uint32_t status_reg = huart->UARTX->SR;
    
    if(REG_FIELD_RD(huart->UARTX->MCR,UART_MCR_DMAEN))
    {
        if (( isrflags& UART_IT_TXS) != 0) 
        {
            huart->UARTX->ICR = UART_IT_TXS;
            UART_Transmit_IT_DMA(huart);
        }
        if ((isrflags & UART_IT_TC) != 0)
        {
            huart->UARTX->ICR = UART_IT_TC;
            UART_EndTransmit_IT_DMA(huart);
        }
    }else
    {
        if (( isrflags& UART_IT_TXS) != 0) 
        {
            huart->UARTX->ICR = UART_IT_TXS;
            UART_Transmit_IT(huart);
        }
        if ((isrflags & UART_IT_TC) != 0)
        {
            huart->UARTX->ICR = UART_IT_TC;
            UART_EndTransmit_IT(huart);
        }
    }
    /* UART in mode Receive ------------------------------------------------*/
    if ((isrflags & UART_IT_RXRD) != 0) 
    {
        UART_Receive_IT(huart);
    }
    if ((status_reg & 0x02) != 0)
    {
        LOG_I("uart_rx overrun!!!");
    }
}

HAL_StatusTypeDef HAL_UART_AutoBaudRate_Detect(UART_HandleTypeDef *huart,uint8_t mode)
{
    REG_FIELD_WR(huart->UARTX->LCR,UART_LCR_BRWEN,1);
    MODIFY_REG(huart->UARTX->MCR, UART_MCR_ABRMOD_MASK | UART_MCR_ABREN_MASK, mode<<UART_MCR_ABRMOD_POS | 1<<UART_MCR_ABREN_POS);
    while(REG_FIELD_RD(huart->UARTX->RIF, UART_ABE)==0);
    huart->UARTX->ICR = UART_ABE_MASK;
    while(REG_FIELD_RD(huart->UARTX->SR,UART_SR_BUSY));
    REG_FIELD_WR(huart->UARTX->MCR,UART_MCR_ABREN,0);
    REG_FIELD_WR(huart->UARTX->LCR,UART_LCR_BRWEN,0);
    return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_AutoBaudRate_Detect_IT(UART_HandleTypeDef * huart,uint8_t mode);






