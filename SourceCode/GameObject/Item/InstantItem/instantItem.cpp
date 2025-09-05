#include "instantItem.h"
#include "character.h"

InstantItem::InstantItem(Position _position, Board& _board)
	: Item(_position, _board)
{
}

InstantItem::~InstantItem()
{
}

ItemCategory InstantItem::getItemCategory()
{
	return ItemCategory::INSTANT;
}