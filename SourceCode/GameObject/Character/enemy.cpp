#include "enemy.h"
#include <iostream>
#include <queue>
#include <random>
#include "block.h"
#include <chrono>
using namespace std;

#define SPEED 0
#define MAX_SPEED 70
#define POWER 1
#define MAX_POWER 6
#define BOMBS_COUNT 1
#define MAX_BOMBS_COUNT 7

Enemy::Enemy(Position _position, Board& _board, Character* _player)
	: Character(_position, _board, ENEMY, SPEED, MAX_SPEED, POWER, MAX_POWER, BOMBS_COUNT, MAX_BOMBS_COUNT), player(_player)
{
	enemyState = EnemyState::IDLE;
	dangerMap = vector<vector<double>>(board.getMaxY(), vector<double>(board.getMaxX(), 0));

	stateIdle = std::make_unique<StateIdle>(*this, _board, _player);
	stateEscapeFromBomb = std::make_unique<StateEscapeFromBomb>(*this, _board, _player);;
	stateChasePlayer = std::make_unique<StateChasePlayer>(*this, _board, _player);
	stateGetItem = std::make_unique<StateGetItem>(*this, _board, _player);
	stateDistructBlock = std::make_unique<StateDistructBlock>(*this, _board, _player);
	statePlaceBomb = std::make_unique<StatePlaceBomb>(*this, _board, _player);
	stateConfine = std::make_unique<StateConfine>(*this, _board, _player);
}

Enemy::~Enemy()
{
}


void Enemy::update()
{
	Character::update();
	updateDangerMap();

	if (state == CharacterState::ALIVE)
	{
		switch (enemyState)
		{
		case EnemyState::IDLE:
			stateIdle->update();
			break;

		case EnemyState::ESCAPE_FROM_BOMB:
			stateEscapeFromBomb->update();
			break;

		case EnemyState::CHASE_PLAYER:
			stateChasePlayer->update();
			break;

		case EnemyState::GET_ITEM:
			stateGetItem->update();
			break;

		case EnemyState::DISTRUCT_BLOCK:
			stateDistructBlock->update();
			break;

		case EnemyState::PLACE_BOMB:
			statePlaceBomb->update();
			break;
		}
	}
	else if (state == CharacterState::CONFINE)
		checkDieTimer();
}

void Enemy::updateDangerMap()
{
	resetDangerMap();

	std::vector<GameObject*> objects = board.getAllObject();

	for (const auto& object : objects)
	{
		if (object && object->getType() == ObjectType::BOMB)
		{
			Bomb* bomb = dynamic_cast<Bomb*>(object);
			double remainExplodeTime = bomb->getExplodeRemainTime();
			std::vector<Position> explosionPositions = bomb->getBombExplosionPositions();

			for (const auto& explosionPos : explosionPositions)
			{
				dangerMap[explosionPos.y][explosionPos.x] = min(dangerMap[explosionPos.y][explosionPos.x], remainExplodeTime);
			}
		}
	}
}

bool Enemy::isBlockedTile(Position pos)
{
	for (auto& obj : board.getObject(pos)) {
		ObjectType type = obj->getType();
		if (type == ObjectType::BLOCK || type == ObjectType::BOMB)
			return true;
	}
	return false;
}

bool Enemy::isInDanger()
{
	return dangerMap[position.y][position.x] < SAFE_BOMB_TIME;
}

void Enemy::resetDangerMap()
{
	int maxY = board.getMaxY();
	int maxX = board.getMaxX();	

	for (int y = 0; y < maxY; y++)
		for (int x = 0; x < maxX; x++)
			dangerMap[y][x] = SAFE_RISK;
}

int Enemy::getPlayerChaseDistance()
{
	return DEFAULT_CHASE_DISTANCE + power;
}

double Enemy::getMoveDelay()
{
	return (CHARACTER_DEFAULT_SPEED - speed);
}

double Enemy::getDangerMapValue(int y, int x)
{
	return dangerMap[y][x];
}

Direction Enemy::dirPosToDirection(Position dirPos)
{
	if (dirPos == Position{ -1, 0 })
		return Direction::UP;

	else if (dirPos == Position{ 1, 0 })
		return Direction::DOWN;

	else if (dirPos == Position{ 0, -1 })
		return Direction::LEFT;
	
	else
		return Direction::RIGHT;
}

std::vector<std::vector<double>>& Enemy::getDangerMap()
{
	return dangerMap;
}

void Enemy::changeState(EnemyState state)
{
	enemyState = state;
}

void Enemy::confine()
{
	Character::confine();
	changeState(EnemyState::CONFINE);
}

void Enemy::revive()
{
	Character::revive();
	changeState(EnemyState::IDLE);
}

void Enemy::die()
{
	Character::die();
	board.dieEnemy();
}

void Enemy::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::RED);

	if (state == CharacterState::CONFINE)
	{
		if (confineToggle)
			std::cout << "○";
		else
			std::cout << "♀";

		confineToggle = !confineToggle;
	}
	else
		std::cout << "♀";
		
	//std::cout << "§";
}


void Enemy::printDangerMap() {
	const int height = dangerMap.size();
	const int width = dangerMap[0].size();

	gotoxy(0, 0);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			if (dangerMap[y][x] != SAFE_RISK) {
				std::cout << 'x'; // 위험 지역
			}
			else {
				std::cout << "."; // 안전 지역
			}
		}
		std::cout << '\n';
	}
}

std::string Enemy::getEnemyStateName()
{
	switch (enemyState) {
	case EnemyState::IDLE:              return "IDLE";
	case EnemyState::ESCAPE_FROM_BOMB:  return "ESCAPE_FROM_BOMB";
	case EnemyState::CHASE_PLAYER:      return "CHASE_PLAYER";
	case EnemyState::PLACE_BOMB:        return "PLACE_BOMB";
	case EnemyState::DISTRUCT_BLOCK:    return "DISTRUCT_BLOCK";
	case EnemyState::GET_ITEM:          return "GET_ITEM";
	case EnemyState::CONFINE:           return "CONFINE";
	default:                             return "UNKNOWN";
	}
}
