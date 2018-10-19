#ifndef USER_H
#define USER_H

#include "stm32f10x.h"
#include "string.h"
#include <stdint.h>
#include "stdbool.h"
#include "stdio.h"
#include "common.h"
#include "USx_Process.h"
#include "XYringbuffer.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "Timer.h"
#include "baseprotocol.h"
#include "version.h"
#include "eeprom.h" 		
#include "can.h"
#include "can_driver.h"
#include "can_protocol.h"
#include "can_as.h"
#include "ADC.h"
#include "led.h"


extern bool USworking;
extern bool CANworking;

extern struct ringbuffer US1_RxRingbuffer;
extern struct ringbuffer US2_RxRingbuffer;
extern struct ringbuffer US3_RxRingbuffer;

#define M(a)	(u16)(a+0.5)

#endif
