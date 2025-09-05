#ifndef __ITEM_H_
#define __ITEM_H_

#include "gameObject.h"
#include "itemType.h"
#include "itemCategory.h"

class Character;
class Board;

class Item : public GameObject
{
public:
	Item(Position _position, Board& _board);
	virtual ~Item();
	virtual void explosionDamageAction();
	virtual void pickupAction();
	virtual ItemCategory getItemCategory() = 0;
	virtual void use(Character& character) = 0;
	virtual ItemType getItemType() = 0;
};

#endif