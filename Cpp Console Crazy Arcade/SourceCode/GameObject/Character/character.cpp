#include <string>
#include <vector>
#include "character.h"
#include "consumeItem.h"
#include "board.h"
#include "item.h"
using namespace std;

Character::Character(Position _position, Board& _board, ObjectType _objectType, int _speed, int _maxSpeed, int _power, int _maxPower,
	int _bombsCount, int _maxBombsCount) 
	: GameObject(_position, _objectType, _board)
{
	speed = _speed;
	maxSpeed = _maxSpeed;
	power = _power;
	maxPower = _maxPower;
	bombsCount = _bombsCount;
	maxBombsCount = _maxBombsCount;
	plantBombsCount = 0;
	lookingDirection = Direction::DOWN;
	state = CharacterState::ALIVE;
	consumeItem = nullptr;
	confineToggle = true;
	isGetItem = false;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&moveDelay);
	QueryPerformanceCounter(&bombDelay);
	QueryPerformanceCounter(&dieDelay);
}

Character::~Character(){}

void Character::update()
{
	isGetItem = false;

	if(state == CharacterState::CONFINE)
		checkDieTimer();
}

CharacterState Character::getState()
{
	return state;
}

Direction Character::getDirection()
{
	return lookingDirection;
}

int Character::getPower()
{
	return power;
}

Item* Character::getItem()
{
	if (consumeItem)
		return consumeItem.get();
	else
		return nullptr;
}

void Character::adjustSpeed(int val)
{
	if (speed + val <= maxSpeed)
		speed += val;
	else
		speed = maxSpeed;
}

void Character::adjustPower(int val)
{
	if (power + val <= maxPower && power + val >= MIN_POWER)
		power += val;
	else if (power + val < MIN_POWER)
		power = MIN_POWER;
	else // power + val > maxPower
		power = maxPower;
}

void Character::adjustBombsCount(int val)
{
	if (bombsCount + val < maxBombsCount && bombsCount + val > 0)
	{
		bombsCount += val;
	}
	else if (bombsCount + val <= 0)
		bombsCount = 1;
	else // bombsCount + val >= maxBombsCount
		bombsCount = maxBombsCount;
}

bool Character::move(Direction direction)
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double elapsedMS = (double)(now.QuadPart - moveDelay.QuadPart) * 1000.0 / freq.QuadPart;

	if (elapsedMS > CHARACTER_DEFAULT_SPEED - speed)
	{
		Position next = position;
		switch (direction)
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

		// 캐릭터가 이동한 방향을 저장. (아이템 던져야함)
		lookingDirection = direction;

		// 캐릭터가 이동할 방향에 있는 오브젝트를 체크
		vector<GameObject*> objects = board.getObject(next);
		if (!objects.empty())
		{
			for (auto& object : objects)
			{
				if (object && object->activeSelf())
				{
					if (object->getType() == ObjectType::BLOCK || object->getType() == ObjectType::BOMB) // 이동할 방향에 블럭이 있으면 이동 불가
						return false;

					else if (object->getType() == ObjectType::ITEM) // 이동할 방향에 아이템이 있으면 아이템 획득
					{
						unique_ptr<GameObject> objUniquePtr = board.getObjectUniquePtr(object);
						if (objUniquePtr)
						{
							objUniquePtr->setActive(false);
							// 타입이 ITEM이라는 것이 보장돼있기 때문에 dynamic 보다 빠른 static cast 사용.
							unique_ptr<Item> itemObj(static_cast<Item*>(objUniquePtr.release()));
							pickupItem(itemObj);
							break;
						}
					}

					// 만약 다음 좌표에 캐릭터가 있는데 캐릭터가 기절상태라면 바로 죽임.
					else if (object->getType() == ObjectType::ENEMY || object->getType() == ObjectType::PLAYER)
					{
						Character* target = dynamic_cast<Character*>(object);
						if (target->getState() == CharacterState::CONFINE)
							target->die();
					}
				}
			}
		}

		// 위치를 바꾸기 전, 원래 있던 자리를 지우기 위해 좌표를 저장함.
		board.setErasePosition(position);
		position = next;
		QueryPerformanceCounter(&moveDelay);

		return true;
	}

	return true;
}

void Character::placeBomb()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double elapsedMS = (double)(now.QuadPart - bombDelay.QuadPart) * 1000.0 / freq.QuadPart;

	if (elapsedMS > PLACE_BOMB_DELAY)
	{
		vector<GameObject*> objects = board.getObject(position);
		if (!objects.empty())
		{
			for (auto& object : objects)
				if (object->getType() == ObjectType::BLOCK || object->getType() == ObjectType::BOMB) // 빈공간에만 폭탄 가능
					return;
		}

		if (isCanPlaceBomb())
		{
			board.setObject(std::move(std::make_unique<Bomb>(position, board, power, *this)));
			plantBombsCount++;

			QueryPerformanceCounter(&bombDelay);
		}
	}
}

// 심어놨던 폭탄이 터지면 다시 캐릭터한테 회수됨
void Character::retrieveBomb()
{
	if(plantBombsCount > 0)
		plantBombsCount--;
}

bool Character::isCanPlaceBomb()
{
	return plantBombsCount < bombsCount;
}

void Character::pickupItem(unique_ptr<Item>& pickItem)
{
	pickItem->pickupAction();

	// 획득한 아이템 종류가 소비 아이템일 경우 캐릭터의 Item 멤버에 보관
	if (pickItem->getItemCategory() == ItemCategory::CONSUME)
		takeConsumeItem(pickItem);
	// 즉시 작용하는 아이템일 경우 바로 사용
	else
		pickItem->use(*this);
}

void Character::takeConsumeItem(unique_ptr<Item>& takeItem)
{
	unique_ptr<ConsumeItem> takeConsumeItem(static_cast<ConsumeItem*>(takeItem.release()));
	consumeItem = std::move(takeConsumeItem);
}

void Character::useConsumeItem()
{
	if(consumeItem)
	{
		consumeItem->use(*this);
		consumeItem = nullptr;
	}
}

void Character::checkDieTimer()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	double elapsedMS = (double)(now.QuadPart - dieDelay.QuadPart) * 1000.0 / freq.QuadPart;
	if (elapsedMS >= DIE_DELAY_TIME)
		die();
}

void Character::confine()
{
	state = CharacterState::CONFINE;
	QueryPerformanceCounter(&dieDelay);
}

void Character::revive()
{
	state = CharacterState::ALIVE;
}

void Character::die()
{
	state = CharacterState::DIE;
	setActive(false);
	QueryPerformanceCounter(&dieDelay);
}

void Character::explosionDamageAction()
{
	confine();
}





