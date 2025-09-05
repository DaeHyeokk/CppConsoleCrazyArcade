#ifndef __ITEMBOMB_H_
#define __ITEMBOMB_H_

#include "instantItem.h"

class character;
class Board;

class ItemBomb : public InstantItem
{
	int increaseVal;
public:
	ItemBomb(Position _position, Board& _board);
	virtual ~ItemBomb();
	virtual void use(Character& character) override;
	virtual void draw() override;
	virtual ItemType getItemType() override;
};

#endif