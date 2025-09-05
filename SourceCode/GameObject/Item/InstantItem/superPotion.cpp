#include <iostream>
#include "superPotion.h"
#include "character.h"

SuperPotion::SuperPotion(Position _position, Board& _board)
	: InstantItem(_position, _board), increaseVal(1)
{
}

SuperPotion::~SuperPotion()
{
}

void SuperPotion::use(Character& character)
{
	character.adjustBombsCount(increaseVal);
	character.adjustPower(increaseVal);
}

void SuperPotion::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::YELLOW);
	std::cout << "ขอ";
}

ItemType SuperPotion::getItemType()
{
	return ItemType::SUPERPOTION;
}


