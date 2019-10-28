#pragma once
#include "Tile.h"
#include <string>

class PipeTile :public Tile
{
public:

	PipeTile(Coordi gridPosition, Image * * image);
	virtual int ResolveCollision(int direction) override;
};

