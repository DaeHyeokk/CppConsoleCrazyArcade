#ifndef __STATE_ESCAPE_FROM_BOMB_H_
#define __STATE_ESCAPE_FROM_BOMB_H_

class Enemy;
class Board;
class Character;

class StateEscapeFromBomb
{
	Enemy& enemy;
	Character* player;
	Board& board;

public:
	StateEscapeFromBomb(Enemy& fromEnemy, Board& _board, Character* _player);
	~StateEscapeFromBomb();
	void update();
	void moveToSafeSpot();
};
#endif // !__STATE_ESCAPE_FROM_BOMB_H_
