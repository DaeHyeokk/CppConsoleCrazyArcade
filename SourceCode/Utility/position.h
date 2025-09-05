#ifndef __POSITION_H_
#define __POSITION_H_

struct Position
{
	int y;
	int x;

    bool operator == (const Position& rPos)
    {
        return this->y == rPos.y && this->x == rPos.x;
    }

    Position operator+(const Position& rpos)
    {
        Position temp;
        temp.y = y + rpos.y;
        temp.x = x + rpos.x;
        return temp;
    }

    Position operator-(const Position& rpos)
    {
        Position temp;
        temp.y = y - rpos.y;
        temp.x = x - rpos.x;
        return temp;
    }
};

#endif