#include "stateEscapeFromBomb.h"
#include "enemy.h"
#include "character.h"
#include "board.h"
#include "randomUtil.h"
#include <iostream>

using namespace std;

StateEscapeFromBomb::StateEscapeFromBomb(Enemy& fromEnemy, Board& _board, Character* _player)
	: enemy(fromEnemy), board(_board), player(_player)
{
}

StateEscapeFromBomb::~StateEscapeFromBomb()
{
}

void StateEscapeFromBomb::update()
{
	if (enemy.isInDanger())
		moveToSafeSpot();
	else
		enemy.changeState(EnemyState::IDLE);
}

void StateEscapeFromBomb::moveToSafeSpot()
{
	vector<vector<bool>> visited(board.getMaxY(), std::vector<bool>(board.getMaxX(), false));
	// pair<이동거리, 폭발까지 남은 최소시간>, pair<최초 방향, 탐색에 필요한 좌표>
	queue<pair<pair<int, double>, pair<Direction, Position>>> posQ;
	Position startPos = enemy.getPosition();
	visited[startPos.y][startPos.x] = true;

	// 방향을 랜덤하게 섞기
	vector<Direction> directions = getShuffledDirections();

	for (Direction dirIndex : directions)
	{
		Position next = startPos + enemy.dir[dirIndex];

		if (board.isSafeRangeMap(next))
		{

			if (!enemy.isBlockedTile(next))
			{
				posQ.push({ {1,enemy.getDangerMapValue(next.y, next.x)}, {dirIndex, next} });
				visited[next.y][next.x] = true;

				if (enemy.getDangerMapValue(next.y, next.x) == SAFE_RISK)
				{
					// 바로 한칸 옆에 안전한 위치가 있으면 바로 이동
					enemy.move(dirIndex);
					return;
				}
			}
		}
	}

	Direction bestDir;
	double bestRisk = 0;

	while (!posQ.empty())
	{
		int depth = posQ.front().first.first;
		double explodeRemainTime = posQ.front().first.second;
		Direction dir = posQ.front().second.first;
		Position pos = posQ.front().second.second;
		double moveDelay = enemy.getMoveDelay();
		posQ.pop();

		// 현재 위치가 안전한 좌표라면 해당 좌표를 향해 이동
		if (enemy.getDangerMapValue(pos.y, pos.x) >= SAFE_BOMB_TIME)
		{
			// (이동 딜레이 * 목적지까지 칸수)까지의 시간이 폭발 남은 시간보다 짧다면 해당 좌표로 이동한다
			if (moveDelay * depth < explodeRemainTime - RISK_BUFFER)
			{
				double risk = explodeRemainTime - (moveDelay * depth);
				if (bestRisk < risk)
				{
					bestRisk = risk;
					bestDir = dir;
				}
			}
		}

		for (const auto& dirPos : enemy.dir)
		{
			Position nextPos = pos + dirPos;
			double nextExplodeRemainTime = enemy.getDangerMapValue(nextPos.y, nextPos.x);

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

	if (bestRisk != 0)
		enemy.move(bestDir);
}

