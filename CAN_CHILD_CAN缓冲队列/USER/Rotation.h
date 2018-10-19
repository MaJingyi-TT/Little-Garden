#ifndef _ROTATION_H
#define _ROTATION_H

#include "user.h"
#include "stm32f10x.h"

#define ROTEnableFlag_Status_Normal  0xf030//16位 1111正常          0000保留  0011类型：转速   0000：保留
#define ROTEnableFlag_Status_Zero    0xa030//16位 1010两路值为0     0000保留  0011类型：转速   0000：保留

extern u16 ROT_Flag_cnt;
extern u16 ROT_SendCnt;
extern u16 ROT1_working_cnt;
extern u16 ROT2_working_cnt;
extern char rot_SendBuffer[18];
extern struct _ROT_Data ROT_Data;
extern struct _ROT_RPM  ROT1;
extern struct _ROT_RPM  ROT2;
extern u16 ROT_cnt;

struct _ROT_Data
{
	ProtocolItem EnableFlag;
	ProtocolItem ROT_Speed1;
	ProtocolItem ROT_Speed2;
	ProtocolItem ROT_Speed3;
	ProtocolItem ROT_Speed4;
};

struct _ROT_RPM
{
	bool RiseFlag;
	bool FallFlag;
	bool WorkingFlag;
	u32  RiseTime;
	u32  FallTime;
	u32  PulseWidth;
	u32  UpdateCnt;
	u32  Width_Array[10];
	u32  Width_ArrayFilter[10];
	u32  Array_Index;
	float Width_Final;
	float Frequency;
	u32	  INT_Frequency;
	float RPM_Value;
};

void Rotation_Timer_Init(void);
void Get_Rotation_Speed(void);
void Init_ROT_RPM(struct _ROT_RPM *ROT);
void Insert_Sort(u32 *array, u16 len);
void Init_ROTData(struct _ROT_Data *ROT_Data);
void RotDataToStruct(struct _ROT_Data *ROT_Data);
void GetCharDataFromROTData(struct _ROT_Data ROT_Data,char *data);
void ProcessRotationSpeed(void);

#endif
