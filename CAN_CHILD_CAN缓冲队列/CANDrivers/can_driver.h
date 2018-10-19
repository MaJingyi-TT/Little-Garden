#ifndef _CAN_DRIVER_H
#define _CAN_DRIVER_H
#include "stm32f10x.h"

#define CANAS_CAN_MASK_STDID   ((uint32_t)0x000007FF)
#define CANAS_CAN_MASK_EXTID   ((uint32_t)0x1FFFFFFF)

#define CANAS_CAN_FLAG_EFF   (uint32_t)(0x80000000)  ///< Extended frame format
#define CANAS_CAN_FLAG_RTR   (uint32_t)(0x40000000)  ///< Remote transmission request

typedef enum
{
    CAN_ERRFLAG_TX_TIMEOUT         = 1,
    CAN_ERRFLAG_TX_OVERFLOW        = 2,
    CAN_ERRFLAG_RX_OVERFLOW        = 4,
    CAN_ERRFLAG_RX_LOST            = 8,
    CAN_ERRFLAG_HARDWARE           = 16,
    CAN_ERRFLAG_BUS_OFF            = 32
} CanErrorFlag;

typedef struct
{
    uint8_t data[8];
    uint32_t id;      ///< Full ID (Standard + Extended) and flags (CANAS_CAN_FLAG_*)
    uint8_t dlc;      ///< Data length code
} CanasCanFrame;

typedef struct
{
    CanasCanFrame frame;
    uint8_t iface;
} FifoEntry;

typedef struct
{
    FifoEntry* const pbuf;
    const int bufsize;
    int in;
    int out;
    int len;
} Fifo;

#define CAN_RX_QUEUE_LEN_TOTAL 100

#define CAN_TX_QUEUE_LEN_TOTAL 100

extern void CanSendToFifo(const CanasCanFrame* pframe);

extern void CanSend(void);

extern void CanReceive(CanasCanFrame* pframe);

#endif
