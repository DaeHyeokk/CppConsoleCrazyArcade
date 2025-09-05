#ifndef __ENEMY_STATE_H_
#define __ENEMY_STATE_H_

#include <string>

enum class EnemyState
{
    IDLE,
    CHASE_PLAYER,
    GET_ITEM,
    DISTRUCT_BLOCK,
    PLACE_BOMB,
    ESCAPE_FROM_BOMB,
    CONFINE
};

#endif // !__ENEMY_STATE_H_
