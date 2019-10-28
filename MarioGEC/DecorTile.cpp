#include "DecorTile.h"

DecorTile::DecorTile(Coordi gridPosition, Image * * image)
{
	this->collisionType = PASSABLE;

	defaultTileConstructer
}

int DecorTile::ResolveCollision(int direction)
{
	if (direction == LEFT)
		std::cout << "DECOR LEFT" << std::endl;
	if (direction == RIGHT)
		std::cout << "DECOR RIGHT" << std::endl;
	if (direction == UP)
		std::cout << "DECOR UP" << std::endl;
	if (direction == DOWN)
		std::cout << "DECOR DOWN" << std::endl;
	return Collisions::COLLISION_DECOR;
}