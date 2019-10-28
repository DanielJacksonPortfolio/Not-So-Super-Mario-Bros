#pragma once

#define WINDOW_WIDTH 1728
#define WINDOW_HEIGHT 972
#define TILESIZE 16

#define screenQuadCalculation { static_cast<int>((worldPosition.x - camera->worldPosition.x - width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((worldPosition.y - camera->worldPosition.y - height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(width * camera->zoom), static_cast<int>(height * camera->zoom) }
#define defaultTileConstructer \
 	this->gridPosition = gridPosition;\
	this->worldPosition = Coordf(gridPosition.x * width + width * 0.5f, gridPosition.y * height + height * 0.5f);\
	this->image = *image;\
	this->hitbox = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height };

#include <vector>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_ttf.h>

struct Coordi;
struct Coordf;
struct Vectorf;
struct intPair;


std::vector<intPair> LoadLevelSizes();

struct Coordi
{
	int x = 0;
	int y = 0;
	Coordi(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Coordf
{
	float x = 0;
	float y = 0;
	Coordf(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

struct Vectorf
{
	float x = 0;
	float y = 0;
	Vectorf(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

struct intPair
{
	int a = 0, b = 0;
	intPair(int x, int y)
	{
		this->a = x;
		this->b = y;
	}
};

enum GameStates {MENU, ENDGAME, LEVELSELECT, RUNNING};
enum Directions {LEFT, RIGHT, UP, DOWN, NONE};
enum CollisionTypes {PASSABLE, IMPASSABLE, ITEM, FLAG};
enum States {SMALL, REGULAR};
enum Collisions {COLLISION_NONE, COLLISION_BRICK, COLLISION_BRICK_COIN, COLLISION_BRICK_STAR, COLLISION_SPECIAL_FIRE, COLLISION_SPECIAL_COIN, COLLISION_SPECIAL_POWERUP, COLLISION_SHELL, COLLISION_SPECIAL_1UP, COLLISION_FLOOR, COLLISION_BLOCK, COLLISION_PIPE, COLLISION_DECOR, COLLISION_FLAG, COLLISION_COIN, COLLISION_FIRE, COLLISION_1UP, COLLISION_SPAWN, COLLISION_GOOMBA, COLLISION_KOOPA, COLLISION_POWERUP};
enum Levels {ONEONE, ONETWO, ONETHREE, ONEFOUR};
enum Fonts {EMULOGIC};
enum Enemies { ENEMY_GOOMBA, ENEMY_KOOPA,ENEMY_SHELL};
enum Effects {EFFECT_GOOMBA, EFFECT_BRICK, EFFECT_COIN,EFFECT_TEXT,EFFECT_FLAG,EFFECT_PLAYER_REGULAR,EFFECT_PLAYER_SMALL,EFFECT_TRANSITION, EFFECT_MARIO_DEATH};
enum Colors {BLACK,WHITE,RED,GREEN,BLUE,YELLOW};
enum TextRenderModes {SOLID,SHADED,BLENDED, WRAPPED};
enum Buttons {BUTTONS_PLAYGAME,BUTTONS_QUIT,BUTTONS_BRICK,BUTTONS_BLOCK,BUTTONS_SPECIAL,BUTTONS_FLOOR,BUTTONS_FLOORBG,BUTTONS_FLAGPOLE,BUTTONS_FLAGTOP,BUTTONS_CASTLEARCH1,BUTTONS_CASTLEARCH2,BUTTONS_CASTLECREN1,BUTTONS_CASTLECREN2,BUTTONS_CASTLEWINRIGHT,BUTTONS_CASTLEWINLEFT,BUTTONS_HILLTOP,BUTTONS_HILLBERRY,BUTTONS_HILLLEFT,BUTTONS_HILLRIGHT,BUTTONS_HILLFILL,BUTTONS_CLOUDTL,BUTTONS_CLOUDTM,BUTTONS_CLOUDTR,BUTTONS_CLOUDBL,BUTTONS_CLOUDBM,BUTTONS_CLOUDBR,BUTTONS_BUSHL,BUTTONS_BUSHM,BUTTONS_BUSHR,BUTTONS_PIPE1,BUTTONS_PIPE2,BUTTONS_PIPE3,BUTTONS_PIPE4,BUTTONS_PIPE5,BUTTONS_PIPE6,BUTTONS_PIPE7,BUTTONS_PIPE8,BUTTONS_PIPE9,BUTTONS_PIPE10,BUTTONS_PIPE11,BUTTONS_PIPE12,BUTTONS_PIPE13,BUTTONS_PIPE14,BUTTONS_PIPE15,BUTTONS_PIPE16,BUTTONS_PIPE17,BUTTONS_PIPE18,BUTTONS_PIPE19,BUTTONS_PIPE20};
enum Scenes {MAINGAME,MAINMENU,LEVELEDITOR};
enum Items {ITEMS_POWERUP, ITEMS_STAR, ITEMS_1UP,ITEMS_FIRE};
enum SpecialTypes {SPECIAL_COIN, SPECIAL_POWERUP, SPECIAL_1UP};
enum BrickTypes {BRICK_NORMAL, BRICK_COIN, BRICK_STAR};
enum SoundObjects {SOUND_THEME,SOUND_JUMP,SOUND_ENEMY_DEATH, SOUND_MARIO_DEATH,SOUND_COIN, SOUND_BRICK,SOUND_POWERUP,SOUND_POWERUP_SPAWN,SOUND_1UP};
enum Sounds{MUSIC,EFFECT};
static std::vector<intPair> levelSizes = LoadLevelSizes();
static std::vector<SDL_Color> colors = { {0,0,0},{255,255,255},{255,0,0},{0,255,0},{0,0,255},{255,255,0} };

static std::vector<intPair> LoadLevelSizes(){

	std::ifstream sizeFile("resources/files/levelSizes.txt");
	std::vector<intPair> levelSizes;
	if (sizeFile.is_open())
	{
		while (!sizeFile.eof()) {
			int x, y;
			sizeFile >> x;
			sizeFile >> y;
			levelSizes.push_back(intPair( x, y ));
		}
	}
	else
	{
		std::cerr << "Unable to open file";
	}
	sizeFile.close();

	return levelSizes;	
}

static const int GetLevelSize(SDL_Point size)
{
	return size.x * size.y;
}

static int roundUp(int numToRound, int multiple)
{
	if (multiple == 0)
	{
		return numToRound;
	}

	int const remainder = numToRound % multiple;
	if (remainder == 0)
	{
		return numToRound;
	}

	return numToRound + multiple - remainder;
}

static std::vector<std::string> SplitString(std::string stringToSplit, char delimeter)
{
	std::stringstream stringStream(stringToSplit);
	std::string splitString;
	std::vector<std::string> splitStrings;
	while (std::getline(stringStream, splitString, delimeter)) 
		splitStrings.push_back(splitString);
	return splitStrings;
}

static bool ValidPosition(Coordi gridPosition, SDL_Point levelSize)
{
	if (gridPosition.x < 0 || gridPosition.x >= levelSize.x || gridPosition.y < 0 || gridPosition.y >= levelSize.y)
	{
		return false;
	}
	return true;
}