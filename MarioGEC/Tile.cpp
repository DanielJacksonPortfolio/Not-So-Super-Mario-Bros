#include "Tile.h"



Tile::Tile()
{
	this->gridPosition = gridPosition;
	this->worldPosition = Coordf(gridPosition.x * width + width * 0.5f, gridPosition.y * height + height * 0.5f);
	this->hitbox = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height };
}


Tile::~Tile()
{
	if (image != nullptr)
	{
		image->destroy();
	}
}

void Tile::UpdateImage(Camera * camera)
{
	if (image != nullptr)
	{
		image->UpdatePosition(screenQuadCalculation);
	}
}

void Tile::Draw(Camera * camera)
{
	if (image != nullptr)
	{
		UpdateImage(camera);
		image->Draw();
	}
}

void Tile::Draw(Camera * camera, double angle, SDL_RendererFlip flip)
{
	if (image != nullptr)
	{
		UpdateImage(camera);
		image->Draw(angle, flip);
	}
}

int Tile::ResolveCollision(int direction)
{
	if (direction == LEFT)
		std::cout << "TILE LEFT" << std::endl;
	if (direction == RIGHT)
		std::cout << "TILE RIGHT" << std::endl;
	if (direction == UP)
		std::cout << "TILE UP" << std::endl;
	if (direction == DOWN)
		std::cout << "TILE DOWN" << std::endl;

	return NULL;
}


SDL_Rect const  Tile::GetHitbox()
{
	return hitbox;
}

Coordf const Tile::GetWorldPosition()
{
	return worldPosition;
}

Coordi const  Tile::GetGridPosition()
{
	return gridPosition;
}

int const Tile::GetCollisionType()
{
	return collisionType;
}

void Tile::Update(int elapsedTime)
{

}

void Tile::Animate(int elapsedTime)
{

}

int const Tile::GetWidth()
{
	return width;
}

int const Tile::GetHeight()
{
	return height;
}
