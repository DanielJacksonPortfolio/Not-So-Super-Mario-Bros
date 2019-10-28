#include "FloorTile.h"

FloorTile::FloorTile(Coordi gridPosition, Image * * image, int collisionType)
{
	this->collisionType = collisionType;

	defaultTileConstructer
}

int FloorTile::ResolveCollision(int direction)
{
	if (direction == Directions::LEFT)
		std::cout << "FLOOR LEFT" << std::endl;
	if (direction == Directions::RIGHT)
		std::cout << "FLOOR RIGHT" << std::endl;
	if (direction == Directions::UP)
		std::cout << "FLOOR UP" << std::endl;
	if (direction == Directions::DOWN)
		std::cout << "FLOOR DOWN" << std::endl;
	return Collisions::COLLISION_FLOOR;
}