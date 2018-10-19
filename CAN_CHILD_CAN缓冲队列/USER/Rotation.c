#include "user.h"

u16 ROT_cnt = 0;
u16 ROT_Flag_cnt = 0;
u16 ROT1_working_cnt = 0;
u16 ROT2_working_cnt = 0;
u16 ROT_SendCnt = 0;
bool RotStartWorking = false;
///////////////////////////////////////
struct _ROT_Data ROT_Data;
struct _ROT_Data ROT_DataToSend;
char rot_datatoSend[10]={0};
char rot_SendBuffer[18];
struct _BaseProtocol RotBaseProtocol;
u16 rot_SendBufferLength;
///////////////////////////////////////////
struct _ROT_RPM  ROT1;
struct _ROT_RPM  ROT2;


void Rotation_Timer_Init(void)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	//Timer8 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器
  
	TIM_DeInit(TIM8);

	TIM_TimeBaseStructure.TIM_Period=0xffff; //自动重装载寄存器的值
	TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1); //时钟预分频数 1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

		  //初始化TIM8输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM_ICInitStructure.TIM_ICPolarity =TIM_ICPolarity_Rising;	   //上升沿捕获
  	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //配置输入分频,不分频 
  	TIM_ICInitStructure.TIM_ICFilter = 0x0f;                        
  	TIM_ICInit(TIM8, &TIM_ICInitStructure);
		
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;  
	TIM_ICInit(TIM8, &TIM_ICInitStructure);


	/* Enable the CC1-2-3-4 Interrupt Request */
	TIM_ITConfig(TIM8, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM8, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	
	TIM_Cmd(TIM8, ENABLE); //开启时钟
}

void TIM8_CC_IRQHandler(void)
{ 
    if (TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET)   
	{	
		TIM_ClearITPendingBit(TIM8, TIM_IT_CC1); //清除中断标志位
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6) == 1) //判断是高电平
		{
			TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Falling);//设置下降沿捕获
			//////////////////////////////////////////////////////////////		
			if(!ROT1.RiseFlag)//如果还未记录上升沿
			{
				ROT1.RiseTime = TIM_GetCounter(TIM8);//记录上升沿时当前计数器的值	
				ROT1.RiseFlag = true;//标记已经记录上升沿的时间
				ROT1.FallFlag = false;//还未捕获下降沿
			}
			else
			{
				if(ROT1.FallFlag == true)
				{
					ROT1.FallTime = TIM_GetCounter(TIM8);
					
					ROT1.PulseWidth = ROT1.UpdateCnt * 65535 + ROT1.FallTime - ROT1.RiseTime;			
					
					if(ROT1.PulseWidth > 0)
					{
						if(ROT1.Array_Index < 10)
						{
							ROT1.Width_Array[ROT1.Array_Index++] = ROT1.PulseWidth;
							ROT1.WorkingFlag = true;
						}
					}

					ROT1.RiseFlag = false;
					ROT1.FallFlag = false;
					ROT1.UpdateCnt = 0;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////
		}// end of  capture rise
		else//捕获下降沿
		{
			TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Rising);//配置上升沿中断 
			ROT1.FallFlag = true;//已经捕获下降沿
		}//end of capture fall
	}
	
	 if (TIM_GetITStatus(TIM8, TIM_IT_CC2) != RESET)   
	{	
		TIM_ClearITPendingBit(TIM8, TIM_IT_CC2); //清除中断标志位
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == 1) //判断是高电平
		{
			TIM_OC2PolarityConfig(TIM8,TIM_ICPolarity_Falling);//设置下降沿捕获
			//////////////////////////////////////////////////////////////		
			if(!ROT2.RiseFlag)//如果还未记录上升沿
			{
				ROT2.RiseTime = TIM_GetCounter(TIM8);//记录上升沿时当前计数器的值	
				ROT2.RiseFlag = true;//标记已经记录上升沿的时间
				ROT2.FallFlag = false;//还未捕获下降沿
			}
			else
			{
				if(ROT2.FallFlag == true)
				{
					ROT2.FallTime = TIM_GetCounter(TIM8);
					
					ROT2.PulseWidth = ROT2.UpdateCnt * 65535 + ROT2.FallTime - ROT2.RiseTime;
					
					if(ROT2.PulseWidth > 0)
					{
						if(ROT2.Array_Index < 10)
						{
							ROT2.Width_Array[ROT2.Array_Index++] = ROT2.PulseWidth;
							ROT2.WorkingFlag = true;
						}
					}

					ROT2.RiseFlag = false;
					ROT2.FallFlag = false;
					ROT2.UpdateCnt = 0;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////
		}// end of  capture rise
		else//捕获下降沿
		{
			TIM_OC2PolarityConfig(TIM8,TIM_ICPolarity_Rising);//配置上升沿中断 
			ROT2.FallFlag = true;//已经捕获下降沿
		}//end of capture fall
	}
	
	if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
		if(ROT1.RiseFlag == true)
			ROT1.UpdateCnt++;
		
		if(ROT2.RiseFlag == true)
			ROT2.UpdateCnt++;
	}	
		
}

void Insert_Sort(u32 *array, u16 len)
{
	int i = 0, j=0;
	u32 key = 0;
	
	for(i=1;i<len;i++)
	{
		key = array[i];
		j = i-1;
		while(j >= 0)
		{
			if(array[j] > key)
			{
				array[j+1] = array[j];
				array[j] = key;
			}
			j -= 1;
		}
	}
}

void Process_PulseArray(struct _ROT_RPM *ROT)
{
	u16 i=0;
	float sum = 0;
	for(i=0;i<10;i++)
	{
		ROT->Width_ArrayFilter[i] = ROT->Width_Array[i];
	}
	ROT->Array_Index = 0;
	
	Insert_Sort(ROT->Width_ArrayFilter,10);
	
	sum = ROT->Width_ArrayFilter[4] + ROT->Width_ArrayFilter[5];
	
	ROT->Width_Final = sum/2;
//	ROT->Width_Final = M(ROT->Width_Final);
	
	if(ROT->Width_Final > 0)
	{
		ROT->Frequency = 1000000/ROT->Width_Final;
	}
	else
		ROT->Frequency = 0;

//	ROT->INT_Frequency = M(ROT->Frequency);
//	ROT->Frequency = 0;
//	ROT->RPM_Value = ROT->INT_Frequency * 60;
}

void Get_Rotation_Speed(void)
{
	if(ROT1.Array_Index>=10)
	{
		Process_PulseArray(&ROT1);
		RotStartWorking = true;
	}
	if(ROT2.Array_Index>=10)
	{
		Process_PulseArray(&ROT2);
		RotStartWorking = true;
	}
}

void Init_ROT_RPM(struct _ROT_RPM *ROT)
{
	ROT->RiseFlag = false;
	ROT->FallFlag = false;
	ROT->WorkingFlag = false;
	ROT->RiseTime = 0;
	ROT->FallTime = 0;
	ROT->PulseWidth = 0;
	ROT->UpdateCnt = 0;
	ROT->Array_Index = 0;
	ROT->Width_Final = 0;
	ROT->Frequency = 0;
	ROT->INT_Frequency = 0;
	ROT->RPM_Value = 0;
}

void Init_ROTData(struct _ROT_Data *ROT_Data)
{
	Init_ItemData(&(ROT_Data->EnableFlag), 2, 1,ROTEnableFlag_Status_Zero);
	Init_ItemData(&(ROT_Data->ROT_Speed1), 4, 1000, 0);
	Init_ItemData(&(ROT_Data->ROT_Speed2), 4, 1000, 0);
//	Init_ItemData(&(ROT_Data->ROT_Speed3), 4, 1000, 0);
//	Init_ItemData(&(ROT_Data->ROT_Speed4), 4, 1000, 0);
	Init_ROT_RPM(&ROT1);
	Init_ROT_RPM(&ROT2);
}

void RotDataToStruct(struct _ROT_Data *ROT_Data)
{
	ROT_Data->ROT_Speed1.item_value = ROT1.Frequency;
	ROT_Data->ROT_Speed2.item_value = ROT2.Frequency;
//	ROT_Data->ROT_Speed1.item_value = ROT1.INT_Frequency;
//	ROT_Data->ROT_Speed2.item_value = ROT2.INT_Frequency;

	if(ROT1.Frequency > 0 || ROT2.Frequency > 0)
	{
		ROT_Data->EnableFlag.item_value = ROTEnableFlag_Status_Normal;
	}
	else
	{
		ROT_Data->EnableFlag.item_value = ROTEnableFlag_Status_Zero;
	}
}

void GetCharDataFromROTData(struct _ROT_Data ROT_Data,char *data)
{
	int offset = 0;

	offset += ItemtoCharData(ROT_Data.EnableFlag, data + offset);
	offset += ItemtoCharData(ROT_Data.ROT_Speed1, data + offset);
	offset += ItemtoCharData(ROT_Data.ROT_Speed2, data + offset);
//	offset += ItemtoCharData(ROT_Data.ROT_Speed3, data + offset);
//	offset += ItemtoCharData(ROT_Data.ROT_Speed4, data + offset);
}

void ProcessRotationSpeed(void)
{
	u16 i=0;
	
	Get_Rotation_Speed();
	
	if(ROT_cnt>= 10)//100ms
	{	
		RotDataToStruct(&ROT_Data);
		GetCharDataFromROTData(ROT_Data,rot_datatoSend);
		RotBaseProtocol = Set_BaseProtocolData(0xf90e, rot_datatoSend, 10);
		rot_SendBufferLength = GetCharDataFromBaseProtocol(RotBaseProtocol, rot_SendBuffer);
		
		if(RotStartWorking == true)
		{
			while(US2_Queue.DMA_Sending)//wait for US2 DMA finish sending
			{
				delay_us(100);
				if(i++ > 1000)
				break;
			}
			for(i=0;i<18;i++)
			{
				USART_SendChar(USART2, rot_SendBuffer[i]);
			}
		}
		
		ROT_cnt = 0;
	}
	
}
