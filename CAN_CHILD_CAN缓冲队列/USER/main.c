#include "user.h"

void IWDG_Config(void);

CanasCanFrame send_pframe;

CanasCanFrame rec_pframe;

bool USworking = false;
bool CANworking = false;
u16 Allegro_Linearity;
u16 Voltage_2;

extern u16 Allegro_Linearity_VirtualAdd;
extern u16 Voltage_2_VirtualAdd;

int main()
{	 
	SCB->VTOR = FLASH_BASE | 0X2800;//application program start from page 10
	Allegro_Linearity = 0;
	
	delay_init(72);
	GPIO_led_Configuration();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	SaveVersionMessage();
	EE_ReadVariable(VirtAddVarTab[Allegro_Linearity_VirtualAdd - 1], &Voltage_2);
	EE_ReadVariable(VirtAddVarTab[Voltage_2_VirtualAdd - 1], &Allegro_Linearity);
	
	CAN_init();
	ADC1_Init();  
	
	Timer2_Init(10000-1,720-1);

	IWDG_Config();
	while(1)
	{
		IWDG_ReloadCounter();
		CanSend();
		CanReceive(&rec_pframe);
	}
}

void IWDG_Config(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器IWDG_PR和IWDG_RLR的写操作
		
	/* IWDG counter clock: 40KHz(LSI) / 4 = 10k */
	IWDG_SetPrescaler(IWDG_Prescaler_64);  //设置IWDG预分频值为64  625Hz
		
	/* Set counter reload value to 200 ,20ms*/
	IWDG_SetReload(1250);  //设置IWDG重装载值 2S
		
	/* Reload IWDG counter */
	IWDG_ReloadCounter();  //按照IWDG重装载寄存器的值重装载IWDG计数器
		
	/* Enable IWDG (the LSI oscillator will be enabled by hardware) */
	IWDG_Enable();	//使能IWDG
}



u16 SendValue;
float temp_value1 = 0, temp_value2 = 0;
u32 Battery_Energy = 0, i = 1, value = 0,Battery_Energy_mAh = 0;
//System Time Base : 100ms interupt
void TIM2_IRQHandler(void)
{
	//double time = 0.02;		//表示定时器定时间隔的十倍
	u16 Volt_Value = 0,Current_Value = 0;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET) 
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		
			Current_Value = GetChVoltage(ADC_Channel_4);//get ADC_channel_4 current data 
			//Current_Value = 1000;
			Volt_Value = GetChVoltage(ADC_Channel_8);		//get ADC_channel_8 voltage data 
			//Volt_Value = 1000;
			send_pframe.id = DC_SystemCurrent;
			send_pframe.dlc = 8;
			send_pframe.data[0] = This_Board_NodeID;
			send_pframe.data[1] = AS_USHORT_2;
			send_pframe.data[2] = DUS;
			send_pframe.data[3] = 0;
			//SendValue = 101;
			memcpy(&send_pframe.data[4],&Current_Value,2);
			send_pframe.data[6] = 0;
			send_pframe.data[7] = 0;
			CanSendToFifo(&send_pframe);
		
			send_pframe.id = DC_SystemVoltage;
			send_pframe.dlc = 8;
			send_pframe.data[0] = This_Board_NodeID;
			send_pframe.data[1] = AS_USHORT_2;
			send_pframe.data[2] = DUS;
			send_pframe.data[3] = 0;
			//SendValue = 102;
			memcpy(&send_pframe.data[4],&Volt_Value,2);	
			send_pframe.data[6] = 0;
			send_pframe.data[7] = 0;
			CanSendToFifo(&send_pframe);
			
			temp_value2 = temp_value2 + (Current_Value * 1000) / 3600;
			Battery_Energy_mAh = temp_value2;
			send_pframe.id = ESC3_CONSUMPTION;		//毫安时输出,三位精度
			send_pframe.dlc = 8;										
			send_pframe.data[0] = 0;
			send_pframe.data[1] = AS_ULONG;
			send_pframe.data[2] = DUS;
			send_pframe.data[3] = 0;
			memcpy(&send_pframe.data[4],&Battery_Energy_mAh,4);	
			CanSendToFifo(&send_pframe);
			
			Led1G_Toggle;
			
	}
}

