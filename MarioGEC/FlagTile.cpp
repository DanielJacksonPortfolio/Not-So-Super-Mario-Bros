#include "FlagTile.h"

FlagTile::FlagTile(Coordi gridPosition, Image * * image)
{
	this->collisionType = PASSABLE;
	defaultTileConstructer
}

int FlagTile::ResolveCollision(int direction)
{
	if (direction == LEFT)
		std::cout << "Flag LEFT" << std::endl;
	if (direction == RIGHT)
		std::cout << "Flag RIGHT" << std::endl;
	if (direction == UP)
		std::cout << "Flag UP" << std::endl;
	if (direction == DOWN)
		std::cout << "Flag DOWN" << std::endl;
	return Collisions::COLLISION_FLAG;
}