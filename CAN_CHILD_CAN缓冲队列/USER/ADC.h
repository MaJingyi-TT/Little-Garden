#ifndef __ADC_H
#define	__ADC_H
#include "user.h"

extern u16 Volt1_Value_U16INT,Volt2_Value_U16INT,Volt3_Value_U16INT,Volt4_Value_U16INT;


void ADC1_Init(void);
void GetDCVoltages(void);
extern float GetChVoltage(u8 ch);

#endif /* __ADC_H */
