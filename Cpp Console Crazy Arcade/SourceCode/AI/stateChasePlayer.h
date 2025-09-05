#ifndef __STATE_CHASE_PLAYER_H_
#define __STATE_CHASE_PLAYER_H_

#define IMPOSSIBLE_ARRIVE -1

class Enemy;
class Board;
class Character;

class StateChasePlayer
{
	Enemy& enemy;
	Character* player;
	Board& board;

public:
	StateChasePlayer(Enemy& fromEnemy, Board& _board, Character* _player);
	~StateChasePlayer();
	void update();
	int getDistanceToPlayer();
	void moveToPlayer();
};

#endif // !__STATE_CHASE_PLAYER_H_
