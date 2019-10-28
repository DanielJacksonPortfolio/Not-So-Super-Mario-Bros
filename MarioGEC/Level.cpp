#include "Level.h"
#include <fstream>
#include "Constants.h"
#include "BrickTile.h"
#include "SpecialTile.h"
#include "FloorTile.h"
#include "BlockTile.h"
#include "PipeTile.h"
#include "FlagTile.h"
#include "DecorTile.h"
#include "CoinTile.h"
#include "SpawnTile.h"

Level::Level(int id, std::string levelName, const std::string filePath, SDL_Renderer * renderer, Coordf spawnLocation,bool hasEnd)
{
	this->hasEnd = hasEnd;
	this->renderer = renderer;
	this->spawnLocation = spawnLocation;
	LoadImages();
	this->ID = id;
	this->levelName = levelName;
	this->levelSize.x = levelSizes[ID].a;
	this->levelSize.y = levelSizes[ID].b;
	this->Load(filePath);
	this->Initialize();
}

void Level::LoadImages()
{
	for (size_t i = 0; i < floorTileImages.size(); i++)
	{
		floorTileImages[i]->LoadRenderer(renderer);
		floorTileImages[i]->Load("resources/textures/tiles.png");
	}
	for (size_t i = 0; i < flagTileImages.size(); i++)
	{
		flagTileImages[i]->LoadRenderer(renderer);
		flagTileImages[i]->Load("resources/textures/tiles.png");
	}
	for (size_t i = 0; i < castleTileImages.size(); i++)
	{
		castleTileImages[i]->LoadRenderer(renderer);
		castleTileImages[i]->Load("resources/textures/tiles.png");
	}
	for (size_t i = 0; i < hillTileImages.size(); i++)
	{
		hillTileImages[i]->LoadRenderer(renderer);
		hillTileImages[i]->Load("resources/textures/tiles.png");
	}
	for (size_t i = 0; i < cloudTileImages.size(); i++)
	{
		cloudTileImages[i]->LoadRenderer(renderer);
		cloudTileImages[i]->Load("resources/textures/tiles.png");
	}
	for (size_t i = 0; i < bushTileImages.size(); i++)
	{
		bushTileImages[i]->LoadRenderer(renderer);
		bushTileImages[i]->Load("resources/textures/tiles.png");
	}
	for (size_t i = 0; i < pipeTileImages.size(); i++)
	{
		pipeTileImages[i]->LoadRenderer(renderer);
		pipeTileImages[i]->Load("resources/textures/tiles.png");
	}
	for (size_t i = 0; i < effectImages.size(); i++)
	{
		effectImages[i]->LoadRenderer(renderer);
		effectImages[i]->Load("resources/textures/effects.png");
	}
	brickTileImage->LoadRenderer(renderer);
	brickTileImage->Load("resources/textures/tiles.png");
	blockTileImage->LoadRenderer(renderer);
	blockTileImage->Load("resources/textures/tiles.png");
	for (size_t i = 0; i < specialTileImages.size(); i++)
	{
		specialTileImages[i]->LoadRenderer(renderer);
		specialTileImages[i]->Load("resources/textures/tiles.png");
	}
	for (size_t i = 0; i < coinImages.size(); i++)
	{
		coinImages[i]->LoadRenderer(renderer);
		coinImages[i]->Load("resources/textures/tiles.png");
	}
	flagImage->LoadRenderer(renderer);
	flagImage->Load("resources/textures/effects.png");
}


Level::~Level()
{
	delete data;
	for (size_t i = 0; i < tiles.size(); i++)
	{
		delete tiles[i];
	}
	blockTileImage->destroy();
	brickTileImage->destroy();
	for (size_t i = 0; i < pipeTileImages.size(); i++)
	{
		pipeTileImages[i]->destroy();
	}
	for (size_t i = 0; i < specialTileImages.size(); i++)
	{
		specialTileImages[i]->destroy();
	}
	for (size_t i = 0; i < floorTileImages.size(); i++)
	{
		floorTileImages[i]->destroy();
	}
	for (size_t i = 0; i < flagTileImages.size(); i++)
	{
		flagTileImages[i]->destroy();
	}
	for (size_t i = 0; i < castleTileImages.size(); i++)
	{
		castleTileImages[i]->destroy();
	}
	for (size_t i = 0; i < hillTileImages.size(); i++)
	{
		hillTileImages[i]->destroy();
	}
	for (size_t i = 0; i < cloudTileImages.size(); i++)
	{
		cloudTileImages[i]->destroy();
	}
	for (size_t i = 0; i < bushTileImages.size(); i++)
	{
		bushTileImages[i]->destroy();
	}
	for (size_t i = 0; i < effectImages.size(); i++)
	{
		effectImages[i]->destroy();
	}	
	for (size_t i = 0; i < coinImages.size(); i++)
	{
		coinImages[i]->destroy();
	}
	flagImage->destroy();
	SDL_DestroyRenderer(renderer);
}

void Level::Load(const std::string filePath)
{
	std::ifstream dataFile(filePath);

	data = new char[GetLevelSize(levelSize)];

	if (dataFile.is_open())
	{	
		for (int y = 0; y < levelSize.y; y++ )
		{
			for (int x = 0; x < levelSize.x; x++ )
			{
				dataFile >> data[y * levelSize.x + x];
			}
		}
	}
	else
	{
		std::cout << "File failed to open:" << filePath << std::endl;
	}
	
	dataFile.close();
}

void Level::Initialize()
{
	tiles = {};
	nextLevel = false;
	complete = false;
	for (int y = 0; y < levelSize.y; y++ )
	{
		for (int x = 0; x < levelSize.x; x++ )
		{
			bool grass = false;
			switch (data[y * levelSize.x + x])
			{
			case 'a':
				tiles.push_back(nullptr);
				break;
			case 'b':
				tiles.push_back(new BrickTile({ x, y }, &brickTileImage, &brickTileImage));
				break;
			case 'c':
				tiles.push_back(new SpecialTile({ x, y }, specialTileImages,SpecialTypes::SPECIAL_COIN));
				break;
			case 'd':
				if (y != 0)
				{
					Tile* tile = tiles[(y - 1) * levelSize.x + x];
					if ((tile != nullptr && (tile->collisionType == PASSABLE || tile->collisionType == ITEM)) || tile == nullptr)
						grass = true;
				}

				if (grass)
					tiles.push_back(new FloorTile({ x, y }, &floorTileImages[1]));
				else
					tiles.push_back(new FloorTile({ x, y }, &floorTileImages[0]));

				break;
			case 'e':
				tiles.push_back(new BlockTile({ x, y }, &blockTileImage));
				break;
			case 'f': 

				if (y != 0)
				{
					Tile* tile = tiles[(y - 1) * levelSize.x + x];
					if (tile == nullptr)
						grass = true;
				}
				if (grass)
					tiles.push_back(new FloorTile({ x, y }, &floorTileImages[3], CollisionTypes::PASSABLE));
				else
					tiles.push_back(new FloorTile({ x, y }, &floorTileImages[2], CollisionTypes::PASSABLE));

				break;
			case 'g':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[0]));
				break;
			case 'h':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[1]));
				break;
			case 'i':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[2]));
				break;
			case 'j':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[3]));
				break;
			case 'k':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[4]));
				break;
			case 'l':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[5]));
				break;
			case 'm':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[6]));
				break;
			case 'n':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[7]));
				break;
			case 'o':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[8]));
				break;
			case 'p':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[9]));
				break;
			case 'q':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[10]));
				break;
			case 'r':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[11]));
				break;
			case 's':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[12]));
				break;
			case 't':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[13]));
				break;
			case 'u':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[14]));
				break;
			case 'v':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[15]));
				break;
			case 'w':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[16]));
				break;
			case 'x':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[17]));
				break;
			case 'y':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[18]));
				break;
			case 'z':
				tiles.push_back(new PipeTile({ x, y }, &pipeTileImages[19]));
				break;
			case 'A':
				tiles.push_back(new FlagTile({ x, y }, &flagTileImages[0]));
				break;
			case 'B':
				tiles.push_back(new FlagTile({ x, y }, &flagTileImages[1]));
				flagPosition.x = tiles.back()->GetWorldPosition().x - 8;
				flagPosition.y = tiles.back()->GetWorldPosition().y + 16;
				flagImage->renderQuad = { x*TILESIZE - 8,(y+1)*TILESIZE,16,16 };
				flagLocation = tiles.size()-1;
				break;
			case 'C':
				tiles.push_back(new DecorTile({ x, y }, &castleTileImages[0]));
				break;
			case 'D':
				tiles.push_back(new DecorTile({ x, y }, &castleTileImages[1]));
				break;
			case 'E':
				tiles.push_back(new DecorTile({ x, y }, &castleTileImages[2]));
				break;
			case 'F':
				tiles.push_back(new DecorTile({ x, y }, &castleTileImages[3]));
				break;
			case 'G':
				tiles.push_back(new DecorTile({ x, y }, &castleTileImages[4]));
				break;
			case 'H':
				tiles.push_back(new DecorTile({ x, y }, &castleTileImages[5]));
				break;
			case 'I':
				tiles.push_back(new DecorTile({ x, y }, &brickTileImage));
				break;
			case 'J':
				tiles.push_back(new DecorTile({ x, y }, &cloudTileImages[0]));
				break;
			case 'K':
				tiles.push_back(new DecorTile({ x, y }, &cloudTileImages[1]));
				break;
			case 'L':
				tiles.push_back(new DecorTile({ x, y }, &cloudTileImages[2]));
				break;
			case 'M':
				tiles.push_back(new DecorTile({ x, y }, &cloudTileImages[3]));
				break;
			case 'N':
				tiles.push_back(new DecorTile({ x, y }, &cloudTileImages[4]));
				break;
			case 'O':
				tiles.push_back(new DecorTile({ x, y }, &cloudTileImages[5]));
				break;
			case 'P':
				tiles.push_back(new DecorTile({ x, y }, &bushTileImages[0]));
				break;
			case 'Q':
				tiles.push_back(new DecorTile({ x, y }, &bushTileImages[1]));
				break;
			case 'R':
				tiles.push_back(new DecorTile({ x, y }, &bushTileImages[2]));
				break;
			case 'S':
				tiles.push_back(new DecorTile({ x, y }, &hillTileImages[0]));
				break;
			case 'T':
				tiles.push_back(new DecorTile({ x, y }, &hillTileImages[1]));
				break;
			case 'U':
				tiles.push_back(new DecorTile({ x, y }, &hillTileImages[2]));
				break;
			case 'V':
				tiles.push_back(new DecorTile({ x, y }, &hillTileImages[3]));
				break;
			case 'W':
				tiles.push_back(new DecorTile({ x, y }, &hillTileImages[4]));
				break;
			case 'X':
				tiles.push_back(new CoinTile({ x, y }, coinImages));
				break;
			case 'Y':
				tiles.push_back(new SpawnTile({ x, y }, Enemies::ENEMY_GOOMBA));
				numberOfEnemies++;
				break;	
			case 'Z':
				tiles.push_back(new SpecialTile({ x, y }, specialTileImages, SpecialTypes::SPECIAL_POWERUP));
				break;
			case '0':
				tiles.push_back(new SpecialTile({ x, y }, specialTileImages, SpecialTypes::SPECIAL_1UP));
				break;
			case '1':
				tiles.push_back(new BrickTile({ x, y }, &brickTileImage, &specialTileImages[0], BrickTypes::BRICK_COIN));
				break;
			case '2':
				tiles.push_back(new BrickTile({ x, y }, &brickTileImage, &specialTileImages[0], BrickTypes::BRICK_STAR));
				break;
			case '3':
				tiles.push_back(new SpawnTile({ x, y }, Enemies::ENEMY_KOOPA));
				numberOfEnemies++;
				break;
			default:
				tiles.push_back(nullptr);
				break;
			}

		}
	}
}

void Level::Update(int elapsedTime)
{
	for (size_t i = 0; i < tiles.size(); i++)
	{
		if (tiles[i] != nullptr)
		{
			tiles[i]->Update(elapsedTime);
		}
	}
}

void Level::Draw(Camera * camera, double angle, SDL_RendererFlip flip)
{
	for (size_t i = 0; i < tiles.size(); i++)
	{
		if (tiles[i] != nullptr)
		{
			if (tiles[i]->GetWorldPosition().x <= camera->worldPosition.x + WINDOW_WIDTH * 0.5f - tiles[i]->GetWidth() * 0.5f && tiles[i]->GetWorldPosition().x >= camera->worldPosition.x - WINDOW_WIDTH * 0.5f + tiles[i]->GetWidth() * 0.5f)
			{
				if (tiles[i]->GetWorldPosition().y <= camera->worldPosition.y + WINDOW_HEIGHT * 0.5f - tiles[i]->GetHeight() * 0.5f && tiles[i]->GetWorldPosition().y >= camera->worldPosition.y - WINDOW_HEIGHT * 0.5f + tiles[i]->GetHeight() * 0.5f)
				{
					tiles[i]->Draw(camera, angle, flip);
				}
			}
		}
		if (!complete && i == flagLocation)
		{
			flagImage->UpdatePosition({ static_cast<int>((tiles[i]->worldPosition.x - camera->worldPosition.x - tiles[i]->GetWidth()) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((tiles[i]->worldPosition.y + 16 - camera->worldPosition.y - tiles[i]->GetHeight() * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(tiles[i]->GetWidth() * camera->zoom), static_cast<int>(tiles[i]->GetHeight() * camera->zoom) });
			flagImage->Draw(angle, flip);
		}
	}

}

void Level::Draw(Camera * camera)
{
	for (size_t i = 0; i < tiles.size(); i++ )
	{
		if (tiles[i] != nullptr)
		{
			if (tiles[i]->GetWorldPosition().x - (tiles[i]->GetWidth() * 0.5f * camera->zoom) < camera->worldPosition.x + (WINDOW_WIDTH * 0.5f / camera->zoom))
			{
				if (tiles[i]->GetWorldPosition().y - (tiles[i]->GetHeight() * 0.5f * camera->zoom) < camera->worldPosition.y + (WINDOW_HEIGHT * 0.5f / camera->zoom))
				{
					tiles[i]->Draw(camera);
				}
			}
		}
		if (!complete && i == flagLocation)
		{
			flagImage->UpdatePosition({ static_cast<int>((tiles[i]->worldPosition.x - camera->worldPosition.x - tiles[i]->GetWidth()) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((tiles[i]->worldPosition.y + 16 - camera->worldPosition.y - tiles[i]->GetHeight() * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(tiles[i]->GetWidth() * camera->zoom), static_cast<int>(tiles[i]->GetHeight() * camera->zoom) });
			flagImage->Draw();
		}
	}
}

void Level::DrawDebug(Camera * camera, bool showTiles, bool showTilePlayerRelationship, Coordf playerWorldPosition)
{
	if (showTiles)
	{
		for (size_t i = 0; i < this->tiles.size(); i++)
		{
			if (this->tiles[i] != nullptr)
			{
				if (this->tiles[i]->GetCollisionType() == IMPASSABLE)
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				else
					SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
				SDL_Rect tileRect = { static_cast<int>((this->tiles[i]->GetWorldPosition().x - this->tiles[i]->GetWidth() * 0.5f - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->tiles[i]->GetWorldPosition().y - camera->worldPosition.y - this->tiles[i]->GetHeight() * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(this->tiles[i]->GetWidth() * camera->zoom), static_cast<int>(this->tiles[i]->GetHeight() * camera->zoom) };
				SDL_RenderDrawRect(renderer, &tileRect);
				SDL_RenderDrawLine(renderer, static_cast<int>((this->tiles[i]->GetWorldPosition().x - this->tiles[i]->GetWidth() * 0.5f - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->tiles[i]->GetWorldPosition().y - camera->worldPosition.y - this->tiles[i]->GetHeight() * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>((this->tiles[i]->GetWorldPosition().x + this->tiles[i]->GetWidth() * 0.5f - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->tiles[i]->GetWorldPosition().y - camera->worldPosition.y + this->tiles[i]->GetHeight() * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f));
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				if (showTilePlayerRelationship)
					SDL_RenderDrawLine(renderer, static_cast<int>((playerWorldPosition.x - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((playerWorldPosition.y - camera->worldPosition.y) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>((this->tiles[i]->GetWorldPosition().x - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->tiles[i]->GetWorldPosition().y - camera->worldPosition.y) * camera->zoom + WINDOW_HEIGHT * 0.5f));
			}
		}
	}
}