#ifndef __CONSOLECURSORUTIL_H_
#define __CONSOLECURSORUTIL_H_

//»ö»ó
enum CursorColor
{
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_SKYBLUE,
    DARK_RED,
    DARK_VOILET,
    DAKR_YELLOW,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    SKYBLUE,
    RED,
    VIOLET,
    YELLOW,
    WHITE,
};

void gotoxy(int x, int y);
void setConsoleFontSize(int width, int height);
void setConsolCursorHide(bool isHide);
void setTextColor(unsigned short text);
void setTextColor(int foreground, int background);

#endif