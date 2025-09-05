#include "player.h"
#include <vector>
#include "Windows.h"
#include <string>
#include <iostream>
using namespace std;

Player::Player(Position _position, Board& _board)
	: Character(_position, _board, PLAYER, SPEED, MAX_SPEED, POWER, MAX_POWER, BOMBS_COUNT, MAX_BOMBS_COUNT)
{
}
Player::~Player()
{
}
void Player::update()
{
	Character::update();
	getPlayerInput();
}
void Player::getPlayerInput()
{
	{
		if (state == CharacterState::ALIVE)
		{
			vector<std::pair<int, Direction>> inputs = {
				{VK_UP, Direction::UP},
				{VK_DOWN, Direction::DOWN},
				{VK_LEFT, Direction::LEFT},
				{VK_RIGHT, Direction::RIGHT},
				{'W', Direction::UP},
				{'S', Direction::DOWN},
				{'A', Direction::LEFT},
				{'D', Direction::RIGHT},
			};

			for (auto& pair : inputs)
			{
				if (GetAsyncKeyState(pair.first) & KEY_PRESSED)
				{
					move(pair.second);
					break; // 동시에 여러 방향 누르는 거 방지
				}
			}

			if (GetAsyncKeyState(VK_SPACE) & KEY_PRESSED)
				placeBomb();
		}

		if (GetAsyncKeyState(VK_CONTROL) & KEY_PRESSED)
			useConsumeItem();
	}

}

void Player::takeConsumeItem(std::unique_ptr<Item>& takeItem)
{
	Character::takeConsumeItem(takeItem);
	consumeItem->drawCharacterHaveItem();
}

void Player::useConsumeItem()
{
	if (consumeItem)
	{
		consumeItem->eraseCharacterHaveItem();
		Character::useConsumeItem();
	}
}

void Player::die()
{
	Character::die();
	board.defeat();
}

void Player::draw()
{
	GameObject::draw();
	setTextColor(CursorColor::GREEN);

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
}
