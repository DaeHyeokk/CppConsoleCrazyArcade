#ifndef __STATE_GET_ITEM_H_
#define __STATE_GET_ITEM_H_

#define GET_ITEM_DISTANCE 4

class Enemy;
class Board;
class Character;

class StateGetItem
{
	Enemy& enemy;
	Character* player;
	Board& board;

public:
	StateGetItem(Enemy& fromEnemy, Board& _board, Character* _player);
	~StateGetItem();
	void update();
	bool isNearItem();
	void moveToNearItem();
};

#endif // !__STATE_GET_ITEM_H_
