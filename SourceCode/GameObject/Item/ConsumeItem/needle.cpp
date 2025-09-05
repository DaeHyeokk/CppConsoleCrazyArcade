#include <iostream>
#include "needle.h"
#include "character.h"

Needle::Needle(Position _position, Board& _board) 
	: ConsumeItem(_position, _board)
{
}

Needle::~Needle()
{
}

void Needle::use(Character& character)
{
	if (character.getState() == CharacterState::CONFINE)
	{
		character.revive();
	}
}

void Needle::draw()
{
	if (!hasPickup)
		GameObject::draw();

	setTextColor(CursorColor::WHITE);
	std::cout << "¢Ö";
}

ItemType Needle::getItemType()
{
	return ItemType::NEEDLE;
}
