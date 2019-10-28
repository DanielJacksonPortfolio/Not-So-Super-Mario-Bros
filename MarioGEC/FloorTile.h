#pragma once
#include "Tile.h"
#include <string>

class FloorTile :public Tile
{
public:
	FloorTile(Coordi gridPosition, Image * * image, int = CollisionTypes::IMPASSABLE);
	int ResolveCollision(int direction) override;
};

