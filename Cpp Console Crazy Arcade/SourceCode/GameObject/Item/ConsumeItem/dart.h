#ifndef __DART_H_
#define __DART_H_

#include "consumeItem.h"

class Character;
class Board;
class DartBullet;

class Dart : public ConsumeItem
{
public:
	Dart(Position _position, Board& _board);
	virtual ~Dart();
	virtual void use(Character& owner) override;
	virtual void draw() override;
	virtual ItemType getItemType() override;
};

#endif