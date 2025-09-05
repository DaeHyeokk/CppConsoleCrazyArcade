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
	// Update�� overriding ���� �ʴ� Ŭ������ �ƹ� ������ �������� ����
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
	// setActiveAction�� overriding ���� �ʴ� Ŭ������ �ƹ� ������ �������� ����
}

void GameObject::setInactiveAction()
{
	// setInactiveAction�� overriding ���� �ʴ� Ŭ������ �ƹ� ������ �������� ����
}

void GameObject::draw()
{
	gotoxy(SCREEN_CENTER_XPOS + position.x * OBJECT_SIZE_X, SCREEN_CENTER_YPOS + position.y * OBJECT_SIZE_Y);
}
