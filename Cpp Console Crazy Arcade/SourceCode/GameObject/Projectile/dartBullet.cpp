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

	// 다트가 벽 끝에 도달 했다면 소멸됨
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
				if (object->getType() == ObjectType::BLOCK) // 이동할 방향에 블럭이 있으면 소멸됨
				{
					setActive(false);
					return;
				}
				else if (object->getType() == ObjectType::BOMB) // 이동할 방향에 폭탄이 있으면 즉시 폭발시킴
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
		// 위치를 바꾸기 전, 원래 있던 자리를 지우기 위해 좌표를 저장함.
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
	// 아무동작 수행안함
}

void DartBullet::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::DARK_SKYBLUE);
	cout << "×";
}