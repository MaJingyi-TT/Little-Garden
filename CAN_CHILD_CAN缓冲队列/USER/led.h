#ifndef LED_H
#define LED_H
#include "user.h"


///////////////////////LED1////////////////////////////////////
#define Led1G_On			{GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);}
#define Led1G_Off			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
#define Led1G_Toggle		GPIO_ToggleBits(GPIOB, GPIO_Pin_5)

#define Led1R_On			{GPIO_SetBits(GPIOB,GPIO_Pin_4);GPIO_ResetBits(GPIOB,GPIO_Pin_5);}
#define Led1R_Off			GPIO_ResetBits(GPIOB,GPIO_Pin_4)
#define Led1R_Toggle		GPIO_ToggleBits(GPIOB, GPIO_Pin_4)

#define Led1Y_On			{GPIO_SetBits(GPIOB,GPIO_Pin_5);GPIO_SetBits(GPIOB,GPIO_Pin_4);}
#define Led1Y_Off			{GPIO_ResetBits(GPIOB,GPIO_Pin_5);GPIO_ResetBits(GPIOB,GPIO_Pin_4);}


void GPIO_ToggleBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_led_Configuration(void);


#endif
