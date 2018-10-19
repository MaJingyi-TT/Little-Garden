#ifndef USART2_H
#define USART2_H

#include "stm32f10x.h"
#include "stdio.h"
#include "stdbool.h"

#define US2_DMA_Tx						DMA1_Channel7
#define US2_DMA_TxTC_IT                 DMA1_IT_TC7
#define US2_DMA_TxTC_FLAG               DMA1_FLAG_TC7
#define US2_DMA_TxGL_FLAG				DMA1_FLAG_GL7
#define US2_DMA_Tx_IRQHandler           DMA1_Channel7_IRQHandler


#define US2_DMA_Rx						DMA1_Channel6
#define US2_DMA_RxTC_IT                 DMA1_IT_TC6
#define US2_DMA_RxTC_FLAG               DMA1_FLAG_TC6
#define US2_DMA_RxGL_FLAG				DMA1_FLAG_GL6
#define US2_DMA_Rx_IRQHandler           DMA1_Channel6_IRQHandler

#define US2_IRQn                        USART2_IRQn
#define US2_DMA_Tx_IRQn                 DMA1_Channel7_IRQn
#define US2_DMA_Rx_IRQn                 DMA1_Channel6_IRQn

#define Enable_US2_Periph_CLK           RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#define Enable_US2_GPIO_CLK             RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#define Enable_US2_DMA_CLK              RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 

#define US2_GPIO_Port                   GPIOA
#define US2_Tx_Pin                      GPIO_Pin_2
#define US2_Rx_Pin                      GPIO_Pin_3
		

bool USART2_Init(uint32_t BaudRate, uint8_t *RxBuffer, uint16_t RxBufferSize);
void USART2_DMA_Init(uint8_t *RxBuffer, uint16_t RxBufferSize);
void US2DMA_RxIdle_Callback(void);
void US2DMA_TxCplt_Callback(void);
void US2DMA_RxCplt_Callback(void);

#endif
