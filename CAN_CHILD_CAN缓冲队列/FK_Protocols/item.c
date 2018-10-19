#include "Item.h"
#include <string.h>

extern int floatToChar(float value, float ratio, int numberofByte, char * data );

extern float CharToFloat(float ratio, int numberofByte, char * data ,bool isSigned);


int ItemtoCharData(ProtocolItem item, char * charData)
{
	int length;
	length = floatToChar(item.item_value, item.item_ratio,item.item_length,charData);
	return length;
}

ProtocolItem  CharDatatoItem(char * data, int numberofBytes, float ratio,bool isSigned)
{
	ProtocolItem item;

	item.item_length = numberofBytes;
	item.item_ratio  = ratio;
       item.item_value	 = CharToFloat(ratio, numberofBytes, data, isSigned);

	return item;
}

void Init_ItemData(ProtocolItem *item, int length, float ratio, float value)
{
	item->item_length = length;
	item->item_ratio  = ratio;
	item->item_value = value;

	return;
}

