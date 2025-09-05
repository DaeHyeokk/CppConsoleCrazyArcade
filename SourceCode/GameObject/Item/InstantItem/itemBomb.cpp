#include <iostream>
#include "itemBomb.h"
#include "character.h"

ItemBomb::ItemBomb(Position _position, Board& _board)
	: InstantItem(_position, _board), increaseVal(1)
{
}

ItemBomb::~ItemBomb()
{
}

void ItemBomb::use(Character& character)
{
	character.adjustBombsCount(increaseVal);
}

void ItemBomb::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::DARK_BLUE);
	std::cout << "¡Ý";
}

ItemType ItemBomb::getItemType()
{
	return ItemType::ITEMBOMB;
}



