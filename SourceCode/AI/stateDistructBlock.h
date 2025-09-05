#ifndef __STATE_DISTRUCT_BLOCK_H_
#define __STATE_DISTRUCT_BLOCK_H_

class Enemy;
class Board;
class Character;

class StateDistructBlock
{
	Enemy& enemy;
	Character* player;
	Board& board;

public:
	StateDistructBlock(Enemy& fromEnemy, Board& _board, Character* _player);
	~StateDistructBlock();
	void update();
	bool canPlaceBomb();
};

#endif // !__STATE_DISTRUCT_BLOCK_H_
