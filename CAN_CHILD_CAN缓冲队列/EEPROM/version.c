#include "version.h"
#include "string.h"
#define InitVar(x)	x,x,x,x,x,x,x,x,x,x
#define VersionMaxLength	50
//Any time a new version is released, Do remember to Update
//the Version Information below!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
const char version[VersionMaxLength] = "XY: Allegro_Current_Sensor_V1.0.0";
//V1.0.0  creat a new project


//NumbOfVar is 200
//the first 100 virtual address save the application parameters
//from 101-200 save the version information
#define VInfoStartAddIndex		100
uint16_t VirtAddVarTab[NumbOfVar] = {InitVar(InitVar(1)),InitVar(InitVar(1))};
//the VirtAddVarTab is none zero initialized, 
//so the tab will be compiled into the flash,instead of sram
u16 Allegro_Linearity_VirtualAdd;
u16 Voltage_2_VirtualAdd;

static void InitVirAdd(void)
{
	u16 i = 0, var = 1;
	
	for(i=0;i<NumbOfVar;i++)
	{
		//from 1 to 200
		VirtAddVarTab[i] = var;
		var++;
	}
}
	
void SaveVersionMessage(void)
{
	u16 Numofbytes,i;
	u16 *Version2Bytes;
	char Versiontemp[VersionMaxLength] = {0};
	
	//init the VirtAddVarTab
	InitVirAdd();
	
	Numofbytes = strlen(version) + 1;//number of bytes include the '/0'
	
	strcpy(Versiontemp,version);
	
	FLASH_Unlock();
	EE_Init();
	//save the version string length
	EE_WriteVariable(VirtAddVarTab[VInfoStartAddIndex],Numofbytes);
	
	if(Numofbytes % 2 != 0)//odd
	{
		Version2Bytes = (u16 *)Versiontemp;
		//save the verison sting
		for(i=0;i<(Numofbytes+1)/2;i++)
		{
			EE_WriteVariable(VirtAddVarTab[VInfoStartAddIndex+1 + i],Version2Bytes[i]);
		}
		Allegro_Linearity_VirtualAdd = VInfoStartAddIndex + 33;
		Voltage_2_VirtualAdd = VInfoStartAddIndex + 1 + i + 2;
	}
	else//even
	{
		Version2Bytes = (u16 *)Versiontemp;
		//save the verison sting
		for(i=0;i<Numofbytes/2;i++)
		{
			EE_WriteVariable(VirtAddVarTab[VInfoStartAddIndex+1 + i],Version2Bytes[i]);
		}
		Allegro_Linearity_VirtualAdd = VirtAddVarTab[VInfoStartAddIndex - 1];
		Voltage_2_VirtualAdd = VirtAddVarTab[VInfoStartAddIndex - 2];
	}
	FLASH_Lock();
}

