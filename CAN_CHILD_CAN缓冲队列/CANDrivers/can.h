#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"

typedef struct
{
    uint16_t idhigh;
	  uint16_t idlow;
    uint16_t maskhigh;
	  uint16_t masklow;
} CanFilterConfig;

extern void CAN_init(void);

#endif
