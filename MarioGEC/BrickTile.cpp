#include "BrickTile.h"

BrickTile::BrickTile(Coordi gridPosition, Image * * image, Image * * uimage, int type)
{
	defaultTileConstructer

	unactiveImage = *uimage;
	this->type = type;
	this->active = true;
	if (type == BrickTypes::BRICK_COIN)
		numCoins = 7;
	if (type == BrickTypes::BRICK_STAR)
		numCoins = 1;
}

int BrickTile::ResolveCollision(int direction)
{
	if (direction == LEFT)
		std::cout << "BRICK LEFT" << std::endl;
	if (direction == RIGHT)
		std::cout << "BRICK RIGHT" << std::endl;
	if (direction == UP)
	{
		if (type == BrickTypes::BRICK_COIN)
		{
			if (numCoins > 0)
				numCoins--;
			if (numCoins == 0)
			{
				this->active = false;
				if (image != unactiveImage)
					image = unactiveImage;
			}
		}
		if (type == BrickTypes::BRICK_STAR)
		{
			if (image != unactiveImage)
				image = unactiveImage;
		}
		std::cout << "BRICK UP" << std::endl;
	}
	if (direction == DOWN)
		std::cout << "BRICK DOWN" << std::endl;

	if (type == BrickTypes::BRICK_NORMAL)
		return Collisions::COLLISION_BRICK;
	else if (type == BrickTypes::BRICK_COIN)
		return Collisions::COLLISION_BRICK_COIN;
	else if (type == BrickTypes::BRICK_STAR)
		return Collisions::COLLISION_BRICK_STAR;
}