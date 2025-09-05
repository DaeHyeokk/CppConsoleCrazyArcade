#ifndef __SUPER_POTION_H_
#define __SUPER_POTION_H_

#include "instantItem.h"

class Character;
class Board;

class SuperPotion : public InstantItem
{
	int increaseVal;

public:
	SuperPotion(Position _position, Board& _board);
	virtual ~SuperPotion();
	virtual void use(Character& character) override;
	virtual void draw() override;
	virtual ItemType getItemType() override;
};

#endif