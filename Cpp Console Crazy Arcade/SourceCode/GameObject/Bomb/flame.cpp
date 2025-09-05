#include "flame.h"
#include "flamePool.h"
#include <iostream>

#define DURATION_TIME 100

Flame::Flame(Position _position, Board& _board, std::shared_ptr<FlamePool> _flamePool) 
	: GameObject(_position, ObjectType::FLAME, _board), isHead(false), tail(nullptr), flamePool(_flamePool)
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&flameCreateTime);
}

Flame::~Flame()
{
}

void Flame::init(Position position)
{
	this->position = position;
	this->isHead = false;
	this->tail = nullptr;

	QueryPerformanceCounter(&flameCreateTime);
	setActive(true);
}

void Flame::setTail(Flame* tailFlame)
{
	this->tail = tailFlame;
}

void Flame::setHeadFlame()
{
	this->isHead = true;
	QueryPerformanceCounter(&flameCreateTime);
}

void Flame::hideFlame()
{
	if(tail != nullptr)
		tail->hideFlame();
	setActive(false);
}

void Flame::update()
{
	if (isHead)
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		double elapsedMS = (double)(now.QuadPart - flameCreateTime.QuadPart) * 1000.0 / freq.QuadPart;


		if (elapsedMS > DURATION_TIME)
		{
			hideFlame();
		}
	}
}


void Flame::explosionDamageAction()
{
}

void Flame::returnObject(std::unique_ptr<GameObject> gameObj)
{
	setPosition({ 0,0 });

	// gameObj에서 객체 소유권을 반납하고(nullptr) 원시 포인터를 반환, 
	// 원시 포인터를 Flame으로 다운캐스팅하고 다시 unique_ptr로 바꿈
	std::unique_ptr<Flame> flameObj(static_cast<Flame*>(gameObj.release())); 
	flamePool->returnObject(std::move(flameObj));
}

void Flame::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::BLUE);
	std::cout << "▦";
}


