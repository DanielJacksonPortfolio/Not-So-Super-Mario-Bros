#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <functional>

#include "Constants.h"
#include "Image.h"
#include "Player.h"
#include "Enemy.h"
#include "Level.h"
#include "Camera.h"
#include "Text.h"
#include "Box.h"
#include "Effect.h"
#include "Item.h"
#include "SoundObject.h"

class Game
{
private:
	SDL_Window * window = nullptr;
	SDL_Renderer * renderer = nullptr;
	int viewWidth = 0;
	int viewHeight = 0;
	int preferredFPS = 60;

	Image* mainMenu = new Image({ 0,0,192,108 });

	SDL_Point mousePos = { 0 , 0 };
	Uint32 elapsedTime = 0 , oldTime = 0 ;

	Player * player = nullptr;
	Camera * camera = nullptr;

	std::vector<Level * > levels;

	int counter = 0;

	//Key bools

	bool debugPressed = false;
	bool debug = false;
	bool ePressed = false;
	bool qPressed = false;
	bool xPressed = false;
	bool cPressed = false;

	SoundObject* music = nullptr;
	SoundObject* jump = nullptr;
	SoundObject* marioDeath = nullptr;
	SoundObject* enemyDeath = nullptr;
	SoundObject* coin = nullptr;
	SoundObject* powerup = nullptr;
	SoundObject* oneUp = nullptr;
	SoundObject* powerupSpawn = nullptr;
	SoundObject* brick = nullptr;

	//Debug bools

	bool showTilePlayerRelationship = false;
	bool showMenus = true;
	bool showPlayerGridPos = false;
	bool showCenterLines = false;
	bool showPlayerSides = false;
	bool showPlayer = true;
	bool showTiles = true;
	bool showEnemies = true;
	bool showItems = true;
	bool marioDead = false;

	struct Scene
	{
		int ID;
		std::vector<Box*> boxes = {};
		Scene(int id) { this->ID = id; }
		~Scene() { for (size_t i = 0; i < boxes.size(); i++) { delete boxes[i]; } }
	};

	std::vector<std::string> fonts = {};

	std::vector<Scene> scenes = {};

	int fpsTarget = 1000;
	int elapsedTarget = 1000 / fpsTarget;

	std::vector<Enemy*> enemies = {};
	std::vector<Item*> items = {};
	std::vector<Effect*> effects = {};
	std::vector<SoundObject**> sounds = {};

	bool transition = false;

public:
	Game(int argc, char * args[]);
	~Game();
	bool InitSDL();
	void CloseSDL();
	bool Input();
	void Draw();
	bool Update();
	bool LoadContent();
	void LoadLevels();
	void LoadScenes();
	void LoadBoxes();
	void StartGame();
	void LoadSounds();
	void LoadLevel(int levelID);

	int currentLevel = Levels::ONEONE;
	int currentScene = Scenes::MAINMENU;

};