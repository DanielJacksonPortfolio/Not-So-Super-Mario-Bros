#pragma once
#include "Tile.h"
#include <string>

class FlagTile :public Tile
{
private:

public:

	FlagTile(Coordi gridPosition, Image * * image);
	int ResolveCollision(int direction) override;
};

