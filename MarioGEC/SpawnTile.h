#pragma once
#include "Tile.h"
#include <string>

class SpawnTile :public Tile
{
public:
	SpawnTile(Coordi gridPosition,int enemyType, int = CollisionTypes::PASSABLE);
	int ResolveCollision(int direction) override;
};

