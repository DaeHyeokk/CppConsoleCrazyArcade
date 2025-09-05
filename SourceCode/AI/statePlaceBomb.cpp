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

// ���� ���� ��ġ�� ��ź�� ��ġ�Ѵٸ� �����ϰ� ������ �� �ִ��� üũ
bool StatePlaceBomb::isSafePlaceBomb()
{
	vector<vector<double>> tempDangerMap = enemy.getDangerMap();
	tempDangerMap[enemy.getPosition().y][enemy.getPosition().x] = min(tempDangerMap[enemy.getPosition().y][enemy.getPosition().x], EXPLODE_SEC);

	// ������ �������� �������� ����
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
								// ������ BLOCK�� ��� ��� ���̻� ������ ����
								if (object->getType() == ObjectType::BLOCK)
									isCollisionBlock = true;
							}
							else // object->activeSelf == false
							{
								// ������ ���ÿ� �Ͼ�� ���̱� ������
								// �� �����ӿ��� �ٸ� ��ź�� ���� �ı��ƴ� ��ϰ� �浹�ص� ���̻� ������ �ʾƾ���
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
	// pair<�̵��Ÿ�, ���߱��� ���� �ּҽð�>, pair<���� ����, Ž���� �ʿ��� ��ǥ>
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

		// ���� ��ġ�� ��ź�� ���� ��ǥ��� �ش� ��ǥ�� ���� �̵� �������� üũ
		if (tempDangerMap[pos.y][pos.x] == SAFE_RISK)
		{
			// (�̵� ������ * ���������� ĭ��)������ �ð��� ���� ���� �ð����� ª�ٸ� ��ź ��ġ ����
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

	return false;
}