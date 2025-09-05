#ifndef __FLAMEPOOL_H_
#define __FLAMEPOOL_H_

#include "gameObject.h"
#include "board.h"
#include <queue>
#include <memory>

#define POOL_OBJECT_POS 0

class Flame;

class FlamePool : public std::enable_shared_from_this<FlamePool>
{
	std::queue<std::unique_ptr<Flame>> flameQ;
	Board& board;

public:
	FlamePool(Board& _board);
	void init();
	std::unique_ptr<Flame> getObject();
	void returnObject(std::unique_ptr<Flame> retObj);
};

#endif // !__FLAMEPOOL_H_