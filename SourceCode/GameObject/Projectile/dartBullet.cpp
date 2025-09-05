#include <vector>
#include <iostream>
#include "dartBullet.h"
#include "board.h"
#include "bomb.h"
using namespace std;

DartBullet::DartBullet(Position _position, Board& _board, Direction _moveDirection)
	: GameObject(_position, ObjectType::BULLET, _board), moveDirection(_moveDirection)
{
}

void DartBullet::move()
{
	Position next = position;
	switch (moveDirection)
	{
	case UP:
		if (position.y > 0)
			next.y--;
		break;

	case DOWN:
		if (position.y < board.getMaxY() - 1)
			next.y++;
		break;

	case LEFT:
		if (position.x > 0)
			next.x--;
		break;

	case RIGHT:
		if (position.x < board.getMaxX() - 1)
			next.x++;
		break;
	}

	// ��Ʈ�� �� ���� ���� �ߴٸ� �Ҹ��
	if (position.y == next.y && position.x == next.x)
	{
		setActive(false);
		return;
	}

	vector<GameObject*> objects = board.getObject(next);
	if (!objects.empty())
	{
		for (auto& object : objects)
		{
			if (object && object->activeSelf())
			{
				if (object->getType() == ObjectType::BLOCK) // �̵��� ���⿡ ���� ������ �Ҹ��
				{
					setActive(false);
					return;
				}
				else if (object->getType() == ObjectType::BOMB) // �̵��� ���⿡ ��ź�� ������ ��� ���߽�Ŵ
				{
					Bomb* bomb = dynamic_cast<Bomb*>(object);
					bomb->explode();
					setActive(false);
					return;
				}
			}
		}
	}
	else
	{
		// ��ġ�� �ٲٱ� ��, ���� �ִ� �ڸ��� ����� ���� ��ǥ�� ������.
		board.setErasePosition(position);
		position = next;
	}
}

void DartBullet::update()
{
	move();
}

void DartBullet::explosionDamageAction()
{
	// �ƹ����� �������
}

void DartBullet::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::DARK_SKYBLUE);
	cout << "��";
}