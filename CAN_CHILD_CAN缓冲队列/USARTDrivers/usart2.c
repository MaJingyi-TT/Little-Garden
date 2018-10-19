#include "usart2.h"

uint8_t *_US2_RxBuffer = NULL;
uint16_t _US2_RxbufferSize = 0;

bool USART2_Init(uint32_t BaudRate, uint8_t *RxBuffer, uint16_t RxBufferSize)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(RxBuffer != NULL && RxBufferSize > 0)
	{
		_US2_RxBuffer = RxBuffer;
		_US2_RxbufferSize = RxBufferSize;

		//USART2 Rx NVIC
		NVIC_InitStructure.NVIC_IRQChannel = US2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
		NVIC_Init(&NVIC_InitStructure);	

		//US2_DMA_Tx DMA Tx NVIC
		NVIC_InitStructure.NVIC_IRQChannel = US2_DMA_Tx_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		//US2_DMA_Rx DMA Rx NVIC
		NVIC_InitStructure.NVIC_IRQChannel = US2_DMA_Rx_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
//////////////////////////////////////////////////////////////
		Enable_US2_Periph_CLK;//enable usart2 peripheral clock
		Enable_US2_GPIO_CLK;//enable GPIOA peripheral clock
		
		USART_DeInit(USART2);//reset usart2
		
		//USART2_TX Pin  
		GPIO_InitStructure.GPIO_Pin = US2_Tx_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     	
		GPIO_Init(US2_GPIO_Port, &GPIO_InitStructure);             
	   
		//USART2_RX Pin
		GPIO_InitStructure.GPIO_Pin = US2_Rx_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
		GPIO_Init(US2_GPIO_Port, &GPIO_InitStructure);
		
		//USART2 configration
		USART_InitStructure.USART_BaudRate = BaudRate;										
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;						
		USART_InitStructure.USART_StopBits = USART_StopBits_1;							
		USART_InitStructure.USART_Parity = USART_Parity_No;								
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
		USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//enable both Tx & Rx mode					
		USART_Init(USART2, &USART_InitStructure);      									
		
		//USART2 DMA_Init
		USART2_DMA_Init(RxBuffer, RxBufferSize);

		//Enable IDLE Interupt
		USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); //Enable IDLE line detecte Interupt

		USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE); //Enable USART2 DMA TX request 
		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE); //Enable USART2 DMA RX request
		
		USART_Cmd(USART2, ENABLE);
		
		return true;
	}
	else
		return false;
}

void USART2_DMA_Init(uint8_t *RxBuffer, uint16_t RxBufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
    
	Enable_US2_DMA_CLK; 
	
 ///////////////////////////////////////////////DMA Tx Config///////////////////////////////////////
	//Config US2_DMA_Tx CH4 for USART2_Tx
    DMA_DeInit(US2_DMA_Tx);// DeInit US2_DMA_Tx firstly
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);//USART2->DR is the PeripheralBaseAddrress
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;//Set memory base address to 0 at first
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//Mem To Peri 
    DMA_InitStructure.DMA_BufferSize = 0;//set buffer size to 0 at first
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	//Init US2_DMA_Tx for USART2_Tx
    DMA_Init(US2_DMA_Tx, &DMA_InitStructure);
    DMA_ITConfig(US2_DMA_Tx, DMA_IT_TC, ENABLE);//Enable Transmission Complete Interupt
    DMA_Cmd(US2_DMA_Tx, DISABLE); //Disable Tx at low level intializing
   
///////////////////////////////////////////////DMA Rx Config///////////////////////////////////////
	//Config US2_DMA_Rx CH4 for USART2_RX
    DMA_DeInit(US2_DMA_Rx);// DeInit US2_DMA_Rx firstly
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);//USART2->DR is the PeripheralBaseAddrress
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(uint32_t)RxBuffer;//MemoryBaseAddress
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//Peri To Mem
    DMA_InitStructure.DMA_BufferSize = RxBufferSize;// The length of RxBuffer;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	//Init US2_DMA_Rx for USART2_Rx
	DMA_Init(US2_DMA_Rx, &DMA_InitStructure);
	DMA_ITConfig(US2_DMA_Rx, DMA_IT_TC, ENABLE);//Enable Receive Complete Interupt
    DMA_Cmd(US2_DMA_Rx, ENABLE);//Enable Rx at low level intializing
   
}


////////////////////////Interupt Process Callback Functions/////////////////////////
//The callback functions can be overloaded by upper applications
__weak void US2DMA_RxIdle_Callback(void)
{
	//reset the  DMA Rx configration
	US2_DMA_Rx->CNDTR = _US2_RxbufferSize;
	US2_DMA_Rx->CMAR = (uint32_t)_US2_RxBuffer;
	DMA_Cmd(US2_DMA_Rx, ENABLE);
}

__weak void US2DMA_RxCplt_Callback(void)
{
	//reset the  DMA Rx configration
	US2_DMA_Rx->CNDTR = _US2_RxbufferSize;
	US2_DMA_Rx->CMAR = (uint32_t)_US2_RxBuffer;
	DMA_Cmd(US2_DMA_Rx, ENABLE);
}

__weak void US2DMA_TxCplt_Callback(void)
{
	
}

////////////////////////System Interupt Request Handler Functions/////////////////////////
//US2 Rx Idle line detecte Interupt
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_IDLE)==SET)        
	{
		USART2->SR;// IDLE (Idle line detected) pending bits are cleared 
		USART2->DR;//by software sequence: a read operation to USART_SR register and USART_DR register

		DMA_Cmd(US2_DMA_Rx, DISABLE);
		DMA_ClearFlag(US2_DMA_RxGL_FLAG);
		
		US2DMA_RxIdle_Callback();
	}
}

//USART2 DMA Receive Complete--US2_DMA_Rx
void US2_DMA_Rx_IRQHandler(void)
{
    if(DMA_GetITStatus(US2_DMA_RxTC_IT))
    {
		DMA_ClearFlag(US2_DMA_RxGL_FLAG);
		DMA_Cmd(US2_DMA_Rx, DISABLE);

		US2DMA_RxCplt_Callback();
    }
}

//USART2 DMA Transmit Complete--US2_DMA_Tx
void US2_DMA_Tx_IRQHandler(void)
{
    if(DMA_GetITStatus(US2_DMA_TxTC_IT))
    {
		DMA_ClearFlag(US2_DMA_TxGL_FLAG);
		DMA_Cmd(US2_DMA_Tx, DISABLE);

		US2DMA_TxCplt_Callback();
    }
}


