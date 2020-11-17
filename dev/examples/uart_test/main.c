#include "lsuart.h"
#include "lsgpio.h"
#include "le501x.h"
#include "platform.h"
#include "io_config.h"
#include <string.h>

#define TEST_ZONE_SIZE 512
static void uart_test_init(void);
static void gpio_init();
/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
UART_HandleTypeDef huart; 
uint8_t test_zone_a[TEST_ZONE_SIZE * 2] ;

/* Buffer used for transmission */
uint8_t aTxBuffer[] __attribute__((section("DMARAM"))) =  "123456****UART_TwoBoards_ComIT****789ssssssssssssssssssssssssssssssssssaaaaaaaaaaaaaaaend\n" ;
/* Buffer used for reception */
uint8_t aRxBuffer[100] __attribute__((section("DMARAM")));

volatile uint8_t recv_flag = 0;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	recv_flag = 1;
}

static void gpio_init()
{
    uart1_io_init(PB00,PB01);
}

static void uart_test_init(void)
{
    huart.UARTX = UART1;
    huart.Init.BaudRate = UART_BAUDRATE_115200;
    huart.Init.MSBEN = 0;
    huart.Init.Parity = UART_NOPARITY;
    huart.Init.StopBits = UART_STOPBITS1;
    huart.Init.WordLength = UART_BYTESIZE8;
    HAL_UART_Init(&huart);
}

static void uart_test()
{
    HAL_UART_Receive_IT(&huart,test_zone_a,1,NULL);
}


void UART_DMA_Init(UART_HandleTypeDef *huart)
{
    /*
	DMA_HandleTypeDef hdma_tx;
	DMA_HandleTypeDef hdma_rx;
	hdma_tx = huart->hdmatx;
	hdma_rx = huart->hdmatx;
	*/

	/* Configure DMA */
	// Configure the DMA handler for xfer process
	// DMA registers config
	huart->hdmatx.Config.dst_inc = DMA_INC_NONE;
	huart->hdmatx.Config.src_inc = DMA_INC_BYTE;
	huart->hdmatx.Config.data_size = DMA_SIZE_BYTE;
	huart->hdmatx.Config.next_useburst = DISABLED;
  huart->hdmatx.Config.primary = ENABLED;
  huart->hdmatx.Config.priority = DMA_PRIO_DEFAULT;
  huart->hdmatx.Config.src_end_ptr = (void *)&aTxBuffer[0];
	huart->hdmatx.Config.dst_end_ptr = (void *)huart->UARTX->TBR;
	huart->hdmatx.Config.cycle_ctrl = DMA_CC_BASIC; //DMA_CC_AUTOREQ;
	huart->hdmatx.Config.burst = ENABLED;
	huart->hdmatx.Config.intEn = ENABLED;
	huart->hdmatx.Config.sigSel = CH_UART1_TX;
	huart->hdmatx.Config.chnIndex = 0;
	huart->hdmatx.Config.rpower = DMA_RPOWER16;
	huart->hdmatx.Parent = huart;
	huart->hdmatx.Instance = DMA1;

	huart->hdmarx.Config.dst_inc = DMA_INC_BYTE;
	huart->hdmarx.Config.src_inc = DMA_INC_NONE;
	huart->hdmarx.Config.data_size = DMA_SIZE_BYTE;
	huart->hdmarx.Config.next_useburst = DISABLED;
  huart->hdmarx.Config.primary = ENABLED;
  huart->hdmarx.Config.priority = DMA_PRIO_DEFAULT;
  huart->hdmarx.Config.src_end_ptr = (void *)huart->UARTX->RBR;
	huart->hdmarx.Config.dst_end_ptr = (void *)&aRxBuffer[0];
	huart->hdmarx.Config.cycle_ctrl = DMA_CC_BASIC;
	huart->hdmarx.Config.burst = ENABLED;
	huart->hdmarx.Config.intEn = ENABLED;
	huart->hdmarx.Config.sigSel = CH_UART1_RX;
	huart->hdmarx.Config.chnIndex = 1;

  // Note: To implement dynamic length DMA receive, DMA_RPOWER2 + FCR_RXFIFO_RL_4 is required
	huart->hdmarx.Config.rpower = DMA_RPOWER2;
  MODIFY_REG(huart->UARTX->FCR, UART_FCR_RXTL_MASK,  (0x1<<UART_FCR_RXTL_POS));
	huart->rxDMARto.dst_end_ptr = (void *)&aRxBuffer[0];
	huart->rxDMARto.recv_size = 0; 
	if (huart->hdmarx.Config.primary)
        huart->rxDMARto.ctrl_struct = (uint32_t *)(DMA1->BPTR + (huart->hdmarx.Config.chnIndex << 4));
    else
        huart->rxDMARto.ctrl_struct = (uint32_t *)(DMA1->ALTBPTR + (huart->hdmarx.Config.chnIndex << 4));
	
	
	huart->hdmarx.Parent = huart;
	huart->hdmarx.Instance = DMA1;
	
	HAL_DMA_Init(&huart->hdmatx);
	HAL_DMA_Init(&huart->hdmarx);
}

void DMA_Handler(DMA_HandleTypeDef *hdma)
{
	HAL_DMA_IRQHandler(&huart.hdmatx);
	HAL_DMA_IRQHandler(&huart.hdmarx);
}
extern volatile uint8_t uart_rto_flag;
int main()
{
    sys_init_app();
    gpio_init();
	HAL_DMA_LL_Init();
	arm_cm_set_int_isr(DMA_IRQn,DMA_Handler);
	huart.UARTX = UART1;
	huart.Init.BaudRate = 0x8b; //115200;
	huart.Init.WordLength = UART_BYTESIZE8;
	huart.Init.Parity     = UART_NOPARITY;
	huart.Init.StopBits = UART_STOPBITS1;
	HAL_UART_Init(&huart);
  UART_DMA_Init(&huart);
	WRITE_REG(huart.UARTX->RTOR, 0x5);
	SET_BIT(huart.UARTX->LCR, UART_LCR_RTOEN_MASK);
	WRITE_REG(huart.UARTX->IER, UART_RTO_MASK);

    HAL_UART_Transmit(&huart,(uint8_t *)aTxBuffer, sizeof(aTxBuffer),0);
	HAL_UART_Transmit_DMA(&huart, (uint8_t *)aTxBuffer, sizeof(aTxBuffer));
	
//	WRITE_REG(huart.Instance->RTOR, 0x55);
//	SET_BIT(huart.Instance->LCR, UART_LCR_RTOEN_MASK);
//	WRITE_REG(huart.Instance->IER, UART_IER_RTO_MASK);
	
	HAL_UART_Receive_DMA(&huart, (uint8_t *)aRxBuffer, 100);

	while(1)
	{
		if (uart_rto_flag)
		{
			HAL_UART_Transmit_DMA(&huart, (uint8_t *)aRxBuffer, huart.rxDMARto.recv_size);
			uart_rto_flag=0;
			for (int i=0;i<1000;i++){__NOP();};
			HAL_UART_Receive_DMA(&huart, (uint8_t *)aRxBuffer, 100);
		}
		
	};
	
}




