#include "PipeTile.h"

PipeTile::PipeTile(Coordi gridPosition, Image * * image)
{
	defaultTileConstructer
}

int PipeTile::ResolveCollision(int direction)
{
	if (direction == LEFT)
		std::cout << "PIPE LEFT" << std::endl;
	if (direction == RIGHT)
		std::cout << "PIPE RIGHT" << std::endl;
	if (direction == UP)
		std::cout << "PIPE UP" << std::endl;
	if (direction == DOWN)
		std::cout << "PIPE DOWN" << std::endl;
	return Collisions::COLLISION_PIPE;
}
