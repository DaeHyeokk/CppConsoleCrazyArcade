#include "board.h"
#include "gameObject.h"
#include "bomb.h"
#include "flame.h"
#include "flamePool.h"
#include "character.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "distructibleBlock.h"
#include "item.h"
#include "dart.h"
#include "needle.h"
#include "itemBomb.h"
#include "superPotion.h"
#include "powerPotion.h"
#include "shoes.h"
#include "dartBullet.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

Board::Board() : maxX(0), maxY(0), playerCnt(0), enemyCnt(0), gameState(PLAYING)
{
}

void Board::setBoard(vector<vector<int>>& newBoard)
{
	gameState = GameState::PLAYING;

	enum ValueMean
	{
		EMPTY,
		BLOCK,
		DISTRUCTIBLEBLOCK
	};

	maxY = newBoard.size();
	maxX = newBoard[0].size();

	for(int y = 0; y < maxY; y++)
		for (int x = 0; x < maxX; x++)
		{
			Position pos = { y,x };
			switch (newBoard[y][x])
			{
			case EMPTY:
				break;

			case BLOCK:
				objects.push_back(move(make_unique<Block>(pos, *this)));
				objects.back()->draw();
				break;

			case DISTRUCTIBLEBLOCK:
				objects.push_back(move(make_unique<DistructibleBlock>(pos, *this)));
				objects.back()->draw();
				break;
			}
		}

	flamePool = make_shared<FlamePool>(*this);
	flamePool->init();

	drawGameInfo();
}

void Board::spawnPlayer()
{
	unique_ptr<Character> player = make_unique<Player>(Position{ 1,5 }, *this);
	unique_ptr<Character> enemy = make_unique<Enemy>(Position{ 11,13 }, *this, player.get());
	setObject(move(enemy));
	setObject(move(player));

	playerCnt = 1;
	enemyCnt = 1;
}


int Board::getMaxY()
{
	return maxY;
}

int Board::getMaxX()
{
	return maxX;
}


vector<GameObject*> Board::getObject(Position position)
{
	vector<GameObject*> retObjects;
	for (auto& object : objects)
	{
		// 오브젝트가 존재한다면
		if (object)
		{
			if (object->getPosition() == position)
			{
				retObjects.push_back(object.get());
			}
		}
	}

	return retObjects;
}

std::vector<GameObject*> Board::getAllObject()
{
	vector<GameObject*> allObject;
	for (auto& object : objects)
		if(object)
			allObject.push_back(object.get());

	return allObject;
}

// unique_ptr 을 받아간다는 것은 해당 객체의 소유권을 이동시킬 목적이라는 것이 명확함.
unique_ptr<GameObject> Board::getObjectUniquePtr(GameObject* gameObj)
{
	for(auto& object : objects)
		if (object && object.get() == gameObj)
		{
			unique_ptr<GameObject> retObject = move(object);
			return retObject;
		}

	return nullptr;
}

std::shared_ptr<FlamePool> Board::getFlamePool()
{
	return flamePool;
}


void Board::setObject(unique_ptr<GameObject> gameObj)
{
	for (auto& newObject : newObjects)
		if (newObject == nullptr)
		{
			newObject = move(gameObj);
			return;
		}

	newObjects.push_back(move(gameObj));
}

void Board::setErasePosition(Position erasePos)
{
	erasePosQ.push(erasePos);
}

void Board::updateBoard()
{
	vector<Enemy*> enemies;

	for (const auto& object : objects)
		if (object)
		{
			// Enemy 객체는 업데이트가 모두 수행된 다음 폭탄의 범위를 계산해야 하는 로직이 있기 때문에
			// Update 가장 마지막에 수행해야함
			if (object->getType() == ObjectType::ENEMY)
			{
				enemies.push_back(dynamic_cast<Enemy*>(object.get()));
				continue;
			}
			object->update();
		}

	for (const auto& enemy : enemies)
		enemy->update();

	// 업데이트 수행 이후 파괴될 오브젝트 확인. -> 늦은 해제
	for(auto& object: objects)
		if (object)
		{
			if (object->activeSelf())
				continue;
			else
			{
				setErasePosition(object->getPosition());
				GameObject* rawPtr = object.get();
				unique_ptr<GameObject> moveObj = move(object);
				rawPtr->returnObject(move(moveObj));
				// object->returnObject(move(object)); 이 코드와 의미가 같음.
			}
		}

	updateObjects();
}

void Board::updateObjects()
{
	for (auto& newObject : newObjects)
	{
		bool isMoved = false;
		for (auto& object : objects)
		{
			if (object == nullptr)
			{
				object = move(newObject);
				newObject = nullptr;
				isMoved = true;
				break;
			}
		}
		if (!isMoved)
		{
			objects.push_back(move(newObject));
			newObject = nullptr;
		}
	}

	newObjects.clear();
}

void Board::drawBoard()
{
	// 새로 그리기 전에 콘솔 상에서 지워져야 하는 좌표를 우선 지움
	while (!erasePosQ.empty())
	{
		eraseDrawingPosition(erasePosQ.front());
		erasePosQ.pop();
	}

	// 활성화 중인 오브젝트를 그림.
	for (const auto& object : objects)
		if (object && object->activeSelf())
		{
			object->draw();
			if (object->getType() == ObjectType::PLAYER)
			{
				Player* player = dynamic_cast<Player*>(object.get());

			}
		}
}

void Board::eraseDrawingPosition(Position pos)
{
	gotoxy(SCREEN_CENTER_XPOS + pos.x * OBJECT_SIZE_X, SCREEN_CENTER_YPOS + pos.y * OBJECT_SIZE_Y);
	cout << "  ";
}

void Board::eraseCharacterHaveItem()
{
	setCursorCharacterHaveItemPosition();
	cout << "  ";
}

void Board::drawEmptyTile(Position pos)
{
	gotoxy(SCREEN_CENTER_XPOS + pos.x * OBJECT_SIZE_X, SCREEN_CENTER_YPOS + pos.y * OBJECT_SIZE_Y);
	cout << "▨";
}

void Board::drawGameInfo()
{
	gotoxy(62, 3);
	setTextColor(WHITE);
	cout << "[아이템 종류]";

	gotoxy(57, 5);
	setTextColor(CursorColor::DARK_BLUE);
	cout << "◎";
	gotoxy(60, 5);
	cout << "→ 물폭탄 개수 +1";

	gotoxy(57, 6);
	setTextColor(CursorColor::DARK_GRAY);
	cout << "∂";
	gotoxy(60, 6);
	cout << "→ 물폭탄 파워 +1";

	gotoxy(57, 7);
	setTextColor(CursorColor::DARK_RED);
	cout << "‰";
	gotoxy(60, 7);
	cout << "→ 이동속도 +10";

	gotoxy(57, 8);
	setTextColor(CursorColor::YELLOW);
	cout << "♨";
	gotoxy(60, 8);
	cout << "→ 물폭탄 개수 +1";
	gotoxy(63, 9);
	cout << "물폭탄 파워 +1";

	gotoxy(57, 10);
	setTextColor(CursorColor::DARK_SKYBLUE);
	cout << "×";
	gotoxy(60, 10);
	cout << "→ 전방으로 다트를 던져";
	gotoxy(63, 11);
	cout << "물폭탄을 터뜨림";

	gotoxy(57, 12);
	setTextColor(CursorColor::WHITE);
	cout << "↗";
	gotoxy(60, 12);
	cout << "→ 물폭탄에 맞았을 시 부활";

	gotoxy(7, 4);
	setTextColor(WHITE);
	cout << "PLAYER : ";
	gotoxy(17, 4);
	setTextColor(CursorColor::GREEN);
	cout << "♀";

	gotoxy(5, 6);
	setTextColor(WHITE);
	cout << "COMPUTER : ";
	gotoxy(17, 6);
	setTextColor(CursorColor::RED);
	cout << "♀";


	gotoxy(2, 8);
	setTextColor(WHITE);
	cout << "방향키: ";
	gotoxy(10, 8);
	cout << "↑ ← ↓ →";
	gotoxy(10, 9);
	cout << "W  A  S  D";

	gotoxy(4, 11);
	cout << "폭탄: ";
	gotoxy(10, 11);
	cout << "Space bar";

	gotoxy(2, 13);
	cout << "아이템: ";
	gotoxy(11, 13);
	cout << "Ctrl";

	gotoxy(15, 15);
	cout << "ITEM";
	gotoxy(13, 16);
	cout << "【    】";
}

void Board::setCursorCharacterHaveItemPosition()
{
	gotoxy(16, 16);
}

bool Board::isSafeRangeMap(Position pos)
{
		return pos.y >= 0 && pos.y < maxY && pos.x >= 0 && pos.x < maxX;
}

void Board::diePlayer()
{
	playerCnt--;
	if (playerCnt <= 0)
		defeat();
}

void Board::dieEnemy()
{
	enemyCnt--;
	if (enemyCnt <= 0)
		victory();
}

void Board::victory()
{
	gameState = GameState::VICTORY;
}

void Board::defeat()
{
	gameState = GameState::DEFEAT;
}

GameState Board::getGameState()
{
	return gameState;
}


