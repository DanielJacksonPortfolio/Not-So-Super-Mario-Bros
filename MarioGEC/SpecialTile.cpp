#include "SpecialTile.h"


SpecialTile::~SpecialTile()
{
	if (image != nullptr)
		image->destroy();
	for (size_t i = 0; i < activeImages.size(); i++)
	{
		activeImages[i]->destroy();
	}
	unactiveImage->destroy();
}
SpecialTile::SpecialTile(Coordi gridPosition, std::vector<Image*> images, int type)
{
	this->gridPosition = gridPosition;
	this->worldPosition = Coordf(gridPosition.x * width + width * 0.5f, gridPosition.y * height + height * 0.5f);
	if (type != SpecialTypes::SPECIAL_1UP)
		this->activeImages = { images[1],images[2],images[3],images[4],images[5] };
	this->unactiveImage = images[0];
	if (activeImages.size() > 0)
		this->image = activeImages[0];
	this->type = type;
	this->hitbox = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height };

	if (type == SpecialTypes::SPECIAL_1UP)
		this->collisionType = CollisionTypes::ITEM;
}

void SpecialTile::Update(int elapsedTime)
{
	Animate(elapsedTime);
}

void SpecialTile::Animate(int elapsedTime)
{
	if (active)
	{
		animationTimer += elapsedTime;
		if (animationTimer >= 100)
		{
			currentFrame++;
			currentFrame %= 8;
			int imageFrame = 0;
			if (currentFrame <= 4)
				imageFrame = currentFrame;
			else
				imageFrame = 8 - currentFrame;
			if (activeImages.size() > imageFrame)
				image = activeImages[imageFrame];
			animationTimer = 0;
		}
	}
}

void SpecialTile::SetUnactive()
{
	this->active = false;
	this->collisionType = CollisionTypes::IMPASSABLE;
	if (image != unactiveImage)
	{
		image = unactiveImage;
	}
}

int SpecialTile::ResolveCollision(int direction)
{
	//if (direction == LEFT)
	//	std::cout << "SPECIAL LEFT" << std::endl;
	//if (direction == RIGHT)
	//	std::cout << "SPECIAL RIGHT" << std::endl;
	//if (direction == UP)
	//{
	//	std::cout << "SPECIAL UP" << std::endl;
	//}
	//if (direction == DOWN)
	//	std::cout << "SPECIAL DOWN" << std::endl;

	if (type == SpecialTypes::SPECIAL_COIN)
		return Collisions::COLLISION_SPECIAL_COIN;
	else if (type == SpecialTypes::SPECIAL_1UP)
		return Collisions::COLLISION_SPECIAL_1UP;
	else
		return Collisions::COLLISION_SPECIAL_POWERUP;
}