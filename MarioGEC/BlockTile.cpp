#include "BlockTile.h"

BlockTile::BlockTile(Coordi gridPosition, Image * * image)
{
	defaultTileConstructer
}
int BlockTile::ResolveCollision(int direction)
{
	if (direction == LEFT)
		std::cout << "BLOCK LEFT" << std::endl;
	if (direction == RIGHT)
		std::cout << "BLOCK RIGHT" << std::endl;
	if (direction == UP)
		std::cout << "BLOCK UP" << std::endl;
	if (direction == DOWN)
		std::cout << "BLOCK DOWN" << std::endl;
	return Collisions::COLLISION_BLOCK;
}