
#include <stdio.h>
#include "lsuart.h"
#include "uart_param.h" 
#include "log.h"

static void UART_EndTransmit_IT(UART_HandleTypeDef *huart);
static void UART_Transmit_IT(UART_HandleTypeDef *huart);
static void UART_Receive_IT(UART_HandleTypeDef *huart);
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
    uart_sw_reset(huart);
    uart_clock_enable(huart,1);
    /* Set the UART Communication parameters */
    UART_SetConfig(huart);
    uart_int_op(HAL_UARTx_IRQHandler,huart,1);
    uart_status_set(huart, 1);

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
    uart_clock_enable(huart,0);
    uart_int_op(HAL_UARTx_IRQHandler,huart,0);
    uart_status_set(huart, 0);

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
__attribute__((weak)) void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart,void *tx_arg)
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
__attribute__((weak)) void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart,void *rx_arg)
{
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
    //   uint32_t tickstart = 0U;
    //   uint32_t timeout = (Timeout*UART_CLOCK)/1000;
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
        huart->TxXferCount = Size;
        
        while (huart->TxXferCount > 0U )
        {
            if (huart->UARTX->SR & UART_SR_TFNF)
            {
                //Transmit FIFO Not Full
                huart->TxXferCount--;
                huart->UARTX->TBR = (*pData++ & (uint8_t)0xFF);
                if (huart->TxXferCount == 0)
                {
                    break;
                }
            }
        }
        // Wait until TX Finish
        while (!(huart->UARTX->SR & UART_SR_TEMT))
        ;
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
    // uint32_t tickstart = 0U;
    // uint32_t Timeout_ms = Timeout*UART_CLOCK/1000;
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
        huart->TxXferCount = Size;
        huart->tx_arg = tx_arg;
        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->gState = HAL_UART_STATE_BUSY_TX;
        REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_TXTL,UART_FIFO_TL_0);
    /* Enable the UART Transmit data register empty Interrupt */
        huart->UARTX->ICR = UART_IT_TC| UART_IT_TXS;
        huart->UARTX->IER = UART_IT_TXS;
        huart->TxXferCount--; 
        huart->UARTX->TBR = (*huart->pTxBuffPtr++ & (uint8_t)0xFF);
        if(huart->TxXferCount == 0)
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
        huart->RxXferCount = Size;
        huart->ErrorCode = HAL_UART_ERROR_NONE;
        huart->RxState = HAL_UART_STATE_BUSY_RX;
        huart->rx_arg = rx_arg;
        if(huart->RxXferCount<8)
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
                                  |FIELD_BUILD(UART_LCR_PARITY,huart->Init.Parity)|FIELD_BUILD(UART_LCR_MSB,huart->Init.MSBEN)
                                  |FIELD_BUILD(UART_LCR_RXEN,1)|FIELD_BUILD(UART_LCR_BRWEN,0);
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
            if (huart->TxXferCount == 0U)
            {
                huart->UARTX->IDR = UART_IT_TXS;
                huart->UARTX->IER = UART_IT_TC;
            }
            else
            {
                while(REG_FIELD_RD(huart->UARTX->SR,  UART_SR_TFNF))
                {
                    if (huart->TxXferCount == 0U)
                    {
                        break;
                    }
                    huart->TxXferCount--;
                    huart->UARTX->TBR = (*huart->pTxBuffPtr++ & (uint8_t)0xFF);
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
        HAL_UART_TxCpltCallback(huart,huart->tx_arg);

    
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
            *huart->pRxBuffPtr++ = (uint8_t)(huart->UARTX->RBR & (uint8_t)0x00FF);
            if (--huart->RxXferCount == 0U)
            {
                REG_FIELD_WR(huart->UARTX->FCR,UART_FCR_RXTL,UART_FIFO_RL_8);
                huart->RxState = HAL_UART_STATE_READY;
                huart->UARTX->IDR = UART_IT_RXRD;
                HAL_UART_RxCpltCallback(huart,huart->rx_arg);
                return;
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
}

/**
  * @brief  This function handles UARTx interrupt request.
  * @param  huart  Pointer to a UARTx_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
volatile uint8_t uart_rto_flag = 0;  // debug

void HAL_UARTx_IRQHandler(UART_HandleTypeDef *huart)
{
    uint32_t isrflags   = huart->UARTX->IFM;  
    uint32_t status_reg = huart->UARTX->SR;
		if ((isrflags & UART_IT_RTO) !=0 )
		{
			uint8_t *bufPtr;
      uint32_t	dma_recved=0;
			DMA_CTRLSTURCT_TypeDef *ctrlstrcut;
				// get DMA received bytes from ctrl struct.
				// calculated address of data to be save by manual read
			ctrlstrcut = (DMA_CTRLSTURCT_TypeDef *)(huart->rxDMARto.ctrl_struct);
			dma_recved = huart->rxDMARto.set_size - ctrlstrcut->ctrl.N_MINUS_1;
			huart->rxDMARto.recv_size = dma_recved;
			bufPtr = (uint8_t *)(huart->rxDMARto.dst_end_ptr) + dma_recved;
				// disable DMA
			while (READ_BIT(huart->UARTX->SR,UART_SR_DR))
			{
			*bufPtr++ = (uint8_t)(huart->UARTX->RBR & (uint8_t)0xff);
			huart->rxDMARto.recv_size++;
			}
			WRITE_REG(huart->UARTX->ICR, UART_IT_RTO);
			// disable DMA
			ctrlstrcut->ctrl.N_MINUS_1 = 0;
			huart->hdmarx.Instance->ENCLR = 1 << huart->hdmarx.Config.chnIndex;
			DMA1INT->DONEICF = 1<< huart->hdmarx.Config.chnIndex;
			huart->hdmarx.XferCpltCallback(&huart->hdmarx);
			uart_rto_flag=1;
		}
    /* UART in mode Transmitter ------------------------------------------------*/
    if (( isrflags& UART_IT_TXS) != 0) 
    {
        huart->UARTX->ICR = UART_IT_TXS;
        UART_Transmit_IT(huart);
    }
    /* UART in mode Transmitter end --------------------------------------------*/
    if ((isrflags & UART_IT_TC) != 0)
    {
        huart->UARTX->ICR = UART_IT_TC;
        UART_EndTransmit_IT(huart);
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


/**
  * @brief DMA UART transmit process complete callback.
  * @param hdma DMA handle.
  * @retval None
  */
static void UART_DMATransmitCplt(DMA_HandleTypeDef *hdma)
{
	UART_HandleTypeDef *huart = (UART_HandleTypeDef *)(hdma->Parent);
	
	if ((huart->gState == HAL_UART_STATE_BUSY_TX) && (huart->gState != HAL_UART_STATE_BUSY_TX))
		CLEAR_BIT(huart->UARTX->MCR, UART_MCR_DMAEN_MASK);
		
    huart->TxXferCount = 0U;

    /* Enable the UART Transmit Complete Interrupt */
    WRITE_REG(huart->UARTX->IER, UART_TC_MASK);

    /*Call legacy weak Tx complete callback*/
    HAL_UART_TxCpltCallback(huart,NULL);
}

static void UART_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
    UART_HandleTypeDef *huart = (UART_HandleTypeDef *)(hdma->Parent);

    if ((huart->RxState == HAL_UART_STATE_BUSY_RX) && (huart->gState != HAL_UART_STATE_BUSY_TX))
        CLEAR_BIT(huart->UARTX->MCR, UART_MCR_DMAEN_MASK);

    huart->RxXferCount = 0;
    huart->RxState = HAL_UART_STATE_READY;

    HAL_UART_RxCpltCallback(huart,NULL);

    return;
}

/**
  * @brief DMA UART communication error callback.
  * @param hdma DMA handle.
  * @retval None
  */
static void UART_DMAError(DMA_HandleTypeDef *hdma)
{
  UART_HandleTypeDef *huart = (UART_HandleTypeDef *)(hdma->Parent);

  //const HAL_UART_StateTypeDef gstate = huart->gState;
  //const HAL_UART_StateTypeDef rxstate = huart->RxState;

	huart->TxXferCount = 0U;
	huart->RxXferCount = 0U;
	/* stop uart DMA request */
	CLEAR_BIT(huart->UARTX->MCR, UART_MCR_DMAEN_MASK);
	
	huart->gState = HAL_UART_STATE_READY;
	huart->RxState = HAL_UART_STATE_READY;
  huart->ErrorCode |= HAL_UART_ERROR_DMA;

  /*Call legacy weak error callback*/
	/* User can add custom code to process UART settings when DMA error there */
  //HAL_UART_ErrorCallback(huart,NULL);

}

HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	/* Check that a Tx process is not already ongoing */
	if (huart->gState != HAL_UART_STATE_READY)
		return HAL_BUSY;
	
	if ((pData == NULL) || (Size == 0U))
		return HAL_ERROR;

	//__HAL_LOCK(huart);
	
	huart->pTxBuffPtr  = pData;
//	huart->TxXferSize  = Size;
	huart->TxXferCount = Size;

	huart->ErrorCode = HAL_UART_ERROR_NONE;
	huart->gState = HAL_UART_STATE_BUSY_TX;	
	

	/* Set the UART DMA transfer complete callback */
	huart->hdmatx.XferCpltCallback = UART_DMATransmitCplt;

	/* Set the DMA error callback */
	huart->hdmatx.XferErrorCallback = UART_DMAError;

	/* Enable the UART transmit DMA channel */
	if (HAL_DMA_Start_IT(&huart->hdmatx, (uint32_t)huart->pTxBuffPtr, (uint32_t)&huart->UARTX->TBR, Size) != HAL_OK)
	{
		/* Set error code to DMA */
		huart->ErrorCode = HAL_UART_ERROR_DMA;

		//__HAL_UNLOCK(huart);

		/* Restore huart->gState to ready */
		huart->gState = HAL_UART_STATE_READY;

		return HAL_ERROR;
	}		
	
	
	/* Clear the TC flag in the ICR register */
	WRITE_REG(huart->UARTX->ICR, UART_TC_MASK);
	WRITE_REG(huart->UARTX->ICR, UART_IT_TXS); // threshold empty 

	//__HAL_UNLOCK(huart);

	/* Enable the DMA transfer */
	SET_BIT(huart->UARTX->MCR, UART_MCR_DMAEN_MASK);

	return HAL_OK;
}

HAL_StatusTypeDef HAL_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	if (huart->RxState != HAL_UART_STATE_READY)
		return HAL_BUSY;
	
	if ((pData == NULL) || (Size == 0U))  // TODO: add address check here?
		return HAL_ERROR;
	
	//__HAL_LOCK(huart);
	
	huart->pRxBuffPtr = pData;
	//huart->RxXferSize = Size;
	huart->ErrorCode = HAL_UART_ERROR_NONE;
	huart->RxState = HAL_UART_STATE_BUSY_RX;
	

	if(&(huart->hdmarx) != NULL)
    {
      /* Set the UART DMA transfer complete callback */
      huart->hdmarx.XferCpltCallback = UART_DMAReceiveCplt;

      /* Set the DMA error callback */
      huart->hdmarx.XferErrorCallback = UART_DMAError;

      /* Enable the DMA channel */
      if (HAL_DMA_Start_IT(&huart->hdmarx, (uint32_t)&huart->UARTX->RBR, (uint32_t)huart->pRxBuffPtr, Size) != HAL_OK)
      {
        /* Set error code to DMA */
        huart->ErrorCode = HAL_UART_ERROR_DMA;

        //__HAL_UNLOCK(huart);

        /* Restore huart->gState to ready */
        huart->gState = HAL_UART_STATE_READY;

        return HAL_ERROR;
      }
    }
		
		if (huart->rxDMARto.dst_end_ptr != 0)
		{
			huart->rxDMARto.set_size = Size - 1;
			huart->rxDMARto.dst_end_ptr = (void *)pData;
		}
    //__HAL_UNLOCK(huart);

    /* Enable the DMA transfer for the receiver request by setting the DMAR bit
    in the UART CR3 register */
	  SET_BIT(huart->UARTX->MCR, UART_MCR_DMAEN_MASK);

    return HAL_OK; 

}





