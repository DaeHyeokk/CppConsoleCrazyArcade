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

		// ĳ���Ͱ� �̵��� ������ ����. (������ ��������)
		lookingDirection = direction;

		// ĳ���Ͱ� �̵��� ���⿡ �ִ� ������Ʈ�� üũ
		vector<GameObject*> objects = board.getObject(next);
		if (!objects.empty())
		{
			for (auto& object : objects)
			{
				if (object && object->activeSelf())
				{
					if (object->getType() == ObjectType::BLOCK || object->getType() == ObjectType::BOMB) // �̵��� ���⿡ ���� ������ �̵� �Ұ�
						return false;

					else if (object->getType() == ObjectType::ITEM) // �̵��� ���⿡ �������� ������ ������ ȹ��
					{
						unique_ptr<GameObject> objUniquePtr = board.getObjectUniquePtr(object);
						if (objUniquePtr)
						{
							objUniquePtr->setActive(false);
							// Ÿ���� ITEM�̶�� ���� ������ֱ� ������ dynamic ���� ���� static cast ���.
							unique_ptr<Item> itemObj(static_cast<Item*>(objUniquePtr.release()));
							pickupItem(itemObj);
							break;
						}
					}

					// ���� ���� ��ǥ�� ĳ���Ͱ� �ִµ� ĳ���Ͱ� �������¶�� �ٷ� ����.
					else if (object->getType() == ObjectType::ENEMY || object->getType() == ObjectType::PLAYER)
					{
						Character* target = dynamic_cast<Character*>(object);
						if (target->getState() == CharacterState::CONFINE)
							target->die();
					}
				}
			}
		}

		// ��ġ�� �ٲٱ� ��, ���� �ִ� �ڸ��� ����� ���� ��ǥ�� ������.
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
				if (object->getType() == ObjectType::BLOCK || object->getType() == ObjectType::BOMB) // ��������� ��ź ����
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

// �ɾ���� ��ź�� ������ �ٽ� ĳ�������� ȸ����
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

	// ȹ���� ������ ������ �Һ� �������� ��� ĳ������ Item ����� ����
	if (pickItem->getItemCategory() == ItemCategory::CONSUME)
		takeConsumeItem(pickItem);
	// ��� �ۿ��ϴ� �������� ��� �ٷ� ���
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





