#pragma once
#include "Tile.h"

class BlockTile :public Tile
{
public:

	BlockTile(Coordi gridPosition, Image * * image);
	int ResolveCollision(int direction) override;
};

