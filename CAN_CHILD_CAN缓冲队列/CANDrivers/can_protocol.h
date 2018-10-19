#ifndef _CAN_PROTOCOL_H
#define _CAN_PROTOCOL_H

#include "user.h"

#define This_Board_NodeID 10

#define This_Board_Hard_Revision 1
#define This_Board_software_Revision 1
#define This_Board_Idistribution 0
#define This_Board_Header_Type 0

#define Node_ENGINE_RPM 10 //RPM for piston engines
#define Node_PROPELLER_RPM 11 //propeller RPM for piston engines
#define Node_OIL_PRESSURE 12 //oil pressure
#define Node_OIL_TEMPRATURE 13 //oil temprature
#define Node_CYLINDERHEAD_TEMPRATURE 14 //cylinder temprature
#define Node_OIL_QUANTITY 15 //oil quantity
#define Node_DC_VOLTAGE 16 //dc voltage

#define Height_Obstacle_Avoiding 250
#define Front_Obstacle_Avoiding 251

#define DC_SystemVoltage	920		
#define DC_SystemCurrent  930   //allegro current
#define ESC2_CONSUMPTION  553		//dian gong WS danwei 
#define ESC3_CONSUMPTION  554   //haoanshi	mAh danwei

#define Eletric_Generator_Indication	1851
#define Engine_Turbocharge_Alarm	1852
#define Engine_Turbocharge_Warning	1853

/**
 * Data to be transferred by IDS
 */
typedef struct
{
    uint8_t hardware_revision;
    uint8_t software_revision;
    uint8_t id_distribution;
    uint8_t header_type;
} CanasSrvIdsPayload;


#endif
