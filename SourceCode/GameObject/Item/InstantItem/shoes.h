#ifndef __SHOES_H_
#define __SHOES_H_

#include "instantItem.h"

class Character;
class Board;

class Shoes : public InstantItem
{
	int increaseVal;
public:
	Shoes(Position _position, Board& _board);
	virtual ~Shoes();
	virtual void use(Character& character) override;
	virtual void draw() override;
	virtual ItemType getItemType() override;
};

#endif