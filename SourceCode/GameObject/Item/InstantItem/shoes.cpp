#include <iostream>
#include "shoes.h"
#include "character.h"

Shoes::Shoes(Position _position, Board& _board)
	: InstantItem(_position, _board), increaseVal(10)
{
}

Shoes::~Shoes()
{
}

void Shoes::use(Character& character)
{
	character.adjustSpeed(increaseVal);
}

void Shoes::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::DARK_RED);
	std::cout << "¢¶";
}

ItemType Shoes::getItemType()
{
	return ItemType::SHOES;
}
