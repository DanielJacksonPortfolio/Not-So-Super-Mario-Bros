#pragma once
#include "Tile.h"
class SpecialTile :public Tile
{
public:

	SpecialTile(Coordi gridPosition, std::vector<Image*> images, int type);
	~SpecialTile();
	int ResolveCollision(int direction) override;
	void Update(int elapsedTime) override;
	void Animate(int elapsedTime) override;
	void SetUnactive() override;

	std::vector<Image*> activeImages;
	Image * unactiveImage;

	int animationTimer = 0;
	int currentFrame = 0;
	int counter = 2;
	int type = 0;
};