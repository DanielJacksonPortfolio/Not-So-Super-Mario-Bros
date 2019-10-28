#pragma once
#include "Tile.h"
class CoinTile :public Tile
{
public:

	CoinTile(Coordi gridPosition, std::vector<Image*> images);
	int ResolveCollision(int direction) override;
	void Update(int elapsedTime) override;
	void Animate(int elapsedTime) override;

	std::vector<Image*> activeImages;

	int animationTimer = 0;
	int currentFrame = 0;
	int counter = 2;

};