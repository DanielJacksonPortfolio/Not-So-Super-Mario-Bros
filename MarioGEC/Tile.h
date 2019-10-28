#pragma once
#include "Image.h"
#include "Constants.h"
#include "Camera.h"

class Tile
{
public:
	Tile();
	~Tile();
	void virtual Draw(Camera * camera);
	void Draw(Camera * camera, double angle, SDL_RendererFlip flip);
	void UpdateImage(Camera * camera);

	void virtual SetUnactive() {}

	Coordf const GetWorldPosition();
	Coordi const GetGridPosition();
	int const GetCollisionType();
	int const GetWidth();
	int const GetHeight();
	SDL_Rect const GetHitbox();

	virtual int ResolveCollision(int direction);
	virtual void Update(int elapsedTime);
	virtual void Animate(int elapsedTime);


	Image * image = nullptr;
	int collisionType = IMPASSABLE;
	Coordf worldPosition = Coordf(0.0f, 0.0f);
	Coordi gridPosition = Coordi(0, 0);
	int width = TILESIZE, height = TILESIZE;
	SDL_Rect hitbox = { 0,0,0,0 };
	bool active = true;
	int enemyType = 0;

};

