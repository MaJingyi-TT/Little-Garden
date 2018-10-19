#ifndef USART1_H
#define USART1_H

#include "stm32f10x.h"
#include "stdio.h"
#include "stdbool.h"

#define US1_DMA_Tx						DMA1_Channel4
#define US1_DMA_TxTC_IT                 DMA1_IT_TC4
#define US1_DMA_TxTC_FLAG               DMA1_FLAG_TC4
#define US1_DMA_TxGL_FLAG				DMA1_FLAG_GL4
#define US1_DMA_Tx_IRQHandler           DMA1_Channel4_IRQHandler


#define US1_DMA_Rx						DMA1_Channel5
#define US1_DMA_RxTC_IT                 DMA1_IT_TC5
#define US1_DMA_RxTC_FLAG               DMA1_FLAG_TC5
#define US1_DMA_RxGL_FLAG				DMA1_FLAG_GL5
#define US1_DMA_Rx_IRQHandler           DMA1_Channel5_IRQHandler

#define US1_IRQn                        USART1_IRQn
#define US1_DMA_Tx_IRQn                 DMA1_Channel4_IRQn
#define US1_DMA_Rx_IRQn                 DMA1_Channel5_IRQn

#define Enable_US1_Periph_CLK           RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#define Enable_US1_GPIO_CLK             RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#define Enable_US1_DMA_CLK              RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 

#define US1_GPIO_Port                   GPIOA
#define US1_Tx_Pin                      GPIO_Pin_9
#define US1_Rx_Pin                      GPIO_Pin_10
		

bool USART1_Init(uint32_t BaudRate, uint8_t *RxBuffer, uint16_t RxBufferSize);
void USART1_DMA_Init(uint8_t *RxBuffer, uint16_t RxBufferSize);
void US1DMA_RxIdle_Callback(void);
void US1DMA_TxCplt_Callback(void);
void US1DMA_RxCplt_Callback(void);

#endif
