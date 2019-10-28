#include "Game.h"

Game::Game(int argc, char * args[])
{
	if (InitSDL())
	{
		if (LoadContent())
		{
			SDL_GetTicks();
			oldTime = SDL_GetTicks();
			while (Update());
		}
	}

	CloseSDL();
}

Game::~Game()
{
	delete camera;
	delete player;
	for (size_t i = 0; i < levels.size(); i++)
		delete levels[i];
	for (size_t i = 0; i < scenes.size(); i++)
		scenes[i].~Scene();
	for (size_t i = 0; i < enemies.size(); i++)
		delete enemies[i];
	for (size_t i = 0; i < effects.size(); i++)
		delete effects[i];
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	/*for (size_t i = 0; i < sounds.size(); i++)
	{
		delete sounds[i];
	}*/
}

bool Game::InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL did not initialise. Error: " << SDL_GetError();
		return false;
	}
	if (TTF_Init() < 0)
	{
		std::cout << "SDL_TTF did not initialise. Error: " << SDL_GetError();
		return false;
	}
	window = SDL_CreateWindow("Not So Super Mario Bros", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cout << "Window was not created. Error: " << SDL_GetError();
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		std::cout << "Renderer was not created. Error: " << SDL_GetError();
		return false;
	}
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		std::cout << "Mixer WAV Initialization Error: " << SDL_GetError();
		return false;
	}
	if (Mix_Init(MIX_INIT_OGG | MIX_INIT_MOD | MIX_INIT_MP3) == 0)
	{
		std::cout << "Mixer OTHER Initialization Error: " << SDL_GetError();
		return false;
	}
	Mix_AllocateChannels(16);
	return true;
}

void Game::CloseSDL()
{
	SDL_DestroyWindow(window);
	window = nullptr;

	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void Game::LoadLevels()
{
	levels.push_back(new Level(Levels::ONEONE,"1-1", "resources/files/level1-1.txt", renderer, { TILESIZE * 8, TILESIZE * 12 }));
	levels.push_back(new Level(Levels::ONETWO, "2-1", "resources/files/level2-1.txt", renderer, { TILESIZE * 8, TILESIZE * 12 }));
	levels.push_back(new Level(Levels::ONETHREE, "3-1", "resources/files/level3-1.txt", renderer, { TILESIZE * 8, TILESIZE * 12 }));
	//levels.push_back(new Level(Levels::ONETHREE, "TUTORIAL", "resources/files/tutorial.txt", renderer, { TILESIZE * 0, TILESIZE * 11 },false));
}


void Game::StartGame()
{
	music->Stop(true);
	currentScene = MAINGAME;
	currentLevel = 0;
	LoadLevel(currentLevel);
	player->lives = 3;
	player->score = 0;
}

void Game::LoadBoxes()
{
	scenes[Scenes::MAINMENU].boxes.push_back(new Box({ WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4 }, renderer, WINDOW_WIDTH/2, WINDOW_HEIGHT / 2, true, true, { 16,8 }));
	scenes[Scenes::MAINMENU].boxes[0]->AddText("SUPER-MARIO BROS.", fonts[Fonts::EMULOGIC], 50, Colors::WHITE, NULL, TextRenderModes::SOLID, false, 448, 0, 0, 0);
	scenes[Scenes::MAINMENU].boxes[0]->tabs[0].pages[0].rows.push_back({});
	scenes[Scenes::MAINMENU].boxes[0]->AddButton(Buttons::BUTTONS_PLAYGAME, 64, 64, 4, fonts[Fonts::EMULOGIC], 40, Colors::WHITE, NULL, Directions::RIGHT, 0, 0, 1, [&, this]() {StartGame(); });
	scenes[Scenes::MAINMENU].boxes[0]->tabs[0].pages[0].rows.push_back({});
	scenes[Scenes::MAINMENU].boxes[0]->AddButton(Buttons::BUTTONS_QUIT, 64, 64, 4, fonts[Fonts::EMULOGIC], 40, Colors::WHITE, NULL, Directions::RIGHT, 0, 0, 2, [&, this]() { CloseSDL();  exit(0); });
	scenes[Scenes::MAINMENU].boxes[0]->UpdateContainers();
	scenes[Scenes::MAINMENU].boxes[0]->UpdateSpacing();
	scenes[Scenes::MAINMENU].boxes[0]->Update();

	scenes[Scenes::MAINGAME].boxes.push_back(new Box({ 0,0 }, renderer, 160, 64, true, false, { 16,8 }));
	scenes[Scenes::MAINGAME].boxes[0]->AddText("N/A", fonts[Fonts::EMULOGIC], 30, Colors::WHITE, NULL, TextRenderModes::SOLID, false, 0, 0, 0, 0);
	scenes[Scenes::MAINGAME].boxes[0]->UpdateContainers();
	scenes[Scenes::MAINGAME].boxes[0]->UpdateSpacing();
	scenes[Scenes::MAINGAME].boxes[0]->Update();

	scenes[Scenes::MAINGAME].boxes.push_back(new Box({ WINDOW_WIDTH-192,0 }, renderer, 192, 64, true, false, { 16,8 }));
	scenes[Scenes::MAINGAME].boxes[1]->AddText("N/A", fonts[Fonts::EMULOGIC], 30, Colors::WHITE, NULL, TextRenderModes::SOLID, false, 0, 0, 0, 0);

	scenes[Scenes::MAINGAME].boxes.push_back(new Box({ WINDOW_WIDTH-288,0 }, renderer, 96, 64, true, false, { 16,8 }));
	scenes[Scenes::MAINGAME].boxes[2]->AddText("N/A", fonts[Fonts::EMULOGIC], 30, Colors::WHITE, NULL, TextRenderModes::SOLID, false, 0, 0, 0, 0);

	scenes[Scenes::LEVELEDITOR].boxes.push_back(new Box({ 0,0 }, renderer, 256, 945, true, true, { 12,16 }));

	scenes[Scenes::LEVELEDITOR].boxes[0]->AddText("Tile List", fonts[Fonts::EMULOGIC], 15, Colors::YELLOW, NULL, TextRenderModes::SOLID, false, 0, 0, 0, 0);
	scenes[Scenes::LEVELEDITOR].boxes[0]->tabs[0].pages[0].rows.push_back({});
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_BRICK, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_FLOOR, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_BLOCK, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_SPECIAL, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE1, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE2, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE3, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE4, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE5, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE6, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE7, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE8, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE9, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE10, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE11, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE12, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE13, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE14, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE15, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE16, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE17, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE18, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE19, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_PIPE20, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_BUSHL, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_BUSHM, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_BUSHR, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CLOUDTL, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CLOUDTM, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CLOUDTR, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CLOUDBL, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CLOUDBM, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CLOUDBR, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CASTLEARCH1, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CASTLEARCH2, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CASTLECREN1, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CASTLECREN2, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CASTLEWINLEFT, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_CASTLEWINRIGHT, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_FLAGPOLE, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_FLAGTOP, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_HILLLEFT, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_HILLBERRY, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_HILLRIGHT, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_HILLFILL, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->AddButton(Buttons::BUTTONS_HILLTOP, 64, 64, 4, fonts[Fonts::EMULOGIC], 10, Colors::WHITE, NULL, Directions::DOWN, 0, 0, 1, nullptr);
	scenes[Scenes::LEVELEDITOR].boxes[0]->UpdateContainers();
	scenes[Scenes::LEVELEDITOR].boxes[0]->UpdateSpacing();
	scenes[Scenes::LEVELEDITOR].boxes[0]->Update();
}

bool Game::LoadContent()
{
	LoadSounds();

	LoadLevels();
	player = new Player(levels[currentLevel]->spawnLocation, renderer);
	player->UpdateState(0, &sounds);

	camera = new Camera();

	fonts.push_back("resources/fonts/emulogic.ttf");
	
	LoadScenes();
	LoadBoxes();
	
	mainMenu->LoadRenderer(renderer);
	mainMenu->Load("resources/textures/MainMenu.png");
	mainMenu->UpdatePosition({ 0,0,WINDOW_WIDTH,WINDOW_HEIGHT });

	return true;
}

void Game::LoadSounds()
{
	music = new SoundObject("resources/audio/OverworldTheme.mp3", MUSIC, NULL, "THEME",true);
	jump  = new SoundObject("resources/audio/smb_jump-small.wav",EFFECT,0, "JUMP");
	marioDeath = new SoundObject("resources/audio/smb_mariodie.wav",MUSIC,NULL, "MARIO DEATH");
	enemyDeath = new SoundObject("resources/audio/smb_stomp.wav",EFFECT,1, "ENEMY DEATH");
	coin = new SoundObject("resources/audio/smb_coin.wav",EFFECT,1, "COIN");
	powerup = new SoundObject("resources/audio/smb_powerup.wav",EFFECT,3, "POWERUP");
	oneUp = new SoundObject("resources/audio/smb_1-up.wav",EFFECT,4, "1-UP");
	powerupSpawn = new SoundObject("resources/audio/smb_powerup_appears.wav",EFFECT,5, "POWERUP SPAWN");
	brick = new SoundObject("resources/audio/smb_breakblock.wav",EFFECT,6, "BRICK");
	music->Play();

	Mix_VolumeMusic(48);
	
	sounds.push_back(&music);
	sounds.push_back(&jump);
	sounds.push_back(&enemyDeath);
	sounds.push_back(&marioDeath);
	sounds.push_back(&coin);
	sounds.push_back(&brick);
	sounds.push_back(&powerup);
	sounds.push_back(&powerupSpawn);
	sounds.push_back(&oneUp);

}
void Game::LoadScenes()
{
	scenes.push_back(Scene(Scenes::MAINGAME));
	scenes.push_back(Scene(Scenes::MAINMENU));
	scenes.push_back(Scene(Scenes::LEVELEDITOR));
}

void Game::LoadLevel(int levelID)
{
	enemies = {};
	effects = {};
	items = {};

	currentLevel = levelID;
	levels[currentLevel]->Initialize();

	if (currentScene != MAINMENU)
		effects.push_back(new Effect({ 0,0 }, Effects::EFFECT_TRANSITION, renderer, {}, 0, 0, ("Level " + levels[currentLevel]->levelName)));

	for (size_t i = 0; i < levels[currentLevel]->tiles.size(); i++)
	{
		if (levels[currentLevel]->tiles[i] != nullptr)
		{
			if (levels[currentLevel]->tiles[i]->ResolveCollision(5) == COLLISION_SPAWN)
			{
				enemies.push_back(new Enemy(levels[currentLevel]->tiles[i]->GetWorldPosition(), renderer, levels[currentLevel]->tiles[i]->enemyType));
			}
		}
	}

	music->Stop();

	player->worldPosition = player->spawnPosition;
	player->leftWall = 0;
	player->UpdateState(0, &sounds);
	player->levelOverride = false;
}

bool Game::Update()
{
	elapsedTime = SDL_GetTicks() - oldTime;
	if (elapsedTime < static_cast<Uint32>(elapsedTarget))
	{
		SDL_Delay(elapsedTarget - elapsedTime);
		elapsedTime = elapsedTarget - elapsedTime;
	}
	oldTime = SDL_GetTicks();

	//if (Mix_PlayingMusic() == 0)
	//	music->Play();

	if (static_cast<int>(elapsedTime) < 1000 / preferredFPS)
		SDL_Delay(1000 / preferredFPS - elapsedTime);

	bool const running = Input();

	viewWidth = static_cast<int>(WINDOW_WIDTH / camera->zoom);
	viewHeight = static_cast<int>(WINDOW_HEIGHT / camera->zoom);

	if (levels[currentLevel]->nextLevel)
	{
		currentLevel++;
		currentLevel %= levels.size();

		LoadLevel(currentLevel);
	}

	transition = false;
	for (size_t i = 0; i < effects.size(); i++) if (effects[i]->GetEffectType() == EFFECT_TRANSITION) { transition = true; break; }


	for (size_t i = 0; i < effects.size(); i++)
	{
		effects[i]->Update(elapsedTime, levels[currentLevel],&sounds);
	}

	for (size_t i = 0; i < effects.size(); i++)
	{
		if (effects[i]->complete)
		{
			if (effects[i]->GetEffectType() == EFFECT_TRANSITION)
			{
				(*sounds[SOUND_THEME])->LoadFile("resources/audio/OverworldTheme.mp3");
				music->Play();
				if (player->gameOver)
				{
					currentScene = MAINMENU;
					player->gameOver = false;
				}
			}
			if (effects[i]->GetEffectType() == EFFECT_MARIO_DEATH)
			{
				(*sounds[SOUND_THEME])->LoadFile("resources/audio/OverworldTheme.mp3");
				player->dying = false;
				marioDead = true;
				if (player->lives == 0)
				{
					effects.push_back(new Effect({ 0,0 }, Effects::EFFECT_TRANSITION, renderer, {}, 0, 0, ("GAME OVER"),50));
				}
			}
			effects.erase(effects.begin() + i);
		}
	}

	switch (currentScene)
	{
	case Scenes::MAINGAME:

		if (!transition && !player->dying)
		{
			counter++;
			counter %= 10;
			if (counter == 0)
			{
				scenes[Scenes::MAINGAME].boxes[0]->UpdateText(0, 0, 0, 0, std::to_string(1000 / elapsedTime));
			}

			scenes[Scenes::MAINGAME].boxes[1]->UpdateText(0, 0, 0, 0, std::to_string(player->score));

			scenes[Scenes::MAINGAME].boxes[2]->UpdateText(0, 0, 0, 0, std::to_string(player->lives));


			for (size_t i = 0; i < enemies.size(); i++)
			{
				if (enemies[i]->worldPosition.x <= camera->worldPosition.x + WINDOW_WIDTH * 0.5f - enemies[i]->width * 0.5f && enemies[i]->worldPosition.x >= camera->worldPosition.x - WINDOW_WIDTH * 0.5f + enemies[i]->width * 0.5f)
				{
					enemies[i]->Update(elapsedTime, levels[currentLevel], enemies);
				}
			}

			for (size_t i = 0; i < items.size(); i++)
			{
				if (items[i]->worldPosition.x <= camera->worldPosition.x + WINDOW_WIDTH * 0.5f - items[i]->width * 0.5f && items[i]->worldPosition.x >= camera->worldPosition.x - WINDOW_WIDTH * 0.5f + items[i]->width * 0.5f)
				{
					items[i]->Update(elapsedTime, levels[currentLevel]);
				}
			}

			if ((!player->Update(elapsedTime, levels[currentLevel], &enemies, &effects, &items,&sounds) && effects.size() == 0) || marioDead)
			{
				if (player->lives > 0)
				{
					LoadLevel(currentLevel);
				}
				marioDead = false;
			}

			levels[currentLevel]->Update(elapsedTime);

			if (player->worldPosition.x >= viewWidth * 0.5f + player->leftWall && player->worldPosition.x <= levels[currentLevel]->levelSize.x * TILESIZE - viewWidth * 0.5f)
			{
				camera->worldPosition.x = player->worldPosition.x;
			}
			if (!(player->worldPosition.x >= viewWidth * 0.5f) && player->worldPosition.x <= levels[currentLevel]->levelSize.x * TILESIZE - viewWidth * 0.5f) // WHITESPACE WOULD BE ON SCREEN (LEFT PRIORITY)
			{
				camera->worldPosition.x = viewWidth * 0.5f + player->leftWall;
			}
			if ((player->worldPosition.x >= viewWidth * 0.5f) && !(player->worldPosition.x <= levels[currentLevel]->levelSize.x * TILESIZE - viewWidth * 0.5f)) // WHITESPACE WOULD BE ON SCREEN (LEFT PRIORITY)
			{
				camera->worldPosition.x = levels[currentLevel]->levelSize.x * TILESIZE - viewWidth * 0.5f;
			}

			if (player->worldPosition.y >= viewHeight * 0.5f && player->worldPosition.y <= levels[currentLevel]->levelSize.y * TILESIZE - viewHeight * 0.5f)
			{
				camera->worldPosition.y = player->worldPosition.y;
			}
			else if (player->worldPosition.y > levels[currentLevel]->levelSize.y * TILESIZE - viewHeight * 0.5f) // WHITESPACE WOULD BE ON SCREEN (BOTTOM PRIORITY)
			{
				camera->worldPosition.y = levels[currentLevel]->levelSize.y * TILESIZE - viewHeight * 0.5f;
			}
			else if (player->worldPosition.y < viewHeight * 0.5f) // WHITESPACE WOULD BE ON SCREEN (BOTTOM PRIORITY)
			{
				if (viewHeight < levels[currentLevel]->levelSize.y * TILESIZE)
				{
					camera->worldPosition.y = viewHeight * 0.5f;
				}
				else
				{
					camera->worldPosition.y = levels[currentLevel]->levelSize.y * TILESIZE - viewHeight * 0.5f;
				}
			}

			if (camera->worldPosition.x - viewWidth * 0.5f > player->leftWall)  player->leftWall = camera->worldPosition.x - viewWidth * 0.5f;
		}
	}
	
	Draw();
	return running;
}

void Game::Draw()
{

	switch (currentScene)
	{
	case Scenes::MAINGAME:
		if (!debug)
			SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255);
		else
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if (debug)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			if (showCenterLines)
			{
				SDL_RenderDrawLine(renderer, static_cast<int>(WINDOW_WIDTH * 0.5f), 0, static_cast<int>(WINDOW_WIDTH * 0.5f), WINDOW_HEIGHT);
				SDL_RenderDrawLine(renderer, 0, static_cast<int>(WINDOW_HEIGHT * 0.5f), WINDOW_WIDTH, static_cast<int>(WINDOW_HEIGHT * 0.5f));
			}
			levels[currentLevel]->DrawDebug(camera, showTiles, showTilePlayerRelationship, player->worldPosition);
			player->DrawDebug(camera, showPlayer, showPlayerSides, showPlayerGridPos);

			if (showEnemies)
			{
				for (size_t i = 0; i < enemies.size(); i++)
				{
					if (enemies[i]->worldPosition.x <= camera->worldPosition.x + WINDOW_WIDTH * 0.5f - enemies[i]->width * 0.5f && enemies[i]->worldPosition.x >= camera->worldPosition.x - WINDOW_WIDTH * 0.5f + enemies[i]->width * 0.5f)
					{
						enemies[i]->DrawDebug(camera);
					}
				}
			}


			if (showItems)
			{
				for (size_t i = 0; i < items.size(); i++)
				{
					if (items[i]->worldPosition.x <= camera->worldPosition.x + WINDOW_WIDTH * 0.5f - items[i]->width * 0.5f && items[i]->worldPosition.x >= camera->worldPosition.x - WINDOW_WIDTH * 0.5f + items[i]->width * 0.5f)
					{
						items[i]->DrawDebug(camera);
					}
				}
			}


			if (showMenus)
			{
				for (size_t i = 0; i < scenes[currentScene].boxes.size(); i++)
				{
					scenes[currentScene].boxes[i]->DrawDebug();
				}
			}
		}
		else
		{

			if (!transition)
			{
				levels[currentLevel]->Draw(camera);

				for (size_t i = 0; i < scenes[currentScene].boxes.size(); i++)
				{
					scenes[currentScene].boxes[i]->Draw();
				}

				for (size_t i = 0; i < enemies.size(); i++)
				{
					if (enemies[i]->worldPosition.x <= camera->worldPosition.x + WINDOW_WIDTH * 0.5f - enemies[i]->width * 0.5f && enemies[i]->worldPosition.x >= camera->worldPosition.x - WINDOW_WIDTH * 0.5f + enemies[i]->width * 0.5f)
					{
						if (enemies[i]->GetDirection() == Directions::LEFT)
							enemies[i]->Draw(camera);
						else if (enemies[i]->GetDirection() == Directions::RIGHT)
							enemies[i]->Draw(camera, 0.0, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
					}
				}

				for (size_t i = 0; i < items.size(); i++)
				{
					if (items[i]->worldPosition.x <= camera->worldPosition.x + WINDOW_WIDTH * 0.5f - items[i]->width * 0.5f && items[i]->worldPosition.x >= camera->worldPosition.x - WINDOW_WIDTH * 0.5f + items[i]->width * 0.5f)
					{
						items[i]->Draw(camera);
					}
				}

			}
			for (size_t i = 0; i < effects.size(); i++)
			{
				if ((effects[i]->GetWorldPosition().x <= camera->worldPosition.x + WINDOW_WIDTH * 0.5f - effects[i]->GetWidth() * 0.5f && effects[i]->GetWorldPosition().x >= camera->worldPosition.x - WINDOW_WIDTH * 0.5f + effects[i]->GetWidth() * 0.5f) || effects[i]->GetEffectType() == EFFECT_TRANSITION)
				{
					effects[i]->Draw(camera,&sounds);
				}
			}
			if (!transition)
			{
				if (player->GetDirection() == Directions::RIGHT)
					player->Draw(camera);
				else if (player->GetDirection() == Directions::LEFT)
					player->Draw(camera, 0.0, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
			}
			break;
		}
		break;
	case Scenes::LEVELEDITOR:
	case Scenes::MAINMENU:
		if (!debug)
			SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255);
		else
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		if (debug)
		{
			if (showMenus)
			{
				for (size_t i = 0; i < scenes[currentScene].boxes.size(); i++)
				{
					scenes[currentScene].boxes[i]->DrawDebug();
				}
			}
		}
		else
		{
			mainMenu->Draw();
			for (size_t i = 0; i < scenes[currentScene].boxes.size(); i++)
			{
				scenes[currentScene].boxes[i]->Draw();
			}
		}
		break;
	}
	SDL_RenderPresent(renderer);
}

bool Game::Input()
{

	SDL_GetMouseState(&mousePos.x, &mousePos.y);

	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		return false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_a:
		case SDLK_LEFT:
			switch (currentScene)
			{
			case Scenes::MAINGAME:
				player->movingLeft = true;
			}
			break;
		case SDLK_d:
		case SDLK_RIGHT:
			switch (currentScene)
			{
			case Scenes::MAINGAME:
				player->movingRight = true;
			}
			break;
		case SDLK_w:
		case SDLK_UP:
			switch (currentScene)
			{
			case Scenes::MAINGAME:
				if (player->Jump() && levels[currentLevel]->complete == false && !player->dying)
				{
					jump->Stop();
					jump->Play();
				}
			}
			break;
		case SDLK_z:
			switch (currentScene)
			{
			case Scenes::LEVELEDITOR:
			case Scenes::MAINMENU:
			case Scenes::MAINGAME:
				if (!debugPressed)
				{
					debug = !debug;
					debugPressed = true;
				}
			}
			break;
		case SDLK_e:
			if (!ePressed)
			{
				SDL_GetMouseState(&mousePos.x, &mousePos.y);
				if (SDL_PointInRect(&mousePos, &scenes[currentScene].boxes[0]->hitbox))
				{
					if (scenes[currentScene].boxes[0]->currentPage < static_cast<int>(scenes[currentScene].boxes[0]->tabs[scenes[currentScene].boxes[0]->currentTab].pages.size() - 1))
					{
						scenes[currentScene].boxes[0]->currentPage++;
						scenes[currentScene].boxes[0]->Update();
					}
				}
				ePressed = true;
			}
			break;
		case SDLK_q:
			if (!qPressed)
			{
				SDL_GetMouseState(&mousePos.x, &mousePos.y);
				if (SDL_PointInRect(&mousePos, &scenes[currentScene].boxes[0]->hitbox))
				{
					if (scenes[currentScene].boxes[0]->currentPage >= 1)
					{
						scenes[currentScene].boxes[0]->currentPage--;
						scenes[currentScene].boxes[0]->Update();
					}
				}

				qPressed = true;
			}
			break;
		case SDLK_c:
			if (!cPressed)
			{
				currentLevel++;
				currentLevel %= levels.size();

				LoadLevel(currentLevel);
				cPressed = true;
			}
			break;
		case SDLK_x:
			if (!xPressed)
			{
				currentScene++;
				currentScene %= scenes.size();
				LoadLevel(currentLevel);
				xPressed = true;
			}
			break;
		case SDLK_p:
			switch (currentScene)
			{
			case Scenes::MAINGAME:
				if (player->state == States::REGULAR)
					player->UpdateState(States::SMALL,&sounds);
				else
					player->UpdateState(States::REGULAR, &sounds);
			}
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			if (currentScene != MAINMENU)
			{
				currentScene = Scenes::MAINMENU;
				music->Stop();
				music->LoadFile("resources/audio/OverworldTheme.mp3");
				music->Play();
			}
			else if (currentScene == MAINMENU)
				return false;
			break;
		case SDLK_a:
		case SDLK_LEFT:
			switch (currentScene)
			{
			case Scenes::MAINGAME:
				player->movingLeft = false;
			}
			break;
		case SDLK_RIGHT:
		case SDLK_d:
			switch (currentScene)
			{
			case Scenes::MAINGAME:
				player->movingRight = false;
			}
			break;
		case SDLK_z:
			switch (currentScene)
			{
			case Scenes::LEVELEDITOR:
			case Scenes::MAINMENU:
			case Scenes::MAINGAME:
				debugPressed = false;
			}
			break;
		case SDLK_q:
			qPressed = false;
			break;
		case SDLK_e:
			ePressed = false;
			break;
		case SDLK_x:
			xPressed = false;
			break;
		case SDLK_c:
			cPressed = false;
			break;
		}
		break;
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			SDL_GetMouseState(&mousePos.x, &mousePos.y);
			for (size_t i = 0; i < scenes[currentScene].boxes[0]->tabs[scenes[currentScene].boxes[0]->currentTab].pages[scenes[currentScene].boxes[0]->currentPage].rows.size(); i++)
			{
				for (size_t j = 0; j < scenes[currentScene].boxes[0]->tabs[scenes[currentScene].boxes[0]->currentTab].pages[scenes[currentScene].boxes[0]->currentPage].rows[i].widgets.size(); j++)
				{
					SDL_Rect hitbox = scenes[currentScene].boxes[0]->tabs[scenes[currentScene].boxes[0]->currentTab].pages[scenes[currentScene].boxes[0]->currentPage].rows[i].widgets[j]->hitbox;
					if (SDL_PointInRect(&mousePos, &hitbox))
					{
						std::cout << "Page = " << scenes[currentScene].boxes[0]->currentPage << ", Row = " << i << ", Widget = " << j << std::endl;
						if (scenes[currentScene].boxes[0]->tabs[scenes[currentScene].boxes[0]->currentTab].pages[scenes[currentScene].boxes[0]->currentPage].rows[i].widgets[j]->GetType() == "Button")
						{
							std::invoke(scenes[currentScene].boxes[0]->tabs[scenes[currentScene].boxes[0]->currentTab].pages[scenes[currentScene].boxes[0]->currentPage].rows[i].widgets[j]->GetExecutable());
							break;
						}
					}
				}
			}
		}
		break;
	case SDL_MOUSEWHEEL:
		if (event.wheel.y > 0)
		{
			//camera->zoom *= 1.001f;
			camera->zoom++;

		}
		if (event.wheel.y < 0)
		{
			/*if (camera->zoom > 1)
				camera->zoom *= 0.999f;
			else
			{
				camera->zoom = 1;
			}*/
			if (camera->zoom > 1)
				camera->zoom--;
		}
		break;
	}

	if (scenes[currentScene].boxes.size() > 0)
	{
		for (size_t i = 0; i < scenes[currentScene].boxes.size(); i++)
		{
			for (size_t j = 0; j < scenes[currentScene].boxes[i]->tabs.size(); j++)
			{
				for (size_t k = 0; k < scenes[currentScene].boxes[i]->tabs[j].pages.size(); k++)
				{
					for (size_t l = 0; l < scenes[currentScene].boxes[i]->tabs[j].pages[k].rows.size(); l++)
					{
						if (scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets.size() > 0)
						{
							for (size_t m = 0; m < scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets.size(); m++)
							{
								if (scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets[m]->GetType() == "Button")
								{
									SDL_Rect hitbox = { static_cast<int>(scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets[m]->GetPosition().x + scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets[m]->GetOrigin().x),static_cast<int>(scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets[m]->GetPosition().y + scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets[m]->GetOrigin().y),scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets[m]->GetWidth(),scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets[m]->GetHeight() };
									SDL_GetMouseState(&mousePos.x, &mousePos.y);
									if (SDL_PointInRect(&mousePos, &hitbox))
									{
										scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets[m]->UpdateColor(Colors::YELLOW, 1);
									}
									else
									{
										scenes[currentScene].boxes[i]->tabs[j].pages[k].rows[l].widgets[m]->UpdateColor(Colors::WHITE, 1);
									}
								}
							}
						}
					}
				}
			}

		}
	}

	return true;
}