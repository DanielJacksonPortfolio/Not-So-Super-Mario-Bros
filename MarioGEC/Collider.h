#pragma once
#include "Level.h"
#include <vector>
#include "Effect.h"
#include "SoundObject.h"
#include "Constants.h"

class Collider
{
public:
	Collider();
	~Collider();
	
	void LevelCollision(SDL_Rect hitboxA, SDL_Rect hitboxB, Coordf positionA, Coordf positionB, std::vector<std::array<int, 2>>& XCollisions, std::vector<std::array<int, 2>>& YCollisions, int index);
	void Circle();
	bool BoundingBox(SDL_Rect primaryRect, SDL_Rect secondaryRect);
};

