#include "flamePool.h"
#include "flame.h"
FlamePool::FlamePool(Board& _board) : board(_board)
{
}

void FlamePool::init()
{
	for (int i = 0; i < 10; i++)
	{
		flameQ.push(std::move(std::make_unique<Flame>(Position{ POOL_OBJECT_POS, POOL_OBJECT_POS }, board, this->shared_from_this())));
	}
}

std::unique_ptr<Flame> FlamePool::getObject()
{
	if (flameQ.empty())
		return std::make_unique<Flame>(Position{ POOL_OBJECT_POS, POOL_OBJECT_POS }, board, this->shared_from_this());
	else
	{
		std::unique_ptr<Flame> flame = std::move(flameQ.front());
		flameQ.pop();
		return flame;
	}
}

void FlamePool::returnObject(std::unique_ptr<Flame> retObj)
{
	flameQ.push(std::move(retObj));
}

