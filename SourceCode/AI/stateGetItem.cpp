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


		// 아이템 탐색 거리에 도달했음에도 아이템을 발견 못했다면 다음 좌표 탐색 중단
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
							// 이동하려는 곳이 막혀 있지 않고 안전하다면 이동 목록에 넣는다.
							posQ.push({ hereDist + 1, next });
						}
					}
				}
			}
		}
	}

	// 이곳에 도달했다면 아이템을 발견못한것
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

		//현재 탐색하고 있는 좌표에 아이템이 있고 안전한 좌표라면 해당 경로로 이동.
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

		// 아이템 탐색 거리에 도달했음에도 아이템을 발견 못했다면 다음 좌표 탐색 중단
		if (hereDist < GET_ITEM_DISTANCE)
		{
			for (const auto& dirPos : enemy.dir)
			{
				pair<Direction, Position> next = { here.first, here.second + dirPos };
				if (board.isSafeRangeMap(next.second))
				{
					if (!visited[next.second.y][next.second.x])
					{
						// 이동하려는 곳이 막혀 있지 않고 위험지역이 아니라면 이동 목록에 넣는다.
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
