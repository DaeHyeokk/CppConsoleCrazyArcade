#include "consumeItem.h"
#include "character.h"
#include <iostream>

ConsumeItem::ConsumeItem(Position _position, Board& _board)
	: Item(_position, _board), hasPickup(false)
{
}

ItemCategory ConsumeItem::getItemCategory()
{
	return ItemCategory::CONSUME;
}

void ConsumeItem::drawCharacterHaveItem()
{
	board.setCursorCharacterHaveItemPosition();
	draw();
}

void ConsumeItem::eraseCharacterHaveItem()
{
	board.setCursorCharacterHaveItemPosition();
	std::cout << "  ";
}

void ConsumeItem::pickupAction()
{
	Item::pickupAction();
	hasPickup = true;
}