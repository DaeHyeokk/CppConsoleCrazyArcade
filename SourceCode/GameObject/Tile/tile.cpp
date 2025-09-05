#include <iostream>
#include "consoleCursorUtil.h"
#include "tile.h"

Tile::Tile(Position _position, Board& _board)
	:GameObject(_position, ObjectType::TILE, _board)
{
}

void Tile::explosionDamageAction()
{
	// �ƹ� �ൿ�� ����
}

void Tile::draw()
{
	// �ϴ� ���� ���
	std::cout << "  ";
}
