#include "gameObject.h"
#include <iostream>
using namespace std;

GameObject::GameObject(Position _position, ObjectType _objectType, Board& _board) 
	: position(_position), objectType(_objectType), board(_board), isActived(true)
{
}

GameObject::~GameObject()
{
}

ObjectType GameObject::getType()
{
	return objectType;
}

Position GameObject::getPosition()
{
	return position;
}

void GameObject::setPosition(Position position)
{
	this->position = position;
}

void GameObject::returnObject(unique_ptr<GameObject> gameObj)
{
	if (gameObj)
		gameObj = nullptr;  // reset
}

void GameObject::update()
{
	// Update를 overriding 하지 않는 클래스는 아무 동작을 수행하지 않음
}

void GameObject::setActive(bool isActived)
{
	this->isActived = isActived;
}

bool GameObject::activeSelf()
{
	return isActived;
}

void GameObject::setActiveAction()
{
	// setActiveAction을 overriding 하지 않는 클래스는 아무 동작을 수행하지 않음
}

void GameObject::setInactiveAction()
{
	// setInactiveAction을 overriding 하지 않는 클래스는 아무 동작을 수행하지 않음
}

void GameObject::draw()
{
	gotoxy(SCREEN_CENTER_XPOS + position.x * OBJECT_SIZE_X, SCREEN_CENTER_YPOS + position.y * OBJECT_SIZE_Y);
}
