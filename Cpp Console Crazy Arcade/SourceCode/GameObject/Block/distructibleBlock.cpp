#include <iostream>
#include <memory>
#include "randomUtil.h"
#include "distructibleBlock.h"
#include "itemType.h"
#include "board.h"
#include "dart.h"
#include "needle.h"
#include "itemBomb.h"
#include "powerPotion.h"
#include "superPotion.h"
#include "shoes.h"

using namespace std;

DistructibleBlock::DistructibleBlock(Position _position, Board& _board)
	: Block(_position, _board)
{
	// 랜덤 아이템을 생성하고 item 멤버변수에 대입.
	int randomInt = getRandomInt(0, ItemType::COUNT - 1);

	switch (randomInt)
	{
	case ItemType::DART:
		item = make_unique<Dart>(position, board);
		break;

	case ItemType::NEEDLE:
		item = make_unique<Needle>(position, board);
		break;

	case ItemType::ITEMBOMB:
		item = make_unique<ItemBomb>(position, board);
		break;

	case ItemType::POWERPOTION:
		item = make_unique<PowerPotion>(position, board);
		break;

	case ItemType::SHOES:
		item = make_unique<Shoes>(position, board);
		break;

	case ItemType::SUPERPOTION:
		item = make_unique<SuperPotion>(position, board);
		break;

	default:
		item = nullptr;
		break;
	}
}

DistructibleBlock::~DistructibleBlock()
{
}

bool DistructibleBlock::isDistructible()
{
	return true;
}

void DistructibleBlock::explosionDamageAction()
{
	board.setObject(move(item));
	setActive(false);
}

void DistructibleBlock::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::VIOLET);
	cout << "□";
}

