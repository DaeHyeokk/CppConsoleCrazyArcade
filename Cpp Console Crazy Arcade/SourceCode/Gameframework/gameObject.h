#ifndef __GAMEOBJECT_H_
#define __GAMEOBJECT_H_

#include "position.h"
#include "objectType.h"
#include "consoleCursorUtil.h"
#include "board.h"
#include <memory>

#define OBJECT_SIZE_X 2
#define OBJECT_SIZE_Y 1
#define SCREEN_CENTER_XPOS 22
#define SCREEN_CENTER_YPOS 3

class Board;

class GameObject
{
protected:
	bool isActived;
	Position position;
	ObjectType objectType;
	Board& board;

public:
	GameObject(Position _position, ObjectType _objectType, Board& _board);
	virtual ~GameObject();
	ObjectType getType();
	Position getPosition();
	void setPosition(Position position);
	void setActive(bool isActived);
	bool activeSelf();
	virtual void returnObject(std::unique_ptr<GameObject> gameObj);
	virtual void update();
	virtual void setActiveAction();
	virtual void setInactiveAction();
	virtual void explosionDamageAction() = 0;
	virtual void draw();
};

#endif