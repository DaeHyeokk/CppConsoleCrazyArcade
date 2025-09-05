#include <iostream>
#include "consoleCursorUtil.h"
#include "tile.h"

Tile::Tile(Position _position, Board& _board)
	:GameObject(_position, ObjectType::TILE, _board)
{
}

void Tile::explosionDamageAction()
{
	// 아무 행동도 안함
}

void Tile::draw()
{
	// 일단 공백 출력
	std::cout << "  ";
}
