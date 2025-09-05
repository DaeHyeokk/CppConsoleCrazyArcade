#ifndef __INSTANTITEM_H_
#define __INSTANTITEM_H_

#include "item.h"

class Character;
class Board;

class InstantItem : public Item
{
public:
	InstantItem(Position _position, Board& _board);
	virtual ~InstantItem();
	virtual ItemCategory getItemCategory() override;
};

#endif