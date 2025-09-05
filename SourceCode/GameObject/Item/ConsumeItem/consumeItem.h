#ifndef __CONSUMEITEM_H_
#define __CONSUMEITEM_H_

#include "item.h"

class Character;
class Board;

class ConsumeItem : public Item
{
protected:
	bool hasPickup;

public:
	ConsumeItem(Position _position, Board& _board);
	virtual ItemCategory getItemCategory() override;
	void drawCharacterHaveItem();
	void eraseCharacterHaveItem();
	virtual void pickupAction() override;
};
#endif