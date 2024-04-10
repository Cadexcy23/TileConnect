#pragma once
#ifndef vector
#include <vector>
#endif
#ifndef Tile
#include "Tile.h"
#endif
#ifndef Object
#include "Object.h"
#endif
#ifndef Entity
#include "Entity.h"
#endif


class Map {
public:
	struct map {
		std::vector< std::vector<Tile::tile>> tileGrid;
		std::vector< std::vector<Object::object>> objectGrid;
		std::vector<Entity::entity> entityList;
	};

	static map activeMap;
	static float mapZoomGoal;

	void init();
	void controller();
	void draw();
};