#include "USx_Process.h"

//RxBuffer struct for each serial port
struct US_RxBuffer   US1_RxBuffer;
struct US_RxBuffer   US2_RxBuffer;
struct US_RxBuffer   US3_RxBuffer;
//Rx buffer pool for each port
u8 US1_RxPool[US1RxPoolLen];
u8 US2_RxPool[US2RxPoolLen];
u8 US3_RxPool[US3RxPoolLen];

/////////////////////////////////Rx ring buffer struct,for saving Rx data/////////////////////////
struct ringbuffer US1_RxRingbuffer;
struct ringbuffer US2_RxRingbuffer;
struct ringbuffer US3_RxRingbuffer;

//Rx ring buffer pool for each port
u8 US1_RxRingbufPool[US1_RxRingBufferLen];
u8 US2_RxRingbufPool[US2_RxRingBufferLen];
u8 US3_RxRingbufPool[US3_RxRingBufferLen];
//////////////////////////////////////////////////////////////////////////////////////////////////

//TxBuffer Data struct for each port
struct US_TxBuffer   US1_TxBuffer;
struct US_TxBuffer   US2_TxBuffer;
struct US_TxBuffer   US3_TxBuffer;
//Tx buffer for each port
u8 US1_TxPool[US1TxPoolLen];
u8 US2_TxPool[US2TxPoolLen];
u8 US3_TxPool[US3TxPoolLen];

/////////////////////////////////////Rx buffer struct Init////////////////////////////////////////
void US_RxBufferInit(struct US_RxBuffer *USx_RxBuf, u8 *buffer, u16 bufferSize)
{
	USx_RxBuf->Ptr = buffer;
	USx_RxBuf->Size = bufferSize;

	ResetUS_RxBuffer(USx_RxBuf);
}

/////////////////////////////////////Tx buffer struct Init////////////////////////////////////////
void US_TxBufferInit(struct US_TxBuffer *USx_TxBuf, u8 *buffer, u16 bufferSize)
{
	USx_TxBuf->Ptr = buffer;
	USx_TxBuf->Size = bufferSize;

	ResetUS_TxBuffer(USx_TxBuf);
}
///////////////////////////////US1 US2 US3 Low Level initialization/////////////////////
void SerialCom1Init(u32 Baudrate)
{
	USART1_Init(Baudrate,US1_RxPool,US1RxPoolLen);
	US_RxBufferInit(&US1_RxBuffer,US1_RxPool,US1RxPoolLen);
	ringbuffer_init(&US1_RxRingbuffer, US1_RxRingbufPool, US1_RxRingBufferLen);
	US_TxBufferInit(&US1_TxBuffer,US1_TxPool,US1TxPoolLen);
}

void SerialCom2Init(u32 Baudrate)
{
	USART2_Init(Baudrate,US2_RxPool,US2RxPoolLen);
	US_RxBufferInit(&US2_RxBuffer,US2_RxPool,US2RxPoolLen);
	ringbuffer_init(&US2_RxRingbuffer, US2_RxRingbufPool, US2_RxRingBufferLen);
	US_TxBufferInit(&US2_TxBuffer,US2_TxPool,US2TxPoolLen);
}

void SerialCom3Init(u32 Baudrate)
{
	USART3_Init(Baudrate,US3_RxPool,US3RxPoolLen);
	US_RxBufferInit(&US3_RxBuffer,US3_RxPool,US3RxPoolLen);
	ringbuffer_init(&US3_RxRingbuffer, US3_RxRingbufPool, US3_RxRingBufferLen);
	US_TxBufferInit(&US3_TxBuffer,US3_TxPool,US3TxPoolLen);
}

/****************************************************************************/
/**********************************   ***************************************/
/********************************* ^ ^ **************************************/
/*******************************{   |   }************************************/
/*********************************  =  **************************************/
/**********************************   ***************************************/
/****************************************************************************/
/****************************************************************************/
/************************Process US1******************/
void US1DMA_RxIdle_Callback(void)
{
    //receive a frame in the rx buffer
    US1_RxBuffer.DataLength = US1_RxBuffer.Size - DMA_GetCurrDataCounter(US1_DMA_Rx);
	
	if(US1_RxBuffer.DataLength)
	{
		ringbuffer_put_force(&US1_RxRingbuffer, US1_RxBuffer.Ptr, US1_RxBuffer.DataLength);
		US1_RxBuffer.DataLength = 0;
	}
	
	//reset the  DMA Rx configration
	US1_DMA_Rx->CNDTR = (uint16_t)US1_RxBuffer.Size;
	US1_DMA_Rx->CMAR = (uint32_t)US1_RxBuffer.Ptr;
    DMA_Cmd(US1_DMA_Rx, ENABLE);
}

void US1DMA_TxCplt_Callback(void)
{
	US1_TxBuffer.DataLength = 0;
    US1_TxBuffer.DMASending = false;
}

u16 US1_DMA_Send(u8 *TxBuffer, u16 Length)
{
//	u16 i = 0;
	
	if(Length && TxBuffer != NULL)
	{
//		while(US1_TxBuffer.DMASending)//check if US1 Tx Buffer is empty
//		{
//			delay_us(100);
//			if(i++ > 100)//Max delay 10ms
//			{   //Time out, clear the Tx buffer
//				US1_TxBuffer.DMASending = false;
//				DMA_ClearFlag(US1_DMA_TxGL_FLAG);
//				DMA_Cmd(US1_DMA_Tx, DISABLE);
//				break;
//			}			
//		}
		
		if(Length > US1_TxBuffer.Size)//the length is over size
			Length = US1_TxBuffer.Size;
		
		memcpy(US1_TxBuffer.Ptr, TxBuffer, Length);
		US1_TxBuffer.DataLength = Length;
		
		US1_DMA_Tx->CNDTR = (uint16_t)US1_TxBuffer.DataLength;
		US1_DMA_Tx->CMAR = (uint32_t)US1_TxBuffer.Ptr;
		DMA_Cmd(US1_DMA_Tx, ENABLE);
		US1_TxBuffer.DMASending = true;
		
		return US1_TxBuffer.DataLength;
	}
	
	else
		return 0;
}
/*****************End of US1 Processing******************/

/************************Process US2******************/
void US2DMA_RxIdle_Callback(void)
{
    //receive a frame in the rx buffer
    US2_RxBuffer.DataLength = US2_RxBuffer.Size - DMA_GetCurrDataCounter(US2_DMA_Rx);
	
	if(US2_RxBuffer.DataLength)
	{
		ringbuffer_put_force(&US2_RxRingbuffer, US2_RxBuffer.Ptr, US2_RxBuffer.DataLength);
		US2_RxBuffer.DataLength = 0;
	}
	
	//reset the  DMA Rx configration
	US2_DMA_Rx->CNDTR = (uint16_t)US2_RxBuffer.Size;
	US2_DMA_Rx->CMAR = (uint32_t)US2_RxBuffer.Ptr;
    DMA_Cmd(US2_DMA_Rx, ENABLE);
}

void US2DMA_TxCplt_Callback(void)
{
	US2_TxBuffer.DataLength = 0;
    US2_TxBuffer.DMASending = false;
}

u16 US2_DMA_Send(u8 *TxBuffer, u16 Length)
{
//	u16 i = 0;
	
	if(Length && TxBuffer != NULL)
	{
//		while(US2_TxBuffer.DMASending)//check if US2 Tx Buffer is empty
//		{	
//			if(i++ > 2000)//Max delay 200ms
//			{   //Time out, clear the Tx buffer
//				US2_TxBuffer.DMASending = false;
//				DMA_ClearFlag(US2_DMA_TxGL_FLAG);
//				DMA_Cmd(US2_DMA_Tx, DISABLE);
//				break;
//			}
//			delay_us(100);
//		}
		
		if(Length > US2_TxBuffer.Size)//the length is over size
			Length = US2_TxBuffer.Size;
		
		memcpy(US2_TxBuffer.Ptr, TxBuffer, Length);
		US2_TxBuffer.DataLength = Length;
		
		US2_DMA_Tx->CNDTR = (uint16_t)US2_TxBuffer.DataLength;
		US2_DMA_Tx->CMAR = (uint32_t)US2_TxBuffer.Ptr;
		DMA_Cmd(US2_DMA_Tx, ENABLE);
		US2_TxBuffer.DMASending = true;
		
		return US2_TxBuffer.DataLength;
	}
	
	else
		return 0;
}
/*****************End of US2 Processing******************/

/************************Process US3******************/
void US3DMA_RxIdle_Callback(void)
{
    //receive a frame in the rx buffer
    US3_RxBuffer.DataLength = US3_RxBuffer.Size - DMA_GetCurrDataCounter(US3_DMA_Rx);
	
	if(US3_RxBuffer.DataLength)
	{
		ringbuffer_put_force(&US3_RxRingbuffer, US3_RxBuffer.Ptr, US3_RxBuffer.DataLength);
		US3_RxBuffer.DataLength = 0;
	}
	
	//reset the  DMA Rx configration
	US3_DMA_Rx->CNDTR = (uint16_t)US3_RxBuffer.Size;
	US3_DMA_Rx->CMAR = (uint32_t)US3_RxBuffer.Ptr;
    DMA_Cmd(US3_DMA_Rx, ENABLE);
}

void US3DMA_TxCplt_Callback(void)
{
	US3_TxBuffer.DataLength = 0;
    US3_TxBuffer.DMASending = false;
}

u16 US3_DMA_Send(u8 *TxBuffer, u16 Length)
{
//	u16 i = 0;
	
	if(Length && TxBuffer != NULL)
	{
//		while(US3_TxBuffer.DMASending)//check if US3 Tx Buffer is empty
//		{
//			delay_us(100);
//			if(i++ > 2000)//Max delay 200ms
//			{   //Time out, clear the Tx buffer
//				US3_TxBuffer.DMASending = false;
//				DMA_ClearFlag(US3_DMA_TxGL_FLAG);
//				DMA_Cmd(US3_DMA_Tx, DISABLE);
//				break;
//			}			
//		}
		
		if(Length > US3_TxBuffer.Size)//the length is over size
			Length = US3_TxBuffer.Size;
		
		memcpy(US3_TxBuffer.Ptr, TxBuffer, Length);
		US3_TxBuffer.DataLength = Length;
		
		US3_DMA_Tx->CNDTR = (uint16_t)US3_TxBuffer.DataLength;
		US3_DMA_Tx->CMAR = (uint32_t)US3_TxBuffer.Ptr;
		DMA_Cmd(US3_DMA_Tx, ENABLE);
		US3_TxBuffer.DMASending = true;
		
		return US3_TxBuffer.DataLength;
	}
	
	else
		return 0;
}
/*****************End of US3 Processing******************/
u8 US1_TxData[100] = {0};
u8 US2_TxData[100] = {0};
u8 US3_TxData[100] = {0};

//u8 US1_TxData[100] = {0};
//u8 US2_TxData[100] = {0};
//u8 US3_TxData[100] = {0};

////This function Tests the USART communition
//void USART_TestFunction(void)
//{
//	enum ringbuffer_state US1_state,US2_state,US3_state;
//	
//	u8 DataTemp[100];
//	u16 DataLength = 0;
//	u16 TxSize = 0;
//	
//	US1_state = ringbuffer_status(&US1_RxRingbuffer);
//	
//	if(US1_state != RINGBUFFER_EMPTY)//US1 Rx Ringbuffer is not empty
//	{ 	
//		DataLength = ringbuffer_data_len(&US1_RxRingbuffer);//get the data length in the ringbuffer
//		ringbuffer_get(&US1_RxRingbuffer, DataTemp , DataLength);//get the rx data from rx ringbuffer
//		memcpy(US1_TxData,DataTemp,DataLength);//copy the rx data to the user Tx data buffer
//		
//		TxSize = US1_DMA_Send(US1_TxData,DataLength);// TxSize is the true length that had been sent by dma
//		TxSize = TxSize;//just for clearing the warning
//	}
//	
//	US2_state = ringbuffer_status(&US2_RxRingbuffer);
//	
//	if(US2_state != RINGBUFFER_EMPTY)//US2 Rx Ringbuffer is not empty
//	{ 	
//		DataLength = ringbuffer_data_len(&US2_RxRingbuffer);//get the data length in the ringbuffer
//		ringbuffer_get(&US2_RxRingbuffer, DataTemp , DataLength);//get the rx data from rx ringbuffer
//		memcpy(US2_TxData,DataTemp,DataLength);//copy the rx data to the user Tx data buffer
//		
//		TxSize = US2_DMA_Send(US2_TxData,DataLength);// TxSize is the true length that had been sent by dma
//		TxSize = TxSize;//just for clearing the warning
//	}
//	
//	US3_state = ringbuffer_status(&US3_RxRingbuffer);
//	
//	if(US3_state != RINGBUFFER_EMPTY)//US3 Rx Ringbuffer is not empty
//	{ 	
//		DataLength = ringbuffer_data_len(&US3_RxRingbuffer);//get the data length in the ringbuffer
//		ringbuffer_get(&US3_RxRingbuffer, DataTemp , DataLength);//get the rx data from rx ringbuffer
//		memcpy(US3_TxData,DataTemp,DataLength);//copy the rx data to the user Tx data buffer
//		
//		TxSize = US3_DMA_Send(US3_TxData,DataLength);// TxSize is the true length that had been sent by dma
//		TxSize = TxSize;//just for clearing the warning
//	}
//}
