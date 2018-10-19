#ifndef USX_PROCESS_H
#define USX_PROCESS_H

#include "user.h"

#define US1RxPoolLen 200
#define US2RxPoolLen 200
#define US3RxPoolLen 200

#define US1_RxRingBufferLen  200
#define US2_RxRingBufferLen  200
#define US3_RxRingBufferLen  200

#define US1TxPoolLen 200
#define US2TxPoolLen 200
#define US3TxPoolLen 200

struct US_RxBuffer
{
    u8 *Ptr;
    u16 Size;
    u16 DataLength;
};

struct US_TxBuffer
{
    u8 *Ptr;
    u16 Size;
    u16 DataLength;
    bool DMASending;
};

void US_RxBufferInit(struct US_RxBuffer *USx_RxData, u8 *buffer, u16 bufferSize);

void US_TxBufferInit(struct US_TxBuffer *USx_TxData, u8 *buffer, u16 bufferSize);


__inline void ResetUS_RxBuffer(struct US_RxBuffer *USx_RxBuf)
{
	memset(USx_RxBuf->Ptr,0,USx_RxBuf->Size);
    USx_RxBuf->DataLength = 0;
}

__inline void ResetUS_TxBuffer(struct US_TxBuffer *USx_TxBuf)
{
	memset(USx_TxBuf->Ptr,0,USx_TxBuf->Size);
    USx_TxBuf->DataLength = 0;
    USx_TxBuf->DMASending = false;
}

void SerialCom1Init(u32 Baudrate);
void SerialCom2Init(u32 Baudrate);
void SerialCom3Init(u32 Baudrate);

u16 US1_DMA_Send(u8 *TxBuffer, u16 Length);
u16 US2_DMA_Send(u8 *TxBuffer, u16 Length);
u16 US3_DMA_Send(u8 *TxBuffer, u16 Length);

#endif

