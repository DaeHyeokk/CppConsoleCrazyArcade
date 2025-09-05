#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "character.h"
#include <memory>

#define SPEED 10 
#define MAX_SPEED 70
#define POWER 1
#define MAX_POWER 6
#define BOMBS_COUNT 1
#define MAX_BOMBS_COUNT 7
#define KEY_PRESSED 0x8000

class Player : public Character
{
public:
	Player(Position _position, Board& _board);
	virtual ~Player();
	virtual void update() override;
	void getPlayerInput();
	virtual void takeConsumeItem(std::unique_ptr<Item>& takeItem) override;
	virtual void useConsumeItem() override;
	virtual void die() override;
	virtual void draw() override;
};
#endif // !__PLAYER_H_
