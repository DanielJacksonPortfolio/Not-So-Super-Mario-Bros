#pragma once
#include "Image.h"
#include "Constants.h"
#include "Level.h"
#include "Collider.h"

class Enemy
{
private:
	Image * image = nullptr;
	float moveSpeed = 0.0002f;
	int animationTimer = 0;
	int currentFrameX = 0;
	int currentFrameY = 0;
	float maxVelocity = 0.5f;
	int direction = Directions::LEFT;
	SDL_Renderer * renderer = nullptr;
	Collider* collider = new Collider();

public:
	Enemy(Coordf position, SDL_Renderer * renderer, int enemyType);
	~Enemy();
	void LoadImage(std::string path);
	void Draw(Camera * camera, double angle, SDL_RendererFlip flip);
	void Draw(Camera * camera);
	void DrawDebug(Camera* camera);
	void UpdateImage(Camera * camera);
	void Update(int elapsedTime, Level * level, std::vector<Enemy*> enemies);
	void Animate(int elapsedTime);
	int const virtual GetDirection() { return direction; }
	void CollisionChecking(Level * level);
	void EnemyCollisionChecking(std::vector<Enemy*> * enemies);

	Coordi gridPosition = Coordi(0, 0);
	Coordf worldPosition = Coordf(0.0f, 0.0f);
	Vectorf velocity = Vectorf(0.0f, 0.0f);
	bool movingLeft = false, movingRight = false, movingUp = false, movingDown = false, isGrounded = false, stopLeft = false, stopRight = false;
	SDL_Rect hitbox = { 0,0,0,0 };
	int width = 16, height = 16;
	int score = 0;
	int type = 0;
	int collisionType = 0;
};

