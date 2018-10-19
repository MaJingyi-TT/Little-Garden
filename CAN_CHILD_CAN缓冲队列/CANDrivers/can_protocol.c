#include "can_protocol.h"

extern void CAN_RX_Callback(void);

extern CanasCanFrame send_pframe;
extern CanasCanFrame rec_pframe;
CanasSrvIdsPayload payload;

void CAN_RX_Callback(void)
{
	switch(rec_pframe.id)
	{
		case IDS:
			if((rec_pframe.data[0]==This_Board_NodeID)&&(rec_pframe.data[2]==IDS))
			{
			   send_pframe.id=IDS;
				 send_pframe.dlc=8;
				 send_pframe.data[0]=This_Board_NodeID;
				 send_pframe.data[1]=AS_UCHAR;
				 send_pframe.data[2]=IDS;
				 send_pframe.data[3]=0;
				 send_pframe.data[4]=This_Board_Hard_Revision;
				 send_pframe.data[5]=This_Board_software_Revision;
				 send_pframe.data[6]=This_Board_Idistribution;
				 send_pframe.data[7]=This_Board_Header_Type;

				 CanSendToFifo(&send_pframe);
			}
		
			break;
		
		default:
			break;
	}

}
