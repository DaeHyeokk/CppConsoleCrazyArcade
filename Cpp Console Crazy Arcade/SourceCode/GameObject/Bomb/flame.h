#ifndef __FLAME_H_
#define __FLAME_H_

#include "gameObject.h"
#include "bomb.h"
#include "Windows.h"
class Board;
class FlamePool;

class Flame : public GameObject
{
	Flame* tail;
	bool isHead;
	std::shared_ptr<FlamePool> flamePool;
	LARGE_INTEGER flameCreateTime;
	LARGE_INTEGER freq;
	
public:
	Flame(Position _position, Board& _board, std::shared_ptr<FlamePool> _flamePool);
	virtual ~Flame();
	void init(Position position);
	void setTail(Flame* tailFlame);
	void setHeadFlame();
	void hideFlame();
	virtual void update();
	virtual void draw();
	virtual void explosionDamageAction();
	virtual void returnObject(std::unique_ptr<GameObject> gameObj);
};

#endif