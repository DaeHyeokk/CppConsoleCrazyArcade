#ifndef __RANDOM_UTIL_H_
#define __RANDOM_UTIL_H_

#pragma once
#include <random>
#include <vector>
#include <algorithm>
#include "direction.h"

// 전역에서 공유할 RNG 엔진
inline std::mt19937& globalRNG() 
{
	static std::mt19937 gen(std::random_device{}());
	return gen;
}

inline int getRandomInt(int minVal, int maxVal) 
{
	std::uniform_int_distribution<int> dist(minVal, maxVal);
	return dist(globalRNG());
}

// 방향을 랜덤하게 섞은 벡터 반환
inline std::vector<Direction> getShuffledDirections() 
{
	std::vector<Direction> directions = { UP, DOWN, LEFT, RIGHT };
	std::shuffle(directions.begin(), directions.end(), globalRNG());
	return directions;
}

#endif // !__RANDOM_UTIL_H_
