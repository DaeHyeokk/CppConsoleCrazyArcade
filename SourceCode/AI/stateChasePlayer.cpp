#include "stateChasePlayer.h"
#include "randomUtil.h"
#include "enemy.h"
#include "character.h"
#include "board.h"
#include <iostream>
using namespace std;

StateChasePlayer::StateChasePlayer(Enemy& fromEnemy, Board& _board, Character* _player)
	: enemy(fromEnemy), board(_board), player(_player)
{
}

StateChasePlayer::~StateChasePlayer()
{
}

void StateChasePlayer::update()
{

	if (enemy.isInDanger())
	{
		enemy.changeState(EnemyState::ESCAPE_FROM_BOMB);
		return;
	}

	int distToPlayer = getDistanceToPlayer();

	if (distToPlayer == IMPOSSIBLE_ARRIVE)
	{
		enemy.changeState(EnemyState::IDLE);
	}
	else
	{
		if (distToPlayer <= enemy.getPower() && player->getState() == CharacterState::ALIVE)
			enemy.changeState(EnemyState::PLACE_BOMB);
		else
			moveToPlayer();
	}
}

int StateChasePlayer::getDistanceToPlayer()
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
		{
			return hereDist;
		}
		// �÷��̾� Ž�� ��Ÿ��� �������� �ʾ��� ��쿡�� ���� ��ǥ Ž��
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
	return IMPOSSIBLE_ARRIVE;
}

void StateChasePlayer::moveToPlayer()
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
				posQ.push({ 1,{ dirIndex, next } });
		}
	}

	while (!posQ.empty())
	{
		int hereDist = posQ.front().first;
		pair<Direction, Position> here = posQ.front().second;
		posQ.pop();

		// ���� ��ġ�� �÷��̾� ��ġ���, �ش� ��η� �̵�
		if (here.second == player->getPosition())
		{
			enemy.move(here.first);
			return;
		}

		// �÷��̾� Ž�� ��Ÿ��� �������� �ʾ��� ��쿡�� ���� ��ǥ Ž��
		if (hereDist < enemy.getPlayerChaseDistance())
		{
			for (const auto& dirPos : enemy.dir)
			{
				pair<Direction, Position> next = { here.first, here.second + dirPos };
				if (board.isSafeRangeMap(next.second))
				{
					if (!visited[next.second.y][next.second.x])
					{
						visited[next.second.y][next.second.x] = true;

						if (!enemy.isBlockedTile(next.second))
						{
							// �̵��Ϸ��� ���� ���� ���� �ʴٸ� �̵� ��Ͽ� �ִ´�.
							posQ.push({ hereDist + 1, { next.first, next.second } });
						}
					}
				}
			}
		}
	}
	return;
}


