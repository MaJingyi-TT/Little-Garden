#ifndef USART3_H
#define USART3_H

#include "stm32f10x.h"
#include "stdio.h"
#include "stdbool.h"

#define US3_DMA_Tx						DMA1_Channel2
#define US3_DMA_TxTC_IT                 DMA1_IT_TC2
#define US3_DMA_TxTC_FLAG               DMA1_FLAG_TC2
#define US3_DMA_TxGL_FLAG				DMA1_FLAG_GL2
#define US3_DMA_Tx_IRQHandler           DMA1_Channel2_IRQHandler


#define US3_DMA_Rx						DMA1_Channel3
#define US3_DMA_RxTC_IT                 DMA1_IT_TC3
#define US3_DMA_RxTC_FLAG               DMA1_FLAG_TC3
#define US3_DMA_RxGL_FLAG				DMA1_FLAG_GL3
#define US3_DMA_Rx_IRQHandler           DMA1_Channel3_IRQHandler

#define US3_IRQn                        USART3_IRQn
#define US3_DMA_Tx_IRQn                 DMA1_Channel2_IRQn
#define US3_DMA_Rx_IRQn                 DMA1_Channel3_IRQn

#define Enable_US3_Periph_CLK           RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#define Enable_US3_GPIO_CLK             RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
#define Enable_US3_DMA_CLK              RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 

#define US3_GPIO_Port                   GPIOB
#define US3_Tx_Pin                      GPIO_Pin_10
#define US3_Rx_Pin                      GPIO_Pin_11
		

bool USART3_Init(uint32_t BaudRate, uint8_t *RxBuffer, uint16_t RxBufferSize);
void USART3_DMA_Init(uint8_t *RxBuffer, uint16_t RxBufferSize);
void US3DMA_RxIdle_Callback(void);
void US3DMA_TxCplt_Callback(void);
void US3DMA_RxCplt_Callback(void);

#endif
