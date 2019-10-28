#pragma once
#include "Tile.h"

class BrickTile:public Tile
{
public:

	BrickTile(Coordi gridPosition, Image * * image, Image * * uimage, int = 0);
	int ResolveCollision(int direction) override;

	Image * unactiveImage = nullptr;
	int type = 0;
	int numCoins = 0;
};

