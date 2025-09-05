#ifndef __NEEDLE_H_
#define __NEEDLE_H_

#include "consumeItem.h"

class Character;
class Board;

class Needle : public ConsumeItem
{
public:
	Needle(Position _position, Board& _board);
	virtual ~Needle();
	virtual void use(Character& character) override;
	virtual void draw() override;
	virtual ItemType getItemType() override;
};

#endif