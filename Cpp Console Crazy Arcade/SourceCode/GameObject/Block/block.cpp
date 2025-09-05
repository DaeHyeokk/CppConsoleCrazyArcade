#include "block.h"
#include "board.h"
#include <iostream>

Block::Block(Position _position, Board& _board)
	: GameObject(_position, ObjectType::BLOCK, _board)
{
}

Block::~Block()
{
}

bool Block::isDistructible()
{
	return false;
}

void Block::explosionDamageAction()
{
	// ����� ��ź�� �¾Ƶ� �ƹ��ϵ� �Ͼ�� ����
}

void Block::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::DARK_GRAY);
	std::cout << "��";
}
