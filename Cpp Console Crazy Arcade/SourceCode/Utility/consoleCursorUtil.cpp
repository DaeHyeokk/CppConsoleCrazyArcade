#include "consoleCursorUtil.h"
#include "Windows.h"

void gotoxy(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void setConsoleFontSize(int width, int height) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };
    GetCurrentConsoleFontEx(hOut, FALSE, &font);

    font.dwFontSize.X = width;
    font.dwFontSize.Y = height;

    wcscpy_s(font.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hOut, FALSE, &font);
}

void setConsoleSize(int width, int height) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // 1. 버퍼 사이즈 변경
    COORD bufferSize = { (SHORT)width, (SHORT)height };
    SetConsoleScreenBufferSize(hOut, bufferSize);

    // 2. 창 사이즈 변경
    SMALL_RECT windowSize = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
}

void setConsolCursorHide(bool isShow)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO consoleCursor;

    consoleCursor.bVisible = isShow;
    consoleCursor.dwSize = 1;

    SetConsoleCursorInfo(hConsole, &consoleCursor);
}

void setTextColor(unsigned short text)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

void setTextColor(int foreground, int background)
{
    int color = foreground + background * 16;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}