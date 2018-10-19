#include "common.h"


static u8  fac_us=0;
static u16 fac_ms=0;


void delay_init(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//HCLK/8
	fac_us=SYSCLK/8;        
	fac_ms=(u16)fac_us*1000;
} 

void delay_us(u32 nus)
{   
	u32 temp; 
	
	SysTick->LOAD=nus*fac_us;     
	SysTick->VAL=0x00;     
	SysTick->CTRL=0x01 ;     
	do
	{
	temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;        
}

void delay_ms(u16 nms)
{           
	u32 temp;
	
	SysTick->LOAD=(u32)nms*fac_ms;//(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           
	SysTick->CTRL=0x01 ;          
	do
	{
	temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;              
}  

void Serial_PutString(USART_TypeDef* USARTx, char *s)
{
	u16 count = 0;
	
	while (*s != '\0')
	{
		USART_SendData(USARTx, *s);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
		{
		  if(count++ > 5000)
			  break;
		}
		s++;
	}
}

void USART_SendChar(USART_TypeDef* USARTx, char data)
{
    u16 count = 0;

    USART_SendData(USARTx, data);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
    {
        if(count++ > 5000)
          break;
    }
}




