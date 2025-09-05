#ifndef __DARTBULLET_H_
#define __DARTBULLET_H_

#include "gameObject.h"
#include "direction.h"

class Board;

class DartBullet : public GameObject
{
	Direction moveDirection;

public:
	DartBullet(Position _position, Board& _board, Direction _moveDirection);
	void move();
	virtual void update();
	virtual void explosionDamageAction();
	virtual void draw();
};

#endif