#include "Enemy.h"
#include "Constants.h"
#include <math.h>
#include "Tile.h"

Enemy::Enemy(Coordf position, SDL_Renderer* renderer, int enemyType)
{
	this->worldPosition = position;
	this->renderer = renderer;
	movingLeft = rand() % 2;
	movingRight = !movingLeft;
	switch (enemyType)
	{
	case Enemies::ENEMY_GOOMBA:
		image = new Image({ 0, 0, 16, 16 });
		image->LoadRenderer(this->renderer);
		this->LoadImage("resources/textures/goomba.png");
		break;
	case Enemies::ENEMY_KOOPA:
		height = 24;
		this->worldPosition.y -= 4.0f;
		image = new Image({ 0, 0, 16, 24 });
		image->LoadRenderer(this->renderer);
		this->LoadImage("resources/textures/koopa.png");
		break;
	case Enemies::ENEMY_SHELL:
		height = 16;
		maxVelocity = 0.3f;
		this->worldPosition.y += 4.0f;
		image = new Image({ 32, 8, 16, 16 });
		image->LoadRenderer(this->renderer);
		this->LoadImage("resources/textures/koopa.png");
		movingLeft = false;
		movingRight = false;
		velocity.x = 0.3f;
		break;
	}
	if (movingLeft)
		direction = Directions::LEFT;
	else
		direction = Directions::RIGHT;
	type = enemyType;
}

Enemy::~Enemy()
{
	image->destroy();
	SDL_DestroyRenderer(renderer);
}

void Enemy::LoadImage(std::string path)
{
	image->Load(path);
}

void Enemy::UpdateImage(Camera* camera)
{
	image->UpdatePosition(screenQuadCalculation);
}

void Enemy::Draw(Camera* camera, double angle, SDL_RendererFlip flip)
{
	if (image != nullptr)
	{
		UpdateImage(camera);
		image->Draw(angle, flip);
	}
}

void Enemy::Draw(Camera* camera)
{
	if (image != nullptr)
	{
		UpdateImage(camera);
		image->Draw();
	}
}

void Enemy::Animate(int elapsedTime)
{
	animationTimer += elapsedTime;
	if (animationTimer >= 200)
	{
		currentFrameX++;
		currentFrameX %= 2;
		image->textureRect = { currentFrameX * width,0,width,height };
		animationTimer = 0;
	}
}

void Enemy::CollisionChecking(Level* level)
{
	std::vector<std::array<int, 2>> XCollisions;
	std::vector<std::array<int, 2>> YCollisions;

	hitbox = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height };

	int index = 0;
	Coordi tileCoord = Coordi(0, 0);

	for (int i = -1; i < width / TILESIZE + 1; i++)
	{
		for (int j = -1; j < height / TILESIZE + 1; j++)
		{
			tileCoord.x = gridPosition.x + i;
			tileCoord.y = gridPosition.y + j;
			if (ValidPosition(tileCoord, level->levelSize))
			{
				index = tileCoord.y * level->levelSize.x + tileCoord.x;
				if (level->tiles[index] != nullptr)
				{
					if (level->tiles[index]->GetCollisionType() == IMPASSABLE || level->tiles[index]->GetCollisionType() == ITEM)
					{
						collider->LevelCollision(level->tiles[index]->GetHitbox(), hitbox, level->tiles[index]->GetWorldPosition(), worldPosition, XCollisions, YCollisions, index);
					}
				}
			}
		}
	}

	// X STUFF


	for (size_t i = 0; i < XCollisions.size(); i++)
	{
		int const index = XCollisions[i][0];
		if (level->tiles[index]->GetCollisionType() == IMPASSABLE)
		{
			if (XCollisions[i][1] > 0)
				worldPosition.x = level->tiles[index]->GetWorldPosition().x + level->tiles[index]->GetWidth() * 0.5f + width * 0.5f;
			else
				worldPosition.x = level->tiles[index]->GetWorldPosition().x - level->tiles[index]->GetWidth() * 0.5f - width * 0.5f;
		}

	}

	//Y STUFF

	for (size_t i = 0; i < YCollisions.size(); i++)
	{
		int const index = YCollisions[i][0];
		if (level->tiles[index]->GetCollisionType() == IMPASSABLE)
		{
			if (YCollisions[i][1] > 0)
				worldPosition.y = level->tiles[index]->GetWorldPosition().y + level->tiles[index]->GetHeight() * 0.5f + height * 0.5f;
			else
				worldPosition.y = level->tiles[index]->GetWorldPosition().y - level->tiles[index]->GetHeight() * 0.5f - height * 0.5f;
		}
	}

	SDL_Rect leftRect = { static_cast<int>(worldPosition.x - width * 0.5f - 1), static_cast<int>(worldPosition.y - height * 0.5f), 1, height };
	SDL_Rect rightRect = { static_cast<int>(worldPosition.x + width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f) , 1, height };
	SDL_Rect upRect = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f - 1), width, 1 };
	SDL_Rect downRect = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y + height * 0.5f), width, 1 };


	for (int i = -1; i < width / TILESIZE + 1; i++)
	{
		for (int j = -1; j < height / TILESIZE + 1; j++)
		{
			tileCoord.x = gridPosition.x + i;
			tileCoord.y = gridPosition.y + j;
			if (ValidPosition(tileCoord, level->levelSize))
			{
				index = tileCoord.y * level->levelSize.x + tileCoord.x;
				if (level->tiles[index] != nullptr)
				{
					if (level->tiles[index]->GetCollisionType() == IMPASSABLE)
					{
						SDL_Rect  const tileHitbox = level->tiles[index]->GetHitbox();

						if (collider->BoundingBox(leftRect, tileHitbox))
						{
							if (type != Enemies::ENEMY_SHELL)
							{
								stopLeft = true;
								velocity.x = 0.0f;
								movingLeft = false;
								movingRight = true;
							}
							else
							{
								velocity.x = maxVelocity;
							}
						}
						if (collider->BoundingBox(rightRect, tileHitbox))
						{
							if (type != Enemies::ENEMY_SHELL)
							{
								stopRight = true;
								velocity.x = 0.0f;
								movingLeft = true;
								movingRight = false;
							}
							else
							{
								velocity.x = -maxVelocity;
							}
						}
						if (collider->BoundingBox(downRect, tileHitbox))
						{
							isGrounded = true;
							velocity.y = 0.0f;
						}
					}
				}
			}
		}
	}

	// World Edge Collisions

	if (worldPosition.x < width * 0.5f)
	{
		//worldPosition.x = width * 0.5f;
		stopLeft = true;
		velocity.x = 0.0f;
		movingLeft = false;
		movingRight = true;
	}
	else if (worldPosition.x > (level->levelSize.x * TILESIZE) - width * 0.5f)
	{
		//worldPosition.x = (level->levelSize.x * TILESIZE) - width * 0.5f;
		stopRight = true;
		velocity.x = 0.0f;
		movingLeft = true;
		movingRight = false;
	}

	if (worldPosition.y > (level->levelSize.y * TILESIZE) - height * 0.5f)
		worldPosition.y = height * 0.5f;

	//Velocity Resolution

	if (YCollisions.size() > 0)
		velocity.y = 0.0f;
}

void Enemy::EnemyCollisionChecking(std::vector<Enemy*> * enemies)
{
	hitbox = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height };

	SDL_Rect leftRect = { static_cast<int>(worldPosition.x - width * 0.5f - 1), static_cast<int>(worldPosition.y - height * 0.5f), 1, height };
	SDL_Rect rightRect = { static_cast<int>(worldPosition.x + width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f) , 1, height };
	SDL_Rect upRect = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f - 1), width, 1 };
	SDL_Rect downRect = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y + height * 0.5f), width, 1 };

	for (size_t i = 0; i < (*enemies).size(); i++)
	{
		if (collider->BoundingBox(leftRect, (*enemies)[i]->hitbox))
		{
			if (type != Enemies::ENEMY_SHELL && (*enemies)[i]->type != Enemies::ENEMY_SHELL)
			{
				stopLeft = true;
				velocity.x = 0.0f;
				movingLeft = false;
				movingRight = true;

				(*enemies)[i]->stopRight = true;
				(*enemies)[i]->velocity.x = 0.0f;
				(*enemies)[i]->movingLeft = true;
				(*enemies)[i]->movingRight = false;
			}
			else
			{
				(*enemies).erase((*enemies).begin() + i);
			}
		}
		else if (collider->BoundingBox(rightRect, (*enemies)[i]->hitbox))
		{
			if (type != Enemies::ENEMY_SHELL && (*enemies)[i]->type != Enemies::ENEMY_SHELL)
			{
				stopRight = true;
				velocity.x = 0.0f;
				movingLeft = true;
				movingRight = false;

				(*enemies)[i]->stopLeft = true;
				(*enemies)[i]->velocity.x = 0.0f;
				(*enemies)[i]->movingLeft = false;
				(*enemies)[i]->movingRight = true;
			}
			else
			{
				(*enemies).erase((*enemies).begin() + i);
			}
		}
	}
}


void Enemy::Update(int elapsedTime, Level * level, std::vector<Enemy*> enemies)
{
	if (type != Enemies::ENEMY_SHELL)
	{
		Animate(elapsedTime);
	}
	if (movingLeft && !stopLeft)
		velocity.x -= moveSpeed * elapsedTime;
	else if (movingRight && !stopRight)
		velocity.x += moveSpeed * elapsedTime;


	if (!isGrounded)
	{
		velocity.y += 0.001f * elapsedTime;
	}

	if (velocity.x >= maxVelocity)
		worldPosition.x += maxVelocity * elapsedTime;
	else if (velocity.x <= -maxVelocity)
		worldPosition.x -= maxVelocity * elapsedTime;
	else
		worldPosition.x += velocity.x * elapsedTime;

	if (velocity.y >= maxVelocity)
		worldPosition.y += maxVelocity * elapsedTime;
	else if (velocity.y <= -maxVelocity)
		worldPosition.y -= maxVelocity * elapsedTime;
	else
		worldPosition.y += velocity.y * elapsedTime;

	if (velocity.x > 0)
	{
		direction = Directions::RIGHT;
	}
	else if (velocity.x < 0)
	{
		direction = Directions::LEFT;
	}
	if (velocity.y < 0)
	{
		movingUp = true;
		movingDown = false;
	}
	else if (velocity.y > 0)
	{
		movingDown = true;
		movingUp = false;
	}
	else if (velocity.y == 0)
	{
		movingUp = false;
		movingDown = false;
	}

	gridPosition.x = static_cast<int>(floor(worldPosition.x / TILESIZE));
	gridPosition.y = static_cast<int>(floor(worldPosition.y / TILESIZE));

	stopLeft = false;
	stopRight = false;
	isGrounded = false;

	CollisionChecking(level);
	EnemyCollisionChecking(&enemies);

	if (type != Enemies::ENEMY_SHELL)
	{
		velocity.x *= 1 - 0.005f * elapsedTime;
	}

	velocity.y *= 1 - 0.001f * elapsedTime;
}


void Enemy::DrawDebug(Camera * camera)
{
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(renderer, static_cast<int>((this->worldPosition.x - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>((this->worldPosition.x - camera->worldPosition.x + this->velocity.x * 100) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y + this->velocity.y * 100) * camera->zoom + WINDOW_HEIGHT * 0.5f));

	SDL_Rect playerRect = { static_cast<int>((this->worldPosition.x - camera->worldPosition.x - this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y - this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(this->width * camera->zoom), static_cast<int>(this->height * camera->zoom) };
	SDL_SetRenderDrawColor(renderer, 255, 127, 0, 255);
	SDL_RenderDrawRect(renderer, &playerRect);
	SDL_RenderDrawLine(renderer, static_cast<int>((this->worldPosition.x - camera->worldPosition.x - this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y - this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>((this->worldPosition.x - camera->worldPosition.x + this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y + this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f));
}