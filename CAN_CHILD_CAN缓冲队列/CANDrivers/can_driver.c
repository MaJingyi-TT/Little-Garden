#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "can_driver.h"

static FifoEntry _buff_rx[CAN_RX_QUEUE_LEN_TOTAL];
static Fifo _fifo_rx = { _buff_rx, CAN_RX_QUEUE_LEN_TOTAL, 0, 0, 0 };

static FifoEntry _buff_tx[CAN_TX_QUEUE_LEN_TOTAL];
static Fifo _fifo_tx = { _buff_tx, CAN_TX_QUEUE_LEN_TOTAL, 0, 0, 0 };

static unsigned int _error_mask = 0;

extern void CAN_RX_Callback(void);

/* Software FIFO */

static void _fifoPush(Fifo* pfifo, const FifoEntry* pframe, bool* overflow)
{
    pfifo->pbuf[pfifo->in++] = *pframe;
    if (pfifo->in >= pfifo->bufsize)
        pfifo->in = 0;
    if (pfifo->len >= pfifo->bufsize)
    {
        *overflow = true;
        pfifo->out++;
        if (pfifo->out >= pfifo->bufsize)
            pfifo->out = 0;
    }
    else
    {
        *overflow = false;
        pfifo->len++;
    }
}

static int _fifoPop(Fifo* pfifo, FifoEntry* pframe)
{
    if (pfifo->len <= 0)
        return 0;
    pfifo->len--;
    if (pframe)
        *pframe = pfifo->pbuf[pfifo->out++];
    if (pfifo->out >= pfifo->bufsize)
        pfifo->out = 0;
    return 1;
}

static void _frameSpl2Canas(const CanRxMsg* pspl, CanasCanFrame* pcanas)
{
    memset(pcanas, 0, sizeof(*pcanas));
    memcpy(pcanas->data, pspl->Data, pspl->DLC);
    pcanas->dlc = pspl->DLC;
    if (pspl->IDE == CAN_Id_Standard)
    {
        pcanas->id = pspl->StdId & CANAS_CAN_MASK_STDID;
    }
    else
    {
        pcanas->id = pspl->ExtId & CANAS_CAN_MASK_EXTID;
        pcanas->id |= CANAS_CAN_FLAG_EFF;
    }
    if (pspl->RTR == CAN_RTR_Remote)
        pcanas->id |= CANAS_CAN_FLAG_RTR;
}

static void _frameCanas2Spl(const CanasCanFrame* pcanas, CanTxMsg* pspl)
{
    memset(pspl, 0, sizeof(*pspl));
    memcpy(pspl->Data, pcanas->data, pcanas->dlc);
    pspl->DLC = pcanas->dlc;
    if (pcanas->id & CANAS_CAN_FLAG_EFF)
    {
        pspl->ExtId = pcanas->id & CANAS_CAN_MASK_EXTID;
        pspl->IDE = CAN_Id_Extended;
    }
    else
    {
        pspl->StdId = pcanas->id & CANAS_CAN_MASK_STDID;
        pspl->IDE = CAN_Id_Standard;
    }
    pspl->RTR = (pcanas->id & CANAS_CAN_FLAG_RTR) ? CAN_RTR_Remote : CAN_RTR_Data;
}

static void _abortPendingTransmissions(void)
{
    CAN1->TSR |= CAN_TSR_ABRQ0 | CAN_TSR_ABRQ1 | CAN_TSR_ABRQ2;
}



void CanSendToFifo(const CanasCanFrame* pframe)
{
		FifoEntry fe;
		bool overflow = false;
    fe.iface = 0;   // Not used really
    fe.frame = *pframe;
		_fifoPush(&_fifo_tx, &fe, &overflow);	
		if (overflow)
    {
			  _error_mask|= CAN_ERRFLAG_TX_OVERFLOW;
        _abortPendingTransmissions();        // Objective is to transmit the newer data faster
    }
}

void CanSend(void)
{
	  CanTxMsg frame_spl;
    FifoEntry fe;
	
	if(((CAN1->TSR&CAN_TSR_TME0) == CAN_TSR_TME0)||((CAN1->TSR&CAN_TSR_TME1) == CAN_TSR_TME1)||((CAN1->TSR&CAN_TSR_TME2) == CAN_TSR_TME2))
	{
		if (_fifoPop(&_fifo_tx, &fe))
		{
			_frameCanas2Spl(&fe.frame, &frame_spl);
			CAN_Transmit(CAN1, &frame_spl);
		}
	}
}

void CanReceive(CanasCanFrame* pframe)
{
	FifoEntry fe;
	const int retval = _fifoPop(&_fifo_rx, &fe);
	if (retval > 0)
	{
		*pframe = fe.frame;
		CAN_RX_Callback();
		Led1G_Toggle;
		CANworking = true;
	}
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg frame_spl;
	FifoEntry fifo_entry;
	bool overflow = false;
	fifo_entry.iface = 0;

	if(SET == CAN_GetITStatus(CAN1,CAN_IT_FF0))
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
	}
	else if(SET == CAN_GetITStatus(CAN1,CAN_IT_FOV0))
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);
	}
	else if(SET == CAN_GetITStatus(CAN1,CAN_IT_FMP0))
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);

		if(CAN_MessagePending(CAN1, CAN_FIFO0)!=0)
		{
			CAN_Receive(CAN1, CAN_FIFO0, &frame_spl);
			_frameSpl2Canas(&frame_spl, &fifo_entry.frame);
			_fifoPush(&_fifo_rx, &fifo_entry, &overflow);
			if (overflow)
			{
				_error_mask|= CAN_ERRFLAG_RX_OVERFLOW;
			}
		}

	}
 
}

void CAN1_RX1_IRQHandler(void)
{
   CanRxMsg frame_spl;
	 FifoEntry fifo_entry;
	 bool overflow = false;
   fifo_entry.iface = 0;
	 
   if(SET == CAN_GetITStatus(CAN1,CAN_IT_FF1))
   {
        CAN_ClearITPendingBit(CAN1,CAN_IT_FF1);
   }
   else if(SET == CAN_GetITStatus(CAN1,CAN_IT_FOV1))
   {
        CAN_ClearITPendingBit(CAN1,CAN_IT_FOV1);
   }
   else if(SET == CAN_GetITStatus(CAN1,CAN_IT_FMP1))
   {
		    CAN_ClearITPendingBit(CAN1,CAN_IT_FMP1);
        
		    if(CAN_MessagePending(CAN1, CAN_FIFO1)!=0)
				{
					CAN_Receive(CAN1, CAN_FIFO0, &frame_spl);
          _frameSpl2Canas(&frame_spl, &fifo_entry.frame);
					_fifoPush(&_fifo_rx, &fifo_entry, &overflow);
					if (overflow)
					{
							_error_mask|= CAN_ERRFLAG_RX_OVERFLOW;
					}
				}
        
   } 
}

