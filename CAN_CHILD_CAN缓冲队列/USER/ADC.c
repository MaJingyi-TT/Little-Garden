/******************** (C) COPYRIGHT 2016 XY Team ***************************

**********************************************************************************/
#include "ADC.h"
#include "user.h"
float GetChVoltage(u8 ch);



const uint8_t ADC_CH4 = ADC_Channel_4,ADC_CH8 = ADC_Channel_8;


//ResRatio = (R1+R2)/R2
//const float CH1_ResRatio = 6.118f,CH2_ResRatio = 6.118f,CH3_ResRatio = 6.118f,CH4_ResRatio = 6.118f;

extern u16 Allegro_Linearity,Voltage_2;//Allegro_Linearity 这个值需要标定，后期存在片内flash
float Volt1_Value = 0,Volt2_Value = 0,Volt3_Value = 0,Volt4_Value = 0;

u16 Volt1_Value_U16INT,Volt2_Value_U16INT,Volt3_Value_U16INT,Volt4_Value_U16INT;


static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

static void ADC1_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	ADC_DeInit(ADC1);
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfChannel = 1;

	ADC_Init(ADC1, &ADC_InitStruct); 
	ADC_Cmd(ADC1, ENABLE); 
	ADC_ResetCalibration(ADC1); 
	while(ADC_GetResetCalibrationStatus(ADC1))
		; 
	ADC_StartCalibration(ADC1); 
	while(ADC_GetCalibrationStatus(ADC1))
		;

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}


static u16 Get_Adc(u8 ch)   
{	unsigned int result = 0;
	unsigned char i;

	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	
  	for(i=0;i<8;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
		result+=ADC_GetConversionValue(ADC1);
	}
	result=result/8;	 
	return result;	
}

float GetChVoltage(u8 ch)
{
	u16 Rt_value = 0;
	float AdcValue = 0;
	float realValue = 0;
	float allegro = (float)Allegro_Linearity/(float)10000;
	float Vol_ratio = 25.9;
	float voltage_2 = (float)Voltage_2/(float)100;
	switch(ch)
	{
		case ADC_CH4:
			AdcValue = Get_Adc(ADC_CH4);
			realValue = (((float)AdcValue/4096)*330) - voltage_2;//1.607这个数值需要标定，后期存在片内flash
			if(realValue < 0)
			{
				realValue = -realValue;
			}
			Rt_value = realValue / allegro;
			//Rt_value = AdcValue;
			break;
		
		case ADC_CH8:
			AdcValue = Get_Adc(ADC_CH8);
			realValue = ((float)AdcValue/4096)*330;
			Rt_value = realValue * Vol_ratio;
		break;
		default:
			break;
	}
	return Rt_value;//电压精度和电流精度均是小数点后两位	
}


