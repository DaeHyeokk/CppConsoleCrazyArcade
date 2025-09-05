#include "stateDistructBlock.h"
#include "block.h"
#include "enemy.h"
#include "character.h"
#include "board.h"
using namespace std;

StateDistructBlock::StateDistructBlock(Enemy& fromEnemy, Board& _board, Character* _player)
	: enemy(fromEnemy), board(_board), player(_player)
{
}

StateDistructBlock::~StateDistructBlock()
{
}

void StateDistructBlock::update()
{
	if (canPlaceBomb())
		enemy.changeState(EnemyState::PLACE_BOMB);
	else
		enemy.changeState(EnemyState::IDLE);

}

bool StateDistructBlock::canPlaceBomb()
{
	return enemy.isCanPlaceBomb();
}

