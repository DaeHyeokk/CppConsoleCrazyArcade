#ifndef __STATE_PLACE_BOMB
#define __STATE_PLACE_BOMB

#include <vector>

class Enemy;
class Board;
class Character;

class StatePlaceBomb
{
	Enemy& enemy;
	Character* player;
	Board& board;

public:
	StatePlaceBomb(Enemy& fromEnemy, Board& _board, Character* _player);
	~StatePlaceBomb();
	void update();
	bool isSafePlaceBomb();
	bool isMoveToSafeSpot(std::vector<std::vector<double>>& dangerMap);
};

#endif // !__STATE_PLACE_BOMB

