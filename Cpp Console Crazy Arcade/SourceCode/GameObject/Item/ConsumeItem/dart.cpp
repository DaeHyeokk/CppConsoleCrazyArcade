#include <iostream>
#include "dart.h"
#include "character.h"
#include "dartBullet.h"
#include "board.h"

Dart::Dart(Position _position, Board& _board) 
	: ConsumeItem(_position, _board)
{}

Dart::~Dart()
{
}

void Dart::use(Character& character)
{
	if(character.getState() == ALIVE)
		board.setObject(std::move(std::make_unique<DartBullet>(character.getPosition(), board, character.getDirection())));
}

void Dart::draw()
{
	if(!hasPickup)
		GameObject::draw();

	setTextColor(CursorColor::DARK_SKYBLUE);
	std::cout << "¡¿";
}

ItemType Dart::getItemType()
{
	return ItemType::DART;
}

