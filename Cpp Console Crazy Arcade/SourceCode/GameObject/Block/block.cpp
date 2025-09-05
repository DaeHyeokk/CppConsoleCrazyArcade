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
	// 블록은 폭탄에 맞아도 아무일도 일어나지 않음
}

void Block::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::DARK_GRAY);
	std::cout << "■";
}
