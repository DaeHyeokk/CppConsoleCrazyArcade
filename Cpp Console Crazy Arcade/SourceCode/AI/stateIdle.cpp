#include "stateIdle.h"
#include "randomUtil.h"
#include "block.h"
#include "enemy.h"
#include "character.h"
#include "board.h"
#include <vector>
#include <iostream>
using namespace std;

StateIdle::StateIdle(Enemy& fromEnemy, Board& _board, Character* _player)
	: enemy(fromEnemy), board(_board), player(_player)
{
}

StateIdle::~StateIdle()
{
}

void StateIdle::update()
{
	if (enemy.isInDanger())
		enemy.changeState(EnemyState::ESCAPE_FROM_BOMB);

	else if (isNearPlayer())
		enemy.changeState(EnemyState::CHASE_PLAYER);

	else if (isNearItem())
		enemy.changeState(EnemyState::GET_ITEM);
	
	else if (isNearDistructibleBlock())
		enemy.changeState(EnemyState::DISTRUCT_BLOCK);

	else
		roamRandomly();
}

void StateIdle::roamRandomly()
{
	// �������� ���� ���� vector ���
	vector<Direction> directions = getShuffledDirections();

	for (Direction dirIndex : directions)
	{
		Position next = enemy.getPosition() + enemy.dir[dirIndex];

		if (board.isSafeRangeMap(next))
		{
			if (!enemy.isBlockedTile(next) && enemy.getDangerMapValue(next.y, next.x) >= SAFE_BOMB_TIME)
			{
				if (enemy.move(dirIndex))
					return;
			}
		}
	}
}

bool StateIdle::isNearPlayer()
{
	vector<vector<bool>> visited(board.getMaxY(), vector<bool>(board.getMaxX(), false));
	queue<pair<int, Position>> posQ;
	Position startPos = enemy.getPosition();
	posQ.push({ 0,startPos });
	visited[startPos.y][startPos.x] = true;

	while (!posQ.empty())
	{
		Position here = posQ.front().second;
		int hereDist = posQ.front().first;
		posQ.pop();

		if (here == player->getPosition())
			return true;

		// �÷��̾� Ž�� �Ÿ��� ������������ �÷��̾ �߰� ���ߴٸ� ���� ��ǥ Ž�� �ߴ�
		if (hereDist < enemy.getPlayerChaseDistance())
		{
			for (const auto& dirPos : enemy.dir)
			{
				Position next = here + dirPos;
				if (board.isSafeRangeMap(next))
				{
					if (!visited[next.y][next.x])
					{
						visited[next.y][next.x] = true;

						if (!enemy.isBlockedTile(next))
						{
							// �̵��Ϸ��� ���� ���� ���� �ʴٸ� �̵� ��Ͽ� �ִ´�.
							posQ.push({ hereDist + 1, next });
						}
					}
				}
			}
		}
	}

	// �̰��� �����ߴٸ� �÷��̾� �߰� ���Ѱ�
	return false;
}

bool StateIdle::isNearItem()
{
	vector<vector<bool>> visited(board.getMaxY(), vector<bool>(board.getMaxX(), false));
	queue<pair<int, Position>> posQ;
	Position startPos = enemy.getPosition();
	posQ.push({ 0,startPos });
	visited[startPos.y][startPos.x] = true;

	while (!posQ.empty())
	{
		Position here = posQ.front().second;
		int hereDist = posQ.front().first;
		posQ.pop();

		vector<GameObject*> objects = board.getObject(here);
		for (auto& object : objects)
		{
			if (object && object->getType() == ObjectType::ITEM)
			{
				return true;
			}
		}


		// ������ Ž�� �Ÿ��� ������������ �������� �߰� ���ߴٸ� ���� ��ǥ Ž�� �ߴ�
		if (hereDist < GET_ITEM_DISTANCE)
		{
			for (const auto& dirPos : enemy.dir)
			{
				Position next = here + dirPos;
				if (board.isSafeRangeMap(next))
				{
					if (!visited[next.y][next.x])
					{
						visited[next.y][next.x] = true;

						if (!enemy.isBlockedTile(next) && enemy.getDangerMapValue(next.y, next.x) >= SAFE_BOMB_TIME)
						{
							// �̵��Ϸ��� ���� ���� ���� �ʴٸ� �̵� ��Ͽ� �ִ´�.
							posQ.push({ hereDist + 1, next });
						}
					}
				}
			}
		}
	}

	// �̰��� �����ߴٸ� �������� �߰߸��Ѱ�
	return false;
}

bool StateIdle::isNearDistructibleBlock()
{
	for (auto& dirPos : enemy.dir)
	{
		Position next = enemy.getPosition() + dirPos;
		// �̵��� ��ġ�� �� ���� �ƴ϶�� ������Ʈ Ž��
		if (board.isSafeRangeMap(next))
		{
			vector<GameObject*> objects = board.getObject(next);
			for (auto& object : objects)
			{
				if (object && object->getType() == ObjectType::BLOCK)
				{
					Block* bomb = dynamic_cast<Block*>(object);
					// ���� ����� �ı� ������ ����̶��
					if (bomb->isDistructible())
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

