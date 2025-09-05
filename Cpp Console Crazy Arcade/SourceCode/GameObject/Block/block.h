#ifndef __BLOCK_H_
#define __BLOCK_H_

#include "gameObject.h"


class Block : public GameObject
{
public:
	Block(Position _position, Board& _board);
	virtual ~Block();
	virtual bool isDistructible();
	virtual void explosionDamageAction() override;
	virtual void draw() override;
};

#endif