
#include <stdio.h>
#include "uart.h"
#include "field_manipulate.h"
#include "uart_param.h" 


static HAL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef *huart);
static HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart);
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
    uart_sw_reset();
    uart_clock_enable();
    /* Set the UART Communication parameters */
    UART_SetConfig(huart);
    uart_int_op(HAL_UARTx_IRQHandler,huart,1);
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

    huart->gState = HAL_UART_STATE_BUSY;
    huart->ErrorCode = HAL_UART_ERROR_NONE;
    huart->gState = HAL_UART_STATE_RESET;
    huart->RxState = HAL_UART_STATE_RESET;
    return HAL_OK;
}
/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
__attribute__((weak)) void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    //todo
    return ;
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
    //todo
    return ;
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
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
        huart->TxXferSize = Size;
        huart->TxXferCount = Size;
        
        while (huart->TxXferCount > 0U)
        {
            huart->TxXferCount--;
            huart->UARTX->TBR = (*pData++ & (uint8_t)0xFF);
            // Wait until TX Buffer Empty
            while (!(huart->UARTX->SR & UART_SR_TBEM))
            ;
        }
        
        // tx_fifo reset.
        REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_TFRST,1);
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

        REG_FIELD_WR(huart->UARTX->LCR,UART_LCR_RXEN,1);
    
        huart->RxXferSize = Size;
        huart->RxXferCount = Size;

        /* Check the remain data to be received */
        while (huart->RxXferCount > 0U)
        {
            while((huart->UARTX->SR & UART_SR_DR)==0)
                ;
            huart->RxXferCount--;
            *pData++ = (uint8_t)(huart->UARTX->RBR & (uint8_t)0x00FF);
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
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, void *tx_arg)

{
    /* Check that a Tx process is not already ongoing */
    if (huart->gState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return HAL_INVALIAD_PARAM;
        }
        huart->pTxBuffPtr = pData;
        huart->TxXferSize = Size;
        huart->TxXferCount = Size;

        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->gState = HAL_UART_STATE_BUSY_TX;
        REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_FIFOEN,1);
        REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_1);
        
    /* Enable the UART Transmit data register empty Interrupt */
        REG_FIELD_WR(huart->UARTX->IER,UART_IER_TC,1);
        REG_FIELD_WR(huart->UARTX->IER,UART_IER_TXS,1);
        huart->TxXferCount--;
        huart->UARTX->TBR = (*huart->pTxBuffPtr++ & (uint8_t)0xFF);
        return HAL_OK;
    }
    else
    {
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
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, void *rx_arg)
{
    /* Check that a Rx process is not already ongoing */
    if (huart->RxState == HAL_UART_STATE_READY)
    {
        if ((pData == NULL) || (Size == 0U))
        {
            return HAL_INVALIAD_PARAM;
        }
        huart->pRxBuffPtr = pData;
        huart->RxXferSize = Size;
        huart->RxXferCount = Size;
        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->RxState = HAL_UART_STATE_BUSY_RX;
        REG_FIELD_WR(huart->UARTX->LCR,UART_LCR_RXEN,1);
        REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_FIFOEN,1);
        if(huart->RxXferSize<8)
        {
            REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_1);
        }
        else
        {
            REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_8);
        } 
        
        /* Enable the UART Data Register not empty Interrupt */
        REG_FIELD_WR(huart->UARTX->IER,UART_IER_RXRD,1);
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
    huart->UARTX->LCR = FIELD_BUILD(UART_LCR_DLS,huart->Init.WordLength)|FIELD_BUILD(UART_LCR_STOP,huart->Init.StopBits)
                                  |FIELD_BUILD(UART_LCR_PS,huart->Init.Parity)|FIELD_BUILD(UART_LCR_MSB,huart->Init.MSBEN);
    REG_FIELD_WR(huart->UARTX->LCR,UART_LCR_BRWEN,1);
    if(huart->Init.BaudRate != 0)
    {
        huart->UARTX->BRR  =  huart->Init.BaudRate;  //todo 
    }
    REG_FIELD_WR(huart->UARTX->LCR,UART_LCR_BRWEN,0);
    REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_FIFOEN,1);   
}

/**
  * @brief  Sends an amount of data in non blocking mode.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart)
{
    uint8_t i=0;
    /* Check that a Tx process is ongoing */
    if (huart->gState == HAL_UART_STATE_BUSY_TX)
    {
        if(!REG_FIELD_RD(huart->UARTX->FCR, UART_FCR_TXFL))
        {
            for(i=0;i<8;i++)
            {
                huart->UARTX->TBR = (*huart->pTxBuffPtr++ & (uint8_t)0xFF);
                if (--huart->TxXferCount == 0U)
                {
                    huart->UARTX->IDR = FIELD_BUILD(UART_IDR_TXS,1);
                    huart->gState = HAL_UART_STATE_READY; 
                    return HAL_OK;
                }
            }
            return HAL_OK;
        }
        else
        {
            return HAL_BUSY;
        }
    }
    return HAL_STATE_ERROR;
}

/**
  * @brief  Wraps up transmission in non blocking mode.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_EndTransmit_IT(UART_HandleTypeDef *huart)
{
    /* Disable the UART Transmit Complete Interrupt */
    REG_FIELD_WR(huart->UARTX->IDR,UART_IDR_TC,1);
    huart->gState = HAL_UART_STATE_READY;
    HAL_UART_TxCpltCallback(huart);
    /* Tx process is ended, restore huart->gState to Ready */
    return HAL_OK;
}

/**
  * @brief  Receives an amount of data in non blocking mode
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval HAL status
  */
static HAL_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart)
{
    /* Check that a Rx process is ongoing */
    if (huart->RxState == HAL_UART_STATE_BUSY_RX)
    {
        while(REG_FIELD_RD(huart->UARTX->FCR, UART_FCR_RXFL))
        {
            *huart->pRxBuffPtr++ = (uint8_t)(huart->UARTX->RBR & (uint8_t)0x00FF);
            if (--huart->RxXferCount == 0U)
            {
                huart->UARTX->IDR = FIELD_BUILD(UART_IDR_TXS,1)|FIELD_BUILD(UART_IDR_TC,1);
                REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_8);
                huart->RxState = HAL_UART_STATE_READY;
                HAL_UART_RxCpltCallback(huart);
                return HAL_OK;
            }
            if(huart->RxXferCount<0x08)
            {
                REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_1);
            }
            else
            {
                REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_8);
            }
        }
    }
    return  HAL_STATE_ERROR;
}

/**
  * @brief  This function handles UARTx interrupt request.
  * @param  huart  Pointer to a UARTx_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */

void HAL_UARTx_IRQHandler(UART_HandleTypeDef *huart)
{
    uint32_t isrflags   = huart->UARTX->RIF;  
    uint32_t ivsits     = huart->UARTX->IVS;  
    /* UART in mode Transmitter ------------------------------------------------*/
    if ((( isrflags& UART_IT_TXS) != 0) &&((ivsits & UART_IT_TXS) != 0))
    {
        UART_Transmit_IT(huart);
        return;
    }

    /* UART in mode Transmitter end --------------------------------------------*/
    if (((isrflags & UART_IT_TC) != 0) && ((ivsits & UART_IT_TC) != 0))
    {
        if (((isrflags & UART_IT_TXS) != 0)) 
        {
            UART_EndTransmit_IT(huart);
        }
        return;
    }
    /* UART in mode Receive ------------------------------------------------*/
    if (((isrflags & UART_IT_RXRD) != 0) && ((ivsits & UART_IT_RXRD) != 0))
    {
        UART_Receive_IT(huart);
        return;
    }
}








