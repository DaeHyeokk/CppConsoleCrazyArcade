#ifndef __DISTRUCTIBLEBLOCK_H_
#define __DISTRUCTIBLEBLOCK_H_

#include "block.h"
#include "item.h"

class Board;

class DistructibleBlock : public Block
{
	std::unique_ptr<Item> item;
public:
	DistructibleBlock(Position _position, Board& board);
	virtual ~DistructibleBlock();
	virtual bool isDistructible();
	virtual void explosionDamageAction() override;
	virtual void draw() override;
};

#endif