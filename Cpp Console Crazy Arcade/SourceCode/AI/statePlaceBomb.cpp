#include "statePlaceBomb.h"
#include "enemy.h"
#include "character.h"
#include "board.h"
#include <iostream>

using namespace std;

StatePlaceBomb::StatePlaceBomb(Enemy& fromEnemy, Board& _board, Character* _player)
	: enemy(fromEnemy), board(_board), player(_player)
{
}

StatePlaceBomb::~StatePlaceBomb()
{
}

void StatePlaceBomb::update()
{
	if (isSafePlaceBomb())
		enemy.placeBomb();

	enemy.changeState(EnemyState::IDLE);
}

// 만약 현재 위치에 폭탄을 설치한다면 안전하게 도망갈 수 있는지 체크
bool StatePlaceBomb::isSafePlaceBomb()
{
	vector<vector<double>> tempDangerMap = enemy.getDangerMap();
	tempDangerMap[enemy.getPosition().y][enemy.getPosition().x] = min(tempDangerMap[enemy.getPosition().y][enemy.getPosition().x], EXPLODE_SEC);

	// 폭발은 동서남북 방향으로 번짐
	for (auto& dirPos : enemy.dir)
	{
		for (int power = 1; power <= enemy.getPower(); power++)
		{
			bool isCollisionBlock = false;
			Position explosionPos = { enemy.getPosition().y + dirPos.y * power, enemy.getPosition().x + dirPos.x * power};
			if (board.isSafeRangeMap(explosionPos))
			{
				std::vector<GameObject*> objects = board.getObject(explosionPos);
				if (!objects.empty())
				{
					for (const auto& object : objects)
					{
						if (object) {
							if (object->activeSelf())
							{
								// 폭발이 BLOCK에 닿는 경우 더이상 번지지 않음
								if (object->getType() == ObjectType::BLOCK)
									isCollisionBlock = true;
							}
							else // object->activeSelf == false
							{
								// 폭발은 동시에 일어나는 것이기 때문에
								// 한 프레임에서 다른 폭탄에 의해 파괴됐던 블록과 충돌해도 더이상 번지지 않아야함
								if (object->getType() == ObjectType::BLOCK)
									isCollisionBlock = true;
							}
						}
					}
					if (isCollisionBlock)
						break;

					tempDangerMap[explosionPos.y][explosionPos.x] = min(tempDangerMap[explosionPos.y][explosionPos.x], EXPLODE_SEC);
				}
				else
					tempDangerMap[explosionPos.y][explosionPos.x] = min(tempDangerMap[explosionPos.y][explosionPos.x], EXPLODE_SEC);
			}
		}
	}

	return isMoveToSafeSpot(tempDangerMap);
}

bool StatePlaceBomb::isMoveToSafeSpot(std::vector<std::vector<double>>& tempDangerMap)
{
	vector<vector<bool>> visited(board.getMaxY(), std::vector<bool>(board.getMaxX(), false));
	// pair<이동거리, 폭발까지 남은 최소시간>, pair<최초 방향, 탐색에 필요한 좌표>
	queue<pair<pair<int, double>, pair<Direction, Position>>> posQ;
	Position startPos = enemy.getPosition();
	visited[startPos.y][startPos.x] = true;

	for (const auto& dirPos : enemy.dir)
	{
		Position next = startPos + dirPos;

		if (board.isSafeRangeMap(next))
		{

			if (!enemy.isBlockedTile(next))
			{
				posQ.push({ {1,tempDangerMap[next.y][next.x]}, {enemy.dirPosToDirection(dirPos), next} });
				visited[next.y][next.x] = true;
			}
		}
	}

	while (!posQ.empty())
	{
		int depth = posQ.front().first.first;
		double explodeRemainTime = posQ.front().first.second;
		Direction dir = posQ.front().second.first;
		Position pos = posQ.front().second.second;
		double moveDelay = enemy.getMoveDelay();
		posQ.pop();

		// 현재 위치가 폭탄이 없는 좌표라면 해당 좌표를 향해 이동 가능한지 체크
		if (tempDangerMap[pos.y][pos.x] == SAFE_RISK)
		{
			// (이동 딜레이 * 목적지까지 칸수)까지의 시간이 폭발 남은 시간보다 짧다면 폭탄 설치 가능
			if (moveDelay * depth < explodeRemainTime - RISK_BUFFER)
				return true;
		}

		for (const auto& dirPos : enemy.dir)
		{
			Position nextPos = pos + dirPos;
			double nextExplodeRemainTime = tempDangerMap[nextPos.y][nextPos.x];

			if (board.isSafeRangeMap(nextPos))
			{
				if (!visited[nextPos.y][nextPos.x])
				{

					// 이동하려는 곳이 막혀 있지 않다면 이동 목록에 넣는다.
					if (!enemy.isBlockedTile(nextPos))
					{
						double shorterTime = min(explodeRemainTime, nextExplodeRemainTime);
						visited[nextPos.y][nextPos.x] = true;
						posQ.push({ {depth + 1, shorterTime }, {dir, nextPos} });
					}
				}
			}
		}
	}

	return false;
}