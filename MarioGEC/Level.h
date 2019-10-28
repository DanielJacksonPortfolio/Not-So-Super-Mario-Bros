#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include "Tile.h"
#include "Image.h"
#include "Camera.h"

class Level
{
private:
	int ID = 0;
	char * data = {};
	SDL_Renderer * renderer;

	Image * brickTileImage = new Image({ 0, 0, 16, 16 });
	Image * blockTileImage = new Image({ 112, 0, 16, 16 });

	std::vector<Image*> specialTileImages = 
	{
		new Image({ 0, 64, 16, 16 }),
		new Image({ 16, 64, 16, 16 }),
		new Image({ 32, 64, 16, 16 }),
		new Image({ 48, 64, 16, 16 }),
		new Image({ 64, 64, 16, 16 }),
		new Image({ 80, 64, 16, 16 }),
	};

	std::vector<Image*> coinImages =
	{
		new Image({96,64,16,16}),
		new Image({112,64,16,16}),
		new Image({128,64,16,16}),
		new Image({144,64,16,16}),
	};


	std::vector<Image*> floorTileImages = 
	{
		new Image({ 16, 0, 16, 16 }),
		new Image({ 32, 0, 16, 16 }),
		new Image({ 48, 0, 16, 16 }),
		new Image({ 64, 0, 16, 16 })
	};

	std::vector<Image*> flagTileImages = 
	{
		new Image({ 80, 0, 16, 16 }),
		new Image({ 96, 0, 16, 16 })
	};

	std::vector<Image*> castleTileImages = 
	{
		new Image({ 0, 48, 16, 16 }),
		new Image({ 16, 48, 16, 16 }),
		new Image({ 32, 48, 16, 16 }),
		new Image({ 48, 48, 16, 16 }),
		new Image({ 64, 48, 16, 16 }),
		new Image({ 80, 48, 16, 16 })
	};

	std::vector<Image*> hillTileImages =
	{
		new Image({ 0, 112, 16, 16 }),
		new Image({ 16, 112, 16, 16 }),
		new Image({ 32, 112, 16, 16 }),
		new Image({ 48, 112, 16, 16 }),
		new Image({ 64, 112, 16, 16 })
	};

	std::vector<Image*> cloudTileImages =
	{
		new Image({ 0, 80, 16, 16 }),
		new Image({ 16, 80, 16, 16 }),
		new Image({ 32, 80, 16, 16 }),
		new Image({ 48, 80, 16, 16 }),
		new Image({ 64, 80, 16, 16 }),
		new Image({ 80, 80, 16, 16 })
	};

	std::vector<Image*> bushTileImages =
	{
		new Image({ 0, 96, 16, 16 }),
		new Image({ 16, 96, 16, 16 }),
		new Image({ 32, 96, 16, 16 })
	};

	std::vector<Image*> pipeTileImages = 
	{
		new Image({ 64, 16, 16, 16 }),
		new Image({ 80, 16, 16, 16 }),
		new Image({ 0, 16, 16, 16 }),
		new Image({ 16, 16, 16, 16 }),
		new Image({ 64, 32, 16, 16 }),
		new Image({ 80, 32, 16, 16 }),
		new Image({ 0, 32, 16, 16 }),
		new Image({ 16, 32, 16, 16 }),
		new Image({ 32, 16, 16, 16 }),
		new Image({ 32, 32, 16, 16 }),
		new Image({ 48, 16, 16, 16 }),
		new Image({ 48, 32, 16, 16 }),
		new Image({ 96, 16, 16, 16 }),
		new Image({ 112, 16, 16, 16 }),
		new Image({ 96, 32, 16, 16 }),
		new Image({ 112, 32, 16, 16 }),
		new Image({ 128, 16, 16, 16 }),
		new Image({ 144, 16, 16, 16 }),
		new Image({ 128, 32, 16, 16 }),
		new Image({ 144, 32, 16, 16 })
	};

	Image* flagImage = new Image({ 0,32,16,16 });
	int flagLocation = -1;

	//Image * mushroomImage = new Image({0,0,16,16});

public:
	SDL_Point levelSize;
	std::vector<Tile * > tiles;

	Level(int id, std::string levelName, const std::string filePath, SDL_Renderer * renderer, Coordf spawnLocation,bool hasEnd = true);
	~Level();
	void Load(const std::string filePath);
	void Draw(Camera * camera);
	void DrawDebug(Camera * camera, bool showTiles, bool showTilePlayerRelationship, Coordf playerWorldPosition);
	void Draw(Camera * camera, double angle, SDL_RendererFlip flip);
	void Initialize();
	void LoadImages();
	void Update(int elapsedTime);
	int numberOfEnemies = 0;
	Coordf spawnLocation = {0,0};
	bool hasEnd = false;

	std::vector<Image*> effectImages = {
	new Image({0,0,16,16 }),
	new Image({ 0,16,16,16 }),
	new Image({ 16,16,16,16 }),
	new Image({ 32,16,16,16 }),
	new Image({ 48,16,16,16 }),
	new Image({16,0,8,8 }),
	new Image({16,8,8,8 }),
	new Image({24,0,8,8 }),
	new Image({24,8,8,8 }),
	new Image({0,32,16,16 }),
	new Image({0,48,16,32 }),
	new Image({16,48,16,32 }),
	new Image({32,48,16,32 }),
	new Image({48,48,16,32 }),
	new Image({64,48,16,32 }),
	new Image({0,80,16,16 }),
	new Image({16,80,16,16 }),
	new Image({32,80,16,16 }),
	new Image({48,80,16,16 }),
	new Image({64,80,16,16 }),
	new Image({64,32,16,16 }),
	};

	bool complete = false;
	bool nextLevel = false;
	std::string levelName = "";
	Coordf flagPosition = { 0,0 };
};

