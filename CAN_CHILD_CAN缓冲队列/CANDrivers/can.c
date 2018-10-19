#include "can.h"

#define GPIO_CAN                   GPIOA
#define RCC_APB2Periph_GPIO_CAN    RCC_APB2Periph_GPIOA
#define GPIO_Pin_RX                GPIO_Pin_11
#define GPIO_Pin_TX                GPIO_Pin_12

#define Filter0_Idhigh    0x0000
#define Filter0_Idlow     0x0000
#define Filter0_Maskhigh  0x0000
#define Filter0_Masklow   0x0006

#define Filter1_Idhigh    0x0000
#define Filter1_Idlow     0x0000
#define Filter1_Maskhigh  0x0000
#define Filter1_Masklow   0x0006

static void CAN_config_init(void)
{
	CAN_InitTypeDef  CAN_InitStructure;

	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	// 36M 1M
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=ENABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW=CAN_SJW_3tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler=2;
	CAN_Init(CAN1, &CAN_InitStructure);

}

static void CAN_filter_init(u8 filt_index,u8 filter_mode,u8 fifo,u16 Idhigh,u16 Idlow,u16 Maskhigh,u16 Masklow)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	CAN_FilterInitStructure.CAN_FilterNumber=filt_index;
	CAN_FilterInitStructure.CAN_FilterMode=filter_mode;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=Idhigh;
	CAN_FilterInitStructure.CAN_FilterIdLow=Idlow;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=Maskhigh;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=Masklow;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=fifo;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure); 
}
 
void CAN_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIO_CAN, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);

	CAN_config_init();

	CAN_filter_init(0,CAN_FilterMode_IdMask,CAN_FIFO0,Filter0_Idhigh,Filter0_Idlow,Filter0_Maskhigh,Filter0_Masklow);
	CAN_filter_init(1,CAN_FilterMode_IdMask,CAN_FIFO1,Filter1_Idhigh,Filter1_Idlow,Filter1_Maskhigh,Filter1_Masklow);

	CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0, ENABLE);
	CAN_ITConfig(CAN1, CAN_IT_FMP1 | CAN_IT_FF1 | CAN_IT_FOV1, ENABLE);
}

