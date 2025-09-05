#include "stateGetItem.h"
#include "enemy.h"
#include "character.h"
#include "board.h"
#include "randomUtil.h"
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

StateGetItem::StateGetItem(Enemy& fromEnemy, Board& _board, Character* _player)
	: enemy(fromEnemy), board(_board), player(_player)
{
}

StateGetItem::~StateGetItem()
{
}

void StateGetItem::update()
{
	if (enemy.isInDanger())
		enemy.changeState(EnemyState::ESCAPE_FROM_BOMB);
	
	else if(isNearItem())
		moveToNearItem();

	else
		enemy.changeState(EnemyState::IDLE);
}

bool StateGetItem::isNearItem()
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
				return true;
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
							// �̵��Ϸ��� ���� ���� ���� �ʰ� �����ϴٸ� �̵� ��Ͽ� �ִ´�.
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

void StateGetItem::moveToNearItem()
{
	vector<vector<bool>> visited(board.getMaxY(), std::vector<bool>(board.getMaxX(), false));
	queue<pair<int, pair<Direction, Position>>> posQ;
	Position startPos = enemy.getPosition();
	visited[startPos.y][startPos.x] = true;

	vector<Direction> directions = getShuffledDirections();

	for (Direction dirIndex : directions)
	{
		Position next = startPos + enemy.dir[dirIndex];

		if (board.isSafeRangeMap(next))
		{
			visited[next.y][next.x] = true;

			if (!enemy.isBlockedTile(next) && enemy.getDangerMapValue(next.y, next.x) >= SAFE_BOMB_TIME)
				posQ.push({ 1, { dirIndex, next } });
		}
	}

	while (!posQ.empty())
	{
		int hereDist = posQ.front().first;
		pair<Direction, Position> here = posQ.front().second;
		posQ.pop();

		//���� Ž���ϰ� �ִ� ��ǥ�� �������� �ְ� ������ ��ǥ��� �ش� ��η� �̵�.
		if (enemy.getDangerMapValue(here.second.y, here.second.x) >= SAFE_BOMB_TIME)
		{
			vector<GameObject*> objects = board.getObject(here.second);
			for (auto& object : objects)
			{
				if (object && object->getType() == ObjectType::ITEM)
				{
					enemy.move(here.first);
					return;
				}
			}
		}

		// ������ Ž�� �Ÿ��� ������������ �������� �߰� ���ߴٸ� ���� ��ǥ Ž�� �ߴ�
		if (hereDist < GET_ITEM_DISTANCE)
		{
			for (const auto& dirPos : enemy.dir)
			{
				pair<Direction, Position> next = { here.first, here.second + dirPos };
				if (board.isSafeRangeMap(next.second))
				{
					if (!visited[next.second.y][next.second.x])
					{
						// �̵��Ϸ��� ���� ���� ���� �ʰ� ���������� �ƴ϶�� �̵� ��Ͽ� �ִ´�.
						if (!enemy.isBlockedTile(next.second) && enemy.getDangerMapValue(next.second.y, next.second.x) >= SAFE_BOMB_TIME)
						{
							visited[next.second.y][next.second.x] = true;
							posQ.push({ hereDist + 1, { next.first, next.second } });
						}
					}
				}
			}
		}
	}

	return;
}
