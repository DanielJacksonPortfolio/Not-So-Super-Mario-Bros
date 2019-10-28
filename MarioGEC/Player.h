#pragma once
#include "Image.h"
#include "Constants.h"
#include "Level.h"
#include "Enemy.h"
#include "Effect.h"
#include "Item.h"
#include "SoundObject.h"
#include "Collider.h"

class Player
{
private:
	Image * image;
	float moveSpeed = 0.001f;
	int animationTimer = 0;
	int currentFrameX = 0;
	int currentFrameY = 0;
	int starFrame = 0;
	float maxVelocity = 0.5f;
	int direction = Directions::LEFT;
	SDL_Rect hitbox = { 0,0,0,0 };
	std::array<int,2> leftAction = {};
	std::array<int,2> rightAction = {};
	std::array<int,2> upAction = {};
	std::array<int,2> downAction = {};
	SDL_Renderer * renderer = nullptr;
	bool showPlayer = true;
	Collider* collider = new Collider();

public:
	Player(Coordf position, SDL_Renderer * renderer);
	~Player();
	void LoadImage(std::string path);
	void Draw(Camera * camera, double angle, SDL_RendererFlip flip);
	void Draw(Camera * camera);
	void DrawDebug(Camera* camera, bool showPlayer, bool showPlayerSides, bool showPlayerGridPos);
	void UpdateImage(Camera * camera);
	bool Update(int elapsedTime, Level * level, std::vector<Enemy*> * enemies, std::vector<Effect*> * effects, std::vector<Item*> * items, std::vector<SoundObject**> * sounds);
	bool Jump();
	void Animate(int elapsedTime);
	int const virtual GetDirection() { return direction; }
	std::array<int, 2> ResolveLevelCollisions(std::vector<int> collisionIndexes, int direction, Level * level);
	void LevelCollisionChecking(Level * level, std::vector<Effect*> * effects, std::vector<Item*> * items, std::vector<SoundObject**>* sounds);
	bool EnemyCollisionChecking(std::vector<Enemy*> * enemies, Level * level, std::vector<Effect*> * effects, std::vector<SoundObject**>* sounds);
	void ItemCollisionChecking(std::vector<Item*> * items, Level * level, std::vector<Effect*> * effects, std::vector<SoundObject**> * sounds);
	void UpdateState(int istate, std::vector<SoundObject**>* sounds);
	void AddScore(int scoreToAdd, Coordf position, std::vector<Effect*> * effects, Level * level);

	Coordi gridPosition = Coordi(0, 0);
	Coordf worldPosition = Coordf(0.0f, 0.0f);
	Coordf spawnPosition = Coordf(0.0f, 0.0f);
	Vectorf velocity = Vectorf(0.0f, 0.0f);
	bool movingLeft = false, movingRight = false, movingUp = false, movingDown = false, isGrounded = false, stopLeft = false, stopRight = false;
	int state = States::SMALL;
	int width = 16, height;
	int score = 0;
	int lives =1;
	bool invunerable = false;
	int invunerableCounter = 0;
	bool starPower = false;
	int starCounter = 0;
	int flashCounter = 0;
	bool levelOverride = false;
	int leftWall = 0;
	bool dying = false;
	bool gameOver = false;
};

