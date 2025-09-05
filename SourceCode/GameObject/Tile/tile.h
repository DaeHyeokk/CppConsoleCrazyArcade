#ifndef __TILE_H_
#define __TILE_H_

#include "gameObject.h"

class Board;

class Tile : public GameObject
{
public:
	Tile(Position _position, Board& _board);
	virtual void explosionDamageAction();
	virtual void draw();
};

#endif