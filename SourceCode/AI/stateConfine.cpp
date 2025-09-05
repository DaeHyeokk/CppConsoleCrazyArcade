#include "stateConfine.h"
#include "enemy.h"
#include "character.h"
#include "board.h"


StateConfine::StateConfine(Enemy& fromEnemy, Board& _board, Character* _player)
	: enemy(fromEnemy), board(_board), player(_player)
{
}

StateConfine::~StateConfine()
{
}

void StateConfine::update()
{
	if (hasNeedleItem())
	{
		enemy.useConsumeItem();
		enemy.changeState(EnemyState::IDLE);
	}
}

bool StateConfine::hasNeedleItem()
{
	Item* hasItem = enemy.getItem();
	if (hasItem)
	{
		return hasItem->getType() == ItemType::NEEDLE;
	}

	return false;
}
