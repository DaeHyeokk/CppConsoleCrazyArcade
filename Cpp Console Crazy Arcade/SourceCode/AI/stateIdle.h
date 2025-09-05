#ifndef __STATE_IDLE_H_
#define __STATE_IDLE_H_

class Enemy;
class Board;
class Character;

class StateIdle
{
	Enemy& enemy;
	Character* player;
	Board& board;

public:
	StateIdle(Enemy& fromEnemy, Board& _board, Character* _player);
	~StateIdle();
	void update();
	void roamRandomly();
	bool isNearPlayer();
	bool isNearItem();
	bool isNearDistructibleBlock();
};

#endif // !__STATE_IDLE_H_
