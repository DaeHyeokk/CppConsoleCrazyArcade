#ifndef __BOARD_H_
#define __BOARD_H_

#include "playMode.h"
#include "gameObject.h"
#include "gameState.h"
#include "itemType.h"
#include <vector>
#include <queue>
#include <memory>

class GameObject;
class Bomb;
class Flame;
class FlamePool;
class Block;
class DistructibleBlock;
class Item;
class Dart;
class Needle;
class ItemBomb;
class PoisonPotion;
class PowerPotion;
class Shoes;
class DartBullet;

class Board
{
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<std::unique_ptr<GameObject>> newObjects;
	std::queue<Position> erasePosQ;
	std::shared_ptr<FlamePool> flamePool;
	int maxY;
	int maxX;
	int playerCnt;
	int enemyCnt;
	GameState gameState;

public:
	Board();
	void setBoard(std::vector<std::vector<int>>& newBoard);
	int getMaxY();
	int getMaxX();
	void spawnPlayer();
	std::vector<GameObject*> getObject(Position position);
	std::vector<GameObject*> getAllObject();
	std::unique_ptr<GameObject> getObjectUniquePtr(GameObject* gameObj);
	std::shared_ptr<FlamePool> getFlamePool();
	void setObject(std::unique_ptr<GameObject> gameObj);
	void setErasePosition(Position erasePos);
	void updateBoard();
	void updateObjects();
	void eraseDrawingPosition(Position pos);
	void eraseCharacterHaveItem();
	void drawBoard();
	void drawEmptyTile(Position pos);
	void drawGameInfo();
	void setCursorCharacterHaveItemPosition();
	bool isSafeRangeMap(Position pos);
	void diePlayer();
	void dieEnemy();
	void victory();
	void defeat();
	GameState getGameState();
};

#endif