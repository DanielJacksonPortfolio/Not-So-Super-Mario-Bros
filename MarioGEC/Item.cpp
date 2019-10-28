#include "Item.h"
#include "Constants.h"
#include <math.h>
#include "Tile.h"

Item::Item(Coordf position, SDL_Renderer * renderer, int direction, int type)
{
	this->type = type;
	this->worldPosition = position;
	this->direction = direction;

	switch (this->type)
	{
	case Items::ITEMS_POWERUP:
		image = new Image({ 0, 0, 16, 16 });
		break;
	case Items::ITEMS_STAR:
		image = new Image({ 0, 32, 16, 16 });
		break;
	case Items::ITEMS_1UP:
		image = new Image({16, 0, 16, 16 });
		break;
	case Items::ITEMS_FIRE:
		image = new Image({ 0, 16, 16, 16 });
		break;
	}
	
	this->renderer = renderer;
	image->LoadRenderer(this->renderer);
	this->LoadImage("resources/textures/items.png");

	if (direction == Directions::LEFT)
		movingLeft = true;
	else
		movingLeft = false;
	movingRight = !movingLeft;
}

Item::~Item()
{
	image->destroy();
	SDL_DestroyRenderer(renderer);
}

void Item::LoadImage(std::string path)
{
	image->Load(path);
}

void Item::UpdateImage(Camera * camera)
{
	image->UpdatePosition(screenQuadCalculation);
}

void Item::Draw(Camera * camera, double angle, SDL_RendererFlip flip)
{
	if (image != nullptr)
	{
		UpdateImage(camera);
		image->Draw(angle, flip);
	}
}

void Item::Draw(Camera * camera)
{
	if (image != nullptr)
	{
		UpdateImage(camera);
		image->Draw();
	}
}

void Item::Animate(int elapsedTime)
{
	if (type == Items::ITEMS_STAR)
	{
		animationTimer += elapsedTime;
		if (animationTimer >= 50)
		{
			currentFrameX++;
			currentFrameX %= 4;
			image->textureRect = { currentFrameX*width,32,width,height };
			animationTimer = 0;
		}
	}
}

void Item::CollisionChecking(Level * level)
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
					if (level->tiles[index]->GetCollisionType() == IMPASSABLE)
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
							stopLeft = true;
							velocity.x = 0.0f;
							movingLeft = false;
							movingRight = true;
						}
						if (collider->BoundingBox(rightRect, tileHitbox))
						{
							stopRight = true;
							velocity.x = 0.0f;
							movingLeft = true;
							movingRight = false;
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
		worldPosition.x = width * 0.5f;
		movingLeft = false;
		movingRight = true;
	}
	else if (worldPosition.x > (level->levelSize.x*TILESIZE) - width * 0.5f)
	{
		worldPosition.x = (level->levelSize.x*TILESIZE) - width * 0.5f;
		movingLeft = true;
		movingRight = false;
	}

	if (worldPosition.y > (level->levelSize.y*TILESIZE) - height * 0.5f)
		worldPosition.y = height * 0.5f;

	//Velocity Resolution

	if (YCollisions.size() > 0)
		velocity.y = 0.0f;
}

void Item::Update(int elapsedTime, Level * level)
{
	Animate(elapsedTime);

	if (type != Items::ITEMS_FIRE)
	{
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
	}
	gridPosition.x = static_cast<int>(floor(worldPosition.x / TILESIZE));
	gridPosition.y = static_cast<int>(floor(worldPosition.y / TILESIZE));

	stopLeft = false;
	stopRight = false;
	isGrounded = false;

	CollisionChecking(level);

	if (type != Items::ITEMS_FIRE)
	{
		velocity.x *= 1 - 0.005f * elapsedTime;

		velocity.y *= 1 - 0.001f * elapsedTime;
	}
}


void Item::DrawDebug(Camera* camera)
{
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(renderer, static_cast<int>((this->worldPosition.x - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>((this->worldPosition.x - camera->worldPosition.x + this->velocity.x * 100) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y + this->velocity.y * 100) * camera->zoom + WINDOW_HEIGHT * 0.5f));

	SDL_Rect playerRect = { static_cast<int>((this->worldPosition.x - camera->worldPosition.x - this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y - this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(this->width * camera->zoom), static_cast<int>(this->height * camera->zoom) };
	SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
	SDL_RenderDrawRect(renderer, &playerRect);
	SDL_RenderDrawLine(renderer, static_cast<int>((this->worldPosition.x - camera->worldPosition.x - this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y - this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>((this->worldPosition.x - camera->worldPosition.x + this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y + this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f));
}