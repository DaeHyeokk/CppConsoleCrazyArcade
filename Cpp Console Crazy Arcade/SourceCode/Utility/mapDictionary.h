#ifndef __MAPDICTIONARY_H_
#define __MAPDICTIONARY_H_

#include <vector>
#include <unordered_map>

enum MapNameEnum
{
	PIRATE,
	FACTORY,
	FOREST
};

class MapDictionary
{
	std::unordered_map<MapNameEnum, std::vector<std::vector<int>>> mapData;

public:
	MapDictionary();
	std::vector<std::vector<int>> getMapData(MapNameEnum mapName);
};
#endif // !__MAPLIST_H_
