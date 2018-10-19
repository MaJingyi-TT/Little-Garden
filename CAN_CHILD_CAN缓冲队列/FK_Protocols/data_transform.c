#include "stdbool.h"

int intToChar(int value, float ratio, int numberofByte, char * data )
{
	int temp, length;

	char  charValue;
	
	temp = value * ratio;
	if(numberofByte ==1)
	{
		length = 1;
		charValue = (char)temp;
		*data = charValue;
	}
	else if(numberofByte ==2)
	{
		length = 2;
		charValue = (char)temp;
		*data = charValue;
		charValue = (char) (temp >>8);
		*(data+1) = charValue;
	}
	else if(numberofByte ==4)
	{
		length = 4;
		charValue = (char)temp;
		*data = charValue;
		charValue = (char) (temp >>8);
		*(data+1) = charValue;
		charValue = (char) (temp >>16);
		*(data+2) = charValue;
		charValue = (char) (temp >>24);
		*(data+3) = charValue;
	}
	return length;
}


int CharToInt(float ratio, int numberofByte, char * data )
{
	int temp;
	int   charValue;

	if(numberofByte ==1)
	{

		charValue = *data;
		temp = charValue / ratio;
	}
	else if(numberofByte ==2)
	{

		charValue = *data;
		temp = charValue ;

		charValue = (*(data+1)) <<8;
		temp = temp + charValue;

		temp = temp/ratio;
	}
	else if(numberofByte ==4)
	{
		charValue = *data;
		temp = charValue ;

		charValue = (*(data+1)) <<8;
		temp = temp + charValue;

	
		charValue = (*(data+2)) <<16;
		temp = temp + charValue;
	
		charValue = (*(data+3)) <<24;
		temp = temp + charValue;
		
		temp = temp/ratio;
	}
	return temp;
}


int floatToChar(float value, float ratio, int numberofByte, char * data )
{
	int temp, length;

	char  charValue;
	temp = value * ratio;
	if(numberofByte ==1)
	{
		length = 1;
		charValue = (char)temp;
		*data = charValue;
	}
	else if(numberofByte ==2)
	{
		length = 2;
		charValue = (char)temp;
		*data = charValue;
		charValue = (char) (temp >>8);
		*(data+1) = charValue;
	}
	else if(numberofByte ==4)
	{
		length = 4;
		charValue = (char)temp;
		*data = charValue;
		charValue = (char) (temp >>8);
		*(data+1) = charValue;
		charValue = (char) (temp >>16);
		*(data+2) = charValue;
		charValue = (char) (temp >>24);
		*(data+3) = charValue;
	}
	return length;
}

float CharToFloat(float ratio, int numberofByte, char * data ,bool isSigned)
{

	float temp;
	char   charValue;
       short int TwoByteValue;
       int FourByteValue;

	if(numberofByte ==1)
	{
		charValue = *data;
        if(isSigned == true)
        {
		    temp = (signed char)charValue;
        }
        else
        {
            temp = (unsigned char)charValue;
        }
        temp = temp/ratio;
	}
	else if(numberofByte ==2)
	{
		charValue = *data;
		TwoByteValue = charValue ;
             TwoByteValue &= 0x00FF;

		TwoByteValue |= (*(data+1)) <<8;
        
        if(isSigned == true)
        {
		    temp = (signed short int)TwoByteValue;
        }
        else
        {
            temp = (unsigned short int)TwoByteValue;
        }
		temp = temp/ratio;
	}
	else if(numberofByte ==4)
	{
        FourByteValue = 0;
        FourByteValue |= *data;
        FourByteValue |= (*(data+1)) <<8;    	
        FourByteValue |= (*(data+2)) <<16;
        FourByteValue |= (*(data+3)) <<24;

        if(isSigned == true)
        {
		    temp = (signed int)FourByteValue;
        }
        else
        {
            temp = (unsigned int)FourByteValue;
        }
		
		temp = temp/ratio;
	}
	return temp;
}

