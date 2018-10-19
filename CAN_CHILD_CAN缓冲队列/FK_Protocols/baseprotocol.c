#include "baseprotocol.h"
#include "Item.h"
#include <string.h>


static const unsigned short crc16tab[256]= {
			0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
			0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
			0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
			0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
			0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
			0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
			0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
			0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
			0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
			0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
			0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
			0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
			0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
			0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
			0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
			0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
			0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
			0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
			0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
			0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
			0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
			0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
			0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
			0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
			0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
			0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
			0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
			0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
			0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
			0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
			0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
			0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};
unsigned short crc16_ccitt(const char *buf, int len)
{
			int counter;
			unsigned short crc = 0;
			for( counter = 0; counter < len; counter++)
				crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf++)&0x00FF];
			return crc;
}



struct  _BaseProtocol   Set_BaseProtocolData(int Protocol_ID, char * data, int dada_length)
{
 	struct _BaseProtocol BaseProtocol;
	short int crc_data;
	char temp[100];
	
	BaseProtocol.head1 = 0xA5;
	BaseProtocol.head2 = 0x5A;
	BaseProtocol.frame_len = dada_length + 8;
	BaseProtocol.ID		= (char)(Protocol_ID>>8);
	BaseProtocol.subID 	= (char)(Protocol_ID&0x00FF);
	
	BaseProtocol.ProtocolData = data;
	//memcpy((char*)&(BaseProtocol.ProtocolData), data, dada_length);
	
	memcpy(temp, &(BaseProtocol.frame_len),1);
	memcpy(temp + 1, &(BaseProtocol.ID), 1);
	memcpy(temp + 2, &(BaseProtocol.subID), 1);
	memcpy(temp + 3, BaseProtocol.ProtocolData, dada_length );
	
	
	crc_data = crc16_ccitt(temp, dada_length+3);
	
	BaseProtocol.crc1 =  crc_data & 0xFF;
	BaseProtocol.crc2 = (crc_data >> 8) & 0xFF;
	BaseProtocol.end  = 0xAA;

	return  BaseProtocol;
}

int GetCharDataFromBaseProtocol(struct _BaseProtocol   baseprotocol, char * data)
{
   memcpy(data, &baseprotocol, 5);
   memcpy(data + 5, baseprotocol.ProtocolData, baseprotocol.frame_len - 8);
   memcpy(data + baseprotocol.frame_len - 3, &(baseprotocol.crc1), 3);
   
   return baseprotocol.frame_len;
}

/*
bool SendBaseProtocol(struct _BaseProtocol protocol_data)
{
  // char sendData[MaximumDataLength];
  bool result = false;
  
   char sendData[256];
   memcpy(sendData   	,&protocol_data, 5);
   memcpy(sendData +5  	,protocol_data.ProtocolData, protocol_data.frame_len - 8);
   memcpy(sendData + protocol_data.frame_len - 3, (char*)&protocol_data.crc1,3);

   AT91F_US_SendFrame((AT91PS_USART) AT91C_BASE_US2, sendData,protocol_data.frame_len ,0,0);
   return result;
}
  
bool SendBaseProtocolToGround(struct _BaseProtocol protocol_data, bool isPriority)
{
  // char sendData[MaximumDataLength];
  bool result = false;
  
   char sendData[256];
   memcpy(sendData   	,&protocol_data, 5);
   memcpy(sendData +5  	,protocol_data.ProtocolData, protocol_data.frame_len - 8);
   memcpy(sendData + protocol_data.frame_len - 3, (char*)&protocol_data.crc1,3);
   
   result = SendDataToGround(sendData, protocol_data.frame_len, isPriority);
   return result;
}

bool SendProtocolToGround(int protocolType, struct _BaseProtocol baseprotocol , bool isPriority)
{
	bool result;
	struct  _EB90SendProtocol eb90Protocol;
	
	if(protocolType == EB90Protocol)
		{
			eb90Protocol = Set_EB90SendProtocolData(baseprotocol,false);
			result = SendEB90ProtocolToGround(eb90Protocol,isPriority);
		}
	else if(protocolType== B5Protocol)
		{
		    result = SendBaseProtocolToGround(baseprotocol,isPriority);
		}
	else
		{
			
			result = SendBaseProtocolToGround(baseprotocol,isPriority);
		}

	return result;
}
*/
//struct _BaseProtocol   GetBaseProtocolFromData(char * data)
//{
//  struct _BaseProtocol baseprotocol;
//  short int crc_data;

//  baseprotocol.head1 			= data[0];
//  baseprotocol.head2 			= data[1];
//  baseprotocol.frame_len 		= data[2];
//  baseprotocol.ID				= data[3];
//  baseprotocol.subID			= data[4];
//  
//  baseprotocol.ProtocolData		= data + 5;
//  
//  baseprotocol.crc1 			= data[ baseprotocol.frame_len-3]; 	
//  baseprotocol.crc2 			= data[ baseprotocol.frame_len-2]; 	 
//  baseprotocol.end 				= data[ baseprotocol.frame_len-1]; 		

// 

//  if(baseprotocol.head1 == 0xA5 && baseprotocol.head2 == 0x5A && baseprotocol.end == 0xAA)
//  	{
//  	   crc_data = crc16_ccitt(&(data[2]) ,baseprotocol.frame_len - 5);
//	   if((baseprotocol.crc1 == (crc_data & 0xFF) ) && (baseprotocol.crc2 == ((crc_data>>8) & 0xFF) ))
//	   	{
//			//return  baseprotocol;
//	   	}
//	   else
//	   	{
//	   		//return  (struct _BaseProtocol)NULL;
//	   		baseprotocol.ProtocolData = NULL;
//	   	}
//  	}

//  	   return baseprotocol;
// 
//}




//int GetPayloadFromBaseProtocol(struct _BaseProtocol   baseprotocol, char *cmd_payload)
//{
//   //memcpy(data, &baseprotocol, 5);
//   memcpy(cmd_payload, baseprotocol.ProtocolData, baseprotocol.frame_len - 8);
//   //memcpy(data + baseprotocol.frame_len - 3, &(baseprotocol.crc1), 3);
//   
//   return (baseprotocol.frame_len - 8);
//}
