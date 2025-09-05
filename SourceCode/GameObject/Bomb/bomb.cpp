#include "bomb.h"
#include "Windows.h"
#include <iostream>
// explodeTimer 3000 은 폭탄이 터지는데 걸리는 시간이 3초라는 의미
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

	// explodeTimer 만큼의 시간이 경과했는지 확인
	if (elapsedMS > EXPLODE_TIME)
	{
		explode();
	}
}

void Bomb::explode()
{
	// 객체 비활성화
	setActive(false);
	fromCharacter.retrieveBomb();
	Flame* headFlame = nullptr;
	Flame* tailFlame = nullptr;
	
	// 폭발은 동서남북 방향으로 번짐
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
									// 폭발이 BLOCK에 닿는 경우 더이상 번지지 않음
									if (object->getType() == ObjectType::BLOCK)
										isCollisionBlock = true;

									object->explosionDamageAction();
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
					}
					createFlame(explosionPos, headFlame, tailFlame);
				}
			}
		}
		else // 폭탄이 설치되어 있던 자리까지 터뜨려야함
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

	// 폭발은 동서남북 방향으로 번짐
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
	// explosion 에 닿으면 남은 시간 관계없이 폭발
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

	// 만약 Head 가 없다면 현재 Flame이 가장 Head가 됨
	if (headFlame == nullptr)
	{
		headFlame = nowFlame;
		headFlame->setHeadFlame();
	}
	else
	{
		// Head가 있다면 Head Flame에 nowFlame을 넣고 nowFlame을 다음 Head Flame으로 바꿈
		headFlame->setTail(nowFlame);
		headFlame = nowFlame;
	}

	board.setObject(std::move(flame));
}

void Bomb::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::DARK_SKYBLUE);
	std::cout << "●";
}
