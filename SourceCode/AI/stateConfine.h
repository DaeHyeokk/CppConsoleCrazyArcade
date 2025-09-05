#ifndef __STATE_CONFINE_H_
#define __STATE_CONFINE_H_

class Enemy;
class Board;
class Character;

class StateConfine
{
	Enemy& enemy;
	Character* player;
	Board& board;

public:
	StateConfine(Enemy& fromEnemy, Board& _board, Character* _player);
	~StateConfine();
	void update();
	bool hasNeedleItem();
};

#endif // !__STATE_CONFINE_H_
