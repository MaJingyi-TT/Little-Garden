#ifndef BASEPROTOCOL_H
#define BASEPROTOCOL_H 1
#include "stdbool.h"



struct _BaseProtocol 
{
	char 			head1 ;   // A5 
	char 			head2 ;   //5A
	unsigned char 	frame_len ; //  length of all protocol, include head and end
	unsigned char 	ID ;		//  Command  ID
	unsigned char	subID; 		//  Sub Command ID
	char *			ProtocolData;
	char 			crc1;
	char 			crc2;
	char 			end;	 //AA
};

extern unsigned short crc16_ccitt(const char *buf, int len);

extern struct  _BaseProtocol Set_BaseProtocolData(int Protocol_ID, char * data, int dada_length);


//extern bool SendBaseProtocolToGround(struct _BaseProtocol protocol_data, bool isPriority);

//extern bool SendBaseProtocol(struct _BaseProtocol protocol_data);

//extern bool SendProtocolToGround(int protocolType, struct _BaseProtocol baseprotocol , bool isPriority);


//extern int GetPayloadFromBaseProtocol(struct _BaseProtocol   baseprotocol, char *cmd_payload);

//extern struct _BaseProtocol  GetBaseProtocolFromData(char * data);


extern int  GetCharDataFromBaseProtocol(struct _BaseProtocol  baseprotocol, char * data);


#endif

