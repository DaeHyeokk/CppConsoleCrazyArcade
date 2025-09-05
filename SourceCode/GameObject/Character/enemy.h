#ifndef __ENEMY_H_
#define __ENEMY_H_

#include "character.h"
#include "enemyState.h"
#include "stateIdle.h"
#include "stateEscapeFromBomb.h"
#include "stateChasePlayer.h"
#include "stateGetItem.h"
#include "stateDistructBlock.h"
#include "statePlaceBomb.h"
#include "stateConfine.h"
#include <memory>
#include <vector>
#include <optional>

#define DEFAULT_CHASE_DISTANCE 3
#define SAFE_RISK 9999
#define RISK_BUFFER 0
#define SAFE_BOMB_TIME 2700
#define EXPLODE_SEC 3000

class Enemy : public Character
{
public:
    std::vector<Position> dir = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };     // UP, DOWN, LEFT, RIGHT πÊ«‚

    Enemy(Position _position, Board& _board, Character* _player);
    virtual ~Enemy();
    void update() override;
    void draw() override;
    void changeState(EnemyState state);
    void resetDangerMap();
    int getPlayerChaseDistance();
    double getMoveDelay();
    double getDangerMapValue(int y, int x);
    Direction dirPosToDirection(Position dirPos);
    std::vector<std::vector<double>>& getDangerMap();
    bool isBlockedTile(Position pos);
    bool isInDanger();


private:
    std::unique_ptr<StateIdle> stateIdle;
    std::unique_ptr<StateEscapeFromBomb> stateEscapeFromBomb;
    std::unique_ptr<StateChasePlayer> stateChasePlayer;
    std::unique_ptr<StateGetItem> stateGetItem;
    std::unique_ptr<StateDistructBlock> stateDistructBlock;
    std::unique_ptr<StatePlaceBomb> statePlaceBomb;
    std::unique_ptr<StateConfine> stateConfine;
    EnemyState enemyState;
    Character* player;
    std::vector<std::vector<double>> dangerMap;

    void updateDangerMap();
    virtual void confine() override;
    virtual void revive() override;
    virtual void die() override;
    void printDangerMap();
    std::string getEnemyStateName();

};


#endif // !__ENEMY_H_