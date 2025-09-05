#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "gameObject.h"
#include "character.h"
#include "Windows.h"
#include "consoleCursorUtil.h"
#include "board.h"
#include "mapDictionary.h"

using namespace std;

#define ARROW_KEY_NUMBER 224
#define KEY_PRESSED 0x8000
#define FRAME_LIMIT 15 // 약 60 FPS

void consoleSetting();
void mainSceneStart();
void drawMainScreen(int& cursorPos);
void getMainMenuPlayerInput(int& cursorPos);
void drawMainScreenMenu(int& cursorPos);
void gameStart();
void drawGameVictory();
void drawGameDefeat();

int main(void)
{
	consoleSetting();
	mainSceneStart();

	return 0;
}


void consoleSetting()
{
	SetConsoleTitle(TEXT("Crazy Arcade"));
	system("mode con: lines=22 cols=87");
	setConsoleFontSize(35, 35); // 글자 크기를 크게 설정
	setConsolCursorHide(false);
}

void mainSceneStart()
{
	int cursorPos = 0;
	while (true)
	{
		getMainMenuPlayerInput(cursorPos);
		drawMainScreen(cursorPos);
		drawMainScreenMenu(cursorPos);
		Sleep(400);
	}
}

void drawMainScreen(int& cursorPos)
{
	int y = 0;
	setTextColor(CursorColor::SKYBLUE);
	gotoxy(12,y++); cout << "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::" << '\n';
	gotoxy(12,y++); cout << ":::::::'######::'########:::::'###::::'########:'##:::'##:::::::" << '\n';
	gotoxy(12,y++); cout << "::::::'##... ##: ##.... ##:::'## ##:::..... ##::. ##:'##::::::::" << '\n';
	gotoxy(12,y++); cout << ":::::: ##:::..:: ##:::: ##::'##:. ##:::::: ##::::. ####:::::::::" << '\n';
	gotoxy(12,y++); cout << ":::::: ##::::::: ########::'##:::. ##:::: ##::::::. ##::::::::::" << '\n';
	gotoxy(12,y++); cout << ":::::: ##::::::: ##.. ##::: #########::: ##:::::::: ##::::::::::" << '\n';
	gotoxy(12,y++); cout << ":::::: ##::: ##: ##::. ##:: ##.... ##:: ##::::::::: ##::::::::::" << '\n';
	gotoxy(12,y++); cout << "::::::. ######:: ##:::. ##: ##:::: ##: ########:::: ##::::::::::" << '\n';
	gotoxy(12,y++); cout << ":::::::......:::..:::::..::..:::::..::........:::::..:::::::::::" << '\n';
	gotoxy(12,y++); cout << ":::'###::::'########:::'######:::::'###::::'########::'########:" << '\n';
	gotoxy(12,y++); cout << "::'## ##::: ##.... ##:'##... ##:::'## ##::: ##.... ##: ##.....::" << '\n';
	gotoxy(12,y++); cout << ":'##:. ##:: ##:::: ##: ##:::..:::'##:. ##:: ##:::: ##: ##:::::::" << '\n';
	gotoxy(12,y++); cout << "'##:::. ##: ########:: ##:::::::'##:::. ##: ##:::: ##: ######:::" << '\n';
	gotoxy(12,y++); cout << ":#########: ##.. ##::: ##::::::: #########: ##:::: ##: ##...::::" << '\n';
	gotoxy(12,y++); cout << ":##.... ##: ##::. ##:: ##::: ##: ##.... ##: ##:::: ##: ##:::::::" << '\n';
	gotoxy(12,y++); cout << ":##:::: ##: ##:::. ##:. ######:: ##:::: ##: ########:: ########:" << '\n';
	gotoxy(12,y++); cout << "..:::::..::..:::::..:::......:::..:::::..::........:::........::" << '\n';

	setTextColor(WHITE);
	gotoxy(12, ++y); cout << "메뉴 이동: ↑,↓";
	gotoxy(12, ++y); cout << "메뉴 선택: ENTER";
}

void drawMainScreenMenu(int& cursorPos)
{
	setTextColor(CursorColor::WHITE);
	vector<Position> cursorPosList = { {18, 33}, {20, 33} };

	if (cursorPos < 0)
		cursorPos = cursorPosList.size() - 1;

	else if (cursorPos >= cursorPosList.size())
		cursorPos = 0;
	
	if (cursorPos == 0)
		setTextColor(CursorColor::BLACK, CursorColor::WHITE);
	gotoxy(35, 18);
	cout << " 게임 시작 ";
	if (cursorPos == 0)
		setTextColor(CursorColor::WHITE, CursorColor::BLACK);

	if(cursorPos == 1)
		setTextColor(CursorColor::BLACK, CursorColor::WHITE);
	gotoxy(35, 20);
	cout << " 게임 종료 ";
	if (cursorPos == 1)
		setTextColor(CursorColor::WHITE, CursorColor::BLACK);
}

void getMainMenuPlayerInput(int& cursorPos)
{
	enum
	{
		GAME_START,
		GAME_EXIT
	};


	if (GetAsyncKeyState(VK_UP))
		cursorPos--;
	else if (GetAsyncKeyState(VK_DOWN))
		cursorPos++;
	else if (GetAsyncKeyState(VK_RETURN))
	{
		if (cursorPos == GAME_START)
			gameStart();
		else // cursorPos == GAME_EXIT
			exit(0);
	}
}

void gameStart()
{
	system("cls");
	Board board = Board();
	MapDictionary mapList = MapDictionary();
	vector<vector<int>> m = mapList.getMapData(PIRATE);
	board.setBoard(m);
	board.spawnPlayer();

	int cursorPos = 0;

	while (true)
	{
		if (board.getGameState() == GameState::PLAYING)
		{
			board.updateBoard();
			board.drawBoard();
		}
		else
		{
			Sleep(1000);
			if (board.getGameState() == GameState::VICTORY)
			{
				drawGameVictory();
			}
			else if(board.getGameState() == GameState::DEFEAT)
			{
				drawGameDefeat();
			}
			getMainMenuPlayerInput(cursorPos);
			Sleep(2500);
			system("cls");
			break;
		}

		Sleep(FRAME_LIMIT);
	}
}

void drawPlayingInfo()
{

}

void drawGameVictory()
{
	setTextColor(CursorColor::GREEN);
	int y = 7;
	gotoxy(22, y++); cout << "■■■■■■■■■■■■■■■■■" << '\n';
	gotoxy(22, y++); cout << "■                              ■" << '\n';
	gotoxy(22, y++); cout << "■                              ■" << '\n';
	gotoxy(22, y++); cout << "■        플레이어 승리!!       ■" << '\n';
	gotoxy(22, y++); cout << "■                              ■" << '\n';
	gotoxy(22, y++); cout << "■                              ■" << '\n';
	gotoxy(22, y);	 cout << "■■■■■■■■■■■■■■■■■" << '\n';
}

void drawGameDefeat()
{
	setTextColor(CursorColor::RED);
	int y = 7;
	gotoxy(22, y++); cout << "■■■■■■■■■■■■■■■■■" << '\n';
	gotoxy(22, y++); cout << "■                              ■" << '\n';
	gotoxy(22, y++); cout << "■                              ■" << '\n';
	gotoxy(22, y++); cout << "■        플레이어 패배...      ■" << '\n';
	gotoxy(22, y++); cout << "■                              ■" << '\n';
	gotoxy(22, y++); cout << "■                              ■" << '\n';
	gotoxy(22, y);	 cout << "■■■■■■■■■■■■■■■■■" << '\n';
}