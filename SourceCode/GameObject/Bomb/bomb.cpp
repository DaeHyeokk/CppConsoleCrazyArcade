#include "bomb.h"
#include "Windows.h"
#include <iostream>
// explodeTimer 3000 �� ��ź�� �����µ� �ɸ��� �ð��� 3�ʶ�� �ǹ�
#define EXPLODE_TIME 3000

Bomb::Bomb(Position _position, Board& _board, int _power, Character& _fromCharacter)
	: GameObject(_position, ObjectType::BOMB, _board), power(_power), fromCharacter(_fromCharacter)
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&createTime);
}

Bomb::~Bomb()
{
}

void Bomb::update()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double elapsedMS = (double)(now.QuadPart - createTime.QuadPart) * 1000.0 / freq.QuadPart;
	gotoxy(0, 0);

	// explodeTimer ��ŭ�� �ð��� ����ߴ��� Ȯ��
	if (elapsedMS > EXPLODE_TIME)
	{
		explode();
	}
}

void Bomb::explode()
{
	// ��ü ��Ȱ��ȭ
	setActive(false);
	fromCharacter.retrieveBomb();
	Flame* headFlame = nullptr;
	Flame* tailFlame = nullptr;
	
	// ������ �������� �������� ����
	Position dir[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
	for (int d = 0; d <= 4; d++)
	{
		if (d < 4)
		{
			for (int power = 1; power <= this->power; power++)
			{
				bool isCollisionBlock = false;
				Position explosionPos = { this->position.y + dir[d].y * power, this->position.x + dir[d].x * power };
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

									object->explosionDamageAction();
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
					}
					createFlame(explosionPos, headFlame, tailFlame);
				}
			}
		}
		else // ��ź�� ��ġ�Ǿ� �ִ� �ڸ����� �Ͷ߷�����
		{
			Position explosionPos = this->position;

			std::vector<GameObject*> objects = board.getObject(explosionPos);
			if (!objects.empty())
			{
				for (const auto& object : objects)
				{
					if(object->activeSelf())
						object->explosionDamageAction();
				}
			}
			createFlame(explosionPos, headFlame, tailFlame);
		}
	}
}

std::vector<Position> Bomb::getBombExplosionPositions()
{
	std::vector<Position> bombExplosionPositions;

	bombExplosionPositions.push_back(this->position);

	// ������ �������� �������� ����
	Position dir[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
	for (int d = 0; d < 4; d++)
	{
		for (int power = 1; power <= this->power; power++)
		{
			bool isCollisionBlock = false;
			Position explosionPos = { this->position.y + dir[d].y * power, this->position.x + dir[d].x * power };
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

					bombExplosionPositions.push_back(explosionPos);
				}
				else
					bombExplosionPositions.push_back(explosionPos);
			}
		}
	}

	return bombExplosionPositions;
}

double Bomb::getExplodeRemainTime()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double elapsedMS = (double)(now.QuadPart - createTime.QuadPart) * 1000.0 / freq.QuadPart;

	return EXPLODE_TIME - elapsedMS;
}


void Bomb::explosionDamageAction()
{
	// explosion �� ������ ���� �ð� ������� ����
	explode();
}

int Bomb::getPower()
{
	return power;
}

void Bomb::createFlame(Position explosionPos, Flame* headFlame, Flame* tailFlame)
{
	std::unique_ptr<Flame> flame = std::move(board.getFlamePool()->getObject());
	flame->init(explosionPos);

	Flame* nowFlame = flame.get();

	// ���� Head �� ���ٸ� ���� Flame�� ���� Head�� ��
	if (headFlame == nullptr)
	{
		headFlame = nowFlame;
		headFlame->setHeadFlame();
	}
	else
	{
		// Head�� �ִٸ� Head Flame�� nowFlame�� �ְ� nowFlame�� ���� Head Flame���� �ٲ�
		headFlame->setTail(nowFlame);
		headFlame = nowFlame;
	}

	board.setObject(std::move(flame));
}

void Bomb::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::DARK_SKYBLUE);
	std::cout << "��";
}
