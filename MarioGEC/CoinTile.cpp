#include "CoinTile.h"

CoinTile::CoinTile(Coordi gridPosition, std::vector<Image*> images)
{
	this->collisionType = ITEM;
	this->gridPosition = gridPosition;
	this->worldPosition = Coordf(gridPosition.x * width + width * 0.5f, gridPosition.y * height + height * 0.5f);
	this->activeImages = { images[0],images[1],images[2],images[3]};
	this->image = activeImages[0];

	this->hitbox = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height };
}

void CoinTile::Update(int elapsedTime)
{
	Animate(elapsedTime);
}

void CoinTile::Animate(int elapsedTime)
{
	animationTimer += elapsedTime;
	if (animationTimer >= 100)
	{
		currentFrame++;
		currentFrame %= 7;
		int imageFrame = 0;
		if (currentFrame <= 3)
			imageFrame = currentFrame;
		else
			imageFrame = 7 - currentFrame;
		image = activeImages[imageFrame];
		animationTimer = 0;
	}
}

int CoinTile::ResolveCollision(int direction)
{
	if (direction == Directions::LEFT)
		std::cout << "COIN LEFT" << std::endl;
	if (direction == Directions::RIGHT)
		std::cout << "COIN RIGHT" << std::endl;
	if (direction == Directions::UP)
		std::cout << "COIN UP" << std::endl;
	if (direction == Directions::DOWN)
		std::cout << "COIN DOWN" << std::endl;

	return Collisions::COLLISION_COIN;
}