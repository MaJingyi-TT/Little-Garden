#ifndef ITEM_H
#define ITEM_H 1

# include "stdbool.h"

struct  _Item
{
	 int item_length;
	 float item_ratio;
	 float item_value;
};

typedef struct _Item ProtocolItem;










extern void Init_ItemData(ProtocolItem *item, int length, float ratio, float value);
extern int ItemtoCharData(ProtocolItem item, char * charData);
extern ProtocolItem  CharDatatoItem(char * data, int numberofBytes, float ratio,bool isSigned);
#endif
