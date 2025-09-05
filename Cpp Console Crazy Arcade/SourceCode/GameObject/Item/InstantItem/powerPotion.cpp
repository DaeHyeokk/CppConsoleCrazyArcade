#include <iostream>
#include "powerPotion.h"
#include "character.h"

PowerPotion::PowerPotion(Position _position, Board& _board)
	: InstantItem(_position, board), increaseVal(1)
{
}

PowerPotion::~PowerPotion()
{
}

void PowerPotion::use(Character& character)
{
	character.adjustPower(increaseVal);
}

void PowerPotion::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::DARK_GRAY);
	std::cout << "กำ";
}

ItemType PowerPotion::getItemType()
{
	return ItemType::POWERPOTION;
}
