#include "Collider.h"



Collider::Collider()
{
}


Collider::~Collider()
{
}

void Collider::LevelCollision(SDL_Rect hitboxA, SDL_Rect hitboxB,Coordf positionA, Coordf positionB, std::vector<std::array<int, 2>> & XCollisions, std::vector<std::array<int, 2>> & YCollisions, int index)
{
	if (BoundingBox(hitboxA, hitboxB))
	{
		float const deltaX = positionB.x - positionA.x;
		float const deltaY = positionB.y - positionA.y;

		if (abs(deltaX) > abs(deltaY))
			XCollisions.push_back({ static_cast<int>(index), static_cast<int>(deltaX) });
		else
			YCollisions.push_back({ static_cast<int>(index), static_cast<int>(deltaY) });
	}
}

bool Collider::BoundingBox(SDL_Rect primaryRect, SDL_Rect secondaryRect)
{
	if (primaryRect.x + primaryRect.w > secondaryRect.x && primaryRect.x < secondaryRect.x + secondaryRect.w && primaryRect.y < secondaryRect.y + secondaryRect.h && primaryRect.y + primaryRect.h > secondaryRect.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
