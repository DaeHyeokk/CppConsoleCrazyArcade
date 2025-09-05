#ifndef __POWERPOTION_H_
#define __POWERPOTION_H_

#include "instantItem.h"

class Character;
class Board;

class PowerPotion : public InstantItem
{
	int increaseVal;

public:
	PowerPotion(Position _position, Board& _board);
	virtual ~PowerPotion();
	virtual void use(Character& character) override;
	virtual void draw() override;
	virtual ItemType getItemType() override;
};

#endif