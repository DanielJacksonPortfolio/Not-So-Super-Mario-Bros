#pragma once
#include "Tile.h"
#include <string>

class DecorTile :public Tile
{
public:

	DecorTile(Coordi gridPosition, Image * * image);
	int ResolveCollision(int direction) override;
};

