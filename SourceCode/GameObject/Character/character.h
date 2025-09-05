#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include "gameObject.h"
#include "consumeItem.h"
#include "bomb.h"
#include "direction.h"
#include "characterState.h"
#include "Windows.h"
#include <memory>
#include <string>

#define CHARACTER_DEFAULT_SPEED 200
#define PLACE_BOMB_DELAY 100
#define MIN_POWER 1
#define DIE_DELAY_TIME 5000

class Board;

class Character : public GameObject
{
protected:
	int speed;
	int maxSpeed;
	int power;
	int maxPower;
	int plantBombsCount;
	int bombsCount;
	int maxBombsCount;
	bool isGetItem;
	LARGE_INTEGER moveDelay;
	LARGE_INTEGER bombDelay;
	LARGE_INTEGER freq;
	LARGE_INTEGER dieDelay;
	std::unique_ptr<ConsumeItem> consumeItem;
	Direction lookingDirection;
	CharacterState state;
	bool confineToggle;

public:
	Character(Position _position, Board& _board, ObjectType _objectType, int _speed, int _maxSpeed, int _power, int _maxPower, 
	int _bombsCount, int _maxBombsCount);
	virtual ~Character();
	virtual void update();
	CharacterState getState();
	Direction getDirection();
	int getPower();
	Item* getItem();
	void adjustSpeed(int val);
	void adjustPower(int val);
	void adjustBombsCount(int val);
	bool move(Direction direction);
	void placeBomb();
	void retrieveBomb();
	bool isCanPlaceBomb();
	void pickupItem(std::unique_ptr<Item>& pickItem);
	virtual void takeConsumeItem(std::unique_ptr<Item>& takeItem);
	virtual void useConsumeItem();
	void checkDieTimer();
	virtual void confine();
	virtual void revive();
	virtual void die();
	virtual void explosionDamageAction() override;
};

#endif