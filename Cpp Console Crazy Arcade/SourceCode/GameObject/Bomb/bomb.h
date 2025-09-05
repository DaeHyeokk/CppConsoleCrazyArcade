#ifndef __BOMB_H_
#define __BOMB_H_

#include "gameObject.h"
#include "flame.h"
#include "flamePool.h"
#include "character.h"
#include <vector>

class Board;

class Bomb : public GameObject
{
	Character& fromCharacter;
	int power;
	LARGE_INTEGER createTime;
	LARGE_INTEGER freq;
public:
	Bomb(Position position, Board& board, int _power, Character& _fromCharacter);
	virtual ~Bomb();
	virtual void update() override;
	virtual void draw() override;
	virtual void explosionDamageAction() override;
	int getPower();
	void createFlame(Position explosionPos, Flame* headFlame, Flame* tailFlame);
	void explode();
	std::vector<Position> getBombExplosionPositions();
	double getExplodeRemainTime();
};

#endif