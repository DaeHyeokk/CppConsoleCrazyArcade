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
	// pair<�̵��Ÿ�, ���߱��� ���� �ּҽð�>, pair<���� ����, Ž���� �ʿ��� ��ǥ>
	queue<pair<pair<int, double>, pair<Direction, Position>>> posQ;
	Position startPos = enemy.getPosition();
	visited[startPos.y][startPos.x] = true;

	// ������ �����ϰ� ����
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
					// �ٷ� ��ĭ ���� ������ ��ġ�� ������ �ٷ� �̵�
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

		// ���� ��ġ�� ������ ��ǥ��� �ش� ��ǥ�� ���� �̵�
		if (enemy.getDangerMapValue(pos.y, pos.x) >= SAFE_BOMB_TIME)
		{
			// (�̵� ������ * ���������� ĭ��)������ �ð��� ���� ���� �ð����� ª�ٸ� �ش� ��ǥ�� �̵��Ѵ�
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

					// �̵��Ϸ��� ���� ���� ���� �ʴٸ� �̵� ��Ͽ� �ִ´�.
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

