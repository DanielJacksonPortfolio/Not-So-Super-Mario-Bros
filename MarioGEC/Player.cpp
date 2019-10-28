#include "Player.h"
#include "Constants.h"
#include <math.h>
#include "Tile.h"

Player::Player(Coordf position, SDL_Renderer * renderer)
{
	this->worldPosition = position;
	this->spawnPosition = position;
	image = new Image({ 0, 0, 16, 32 });
	this->renderer = renderer;
	image->LoadRenderer(this->renderer);
	this->LoadImage("resources/textures/mario.png");
}

Player::~Player()
{
	image->destroy();
	SDL_DestroyRenderer(renderer);
}

void Player::LoadImage(std::string path)
{
	image->Load(path);
}

void Player::UpdateImage(Camera * camera)
{
	image->UpdatePosition(screenQuadCalculation);
}

void Player::Draw(Camera * camera, double angle, SDL_RendererFlip flip)
{
	if (image != nullptr && showPlayer)
	{
		UpdateImage(camera);
		image->Draw(angle, flip);
	}
}

void Player::Draw(Camera * camera)
{
	if (image != nullptr && showPlayer)
	{
		UpdateImage(camera);
		image->Draw();
	}
}

void Player::Animate(int elapsedTime)
{
	animationTimer += elapsedTime;
	if (animationTimer >= 100)
	{	
		if (!starPower)
		{
			starFrame = 0;
		}
		else
		{
			starFrame++;
			starFrame %= 4;
			if (starFrame == 0)
			{
				starFrame++;
			}
		}
		if (!isGrounded)
		{
			currentFrameX = 5;
		}
		if (!movingLeft && !movingRight && isGrounded)
		{
			currentFrameX = 0;
		}
		if ((movingRight || movingLeft) && isGrounded)
		{
			if (++currentFrameX >= 4)
				currentFrameX = 1;
		}
		if ((movingLeft && velocity.x > 0) || (movingRight && velocity.x < 0))
		{
			currentFrameX = 4;
		}
		if (invunerable)
		{
			flashCounter += elapsedTime;
			if (flashCounter >= 20)
			{
				showPlayer = false;
				flashCounter = 0;
			}
			else
			{
				showPlayer = true;
			}
		}
		else if (!levelOverride)
		{
			showPlayer = true;
		}
		image->textureRect = { currentFrameX*width,currentFrameY*32 + 48 * starFrame,width,height};
		animationTimer = 0;
	}
}

std::array<int,2> Player::ResolveLevelCollisions(std::vector<int> collisionIndexes, int direction, Level * level)
{
	if (collisionIndexes.size() > 0)
	{
		int closestTile = 0;
		float closestDistance = static_cast<float>(INTMAX_MAX);
		for (size_t i = 0; i < collisionIndexes.size(); i++)
		{
			float const distance = sqrt(pow(worldPosition.x - level->tiles[collisionIndexes[i]]->GetWorldPosition().x, 2) + pow(worldPosition.y - level->tiles[collisionIndexes[i]]->GetWorldPosition().y, 2));
			if (distance < closestDistance)
			{
				closestDistance = distance;
				closestTile = collisionIndexes[i];
			}
		}
		return { closestTile, level->tiles[closestTile]->ResolveCollision(direction) };
	}
	return {};
}

void Player::LevelCollisionChecking(Level * level, std::vector<Effect*> * effects, std::vector<Item*> * items, std::vector<SoundObject**>* sounds)
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
					if (level->tiles[index]->GetCollisionType() == IMPASSABLE || level->tiles[index]->GetCollisionType() == ITEM || level->tiles[index]->GetCollisionType() == FLAG)
					{
						collider->LevelCollision(level->tiles[index]->GetHitbox(), hitbox, level->tiles[index]->GetWorldPosition(), worldPosition, XCollisions, YCollisions, index);
					}
				}
			}
		}
	}

	std::vector<int> leftCollisions = {};
	std::vector<int> rightCollisions = {};
	std::vector<int> upCollisions = {};
	std::vector<int> downCollisions = {};

	// X STUFF

	for (size_t i = 0; i < XCollisions.size(); i++)
	{
		int const index = XCollisions[i][0];
		SDL_Rect  const tileHitbox = level->tiles[index]->GetHitbox();

		if (XCollisions[i][1] > 0)
		{
			if (collider->BoundingBox({ static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f + 1), 1, height - 2 }, tileHitbox))
				leftCollisions.push_back(index);
		}
		else
		{
			if (collider->BoundingBox({ static_cast<int>(worldPosition.x + width * 0.5f - 1), static_cast<int>(worldPosition.y - height * 0.5f + 1), 1, height - 2 }, tileHitbox))
				rightCollisions.push_back(index);
		}
	}

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
		SDL_Rect  const tileHitbox = level->tiles[index]->GetHitbox();

		if (YCollisions[i][1] > 0)
		{
			if (collider->BoundingBox({ static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, 1 }, tileHitbox))
				upCollisions.push_back(index);
		}
		else
		{
			if (collider->BoundingBox({ static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y + height * 0.5f - 1), width, 1 }, tileHitbox))
				downCollisions.push_back(index);
		}
	}

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
						SDL_Rect const  tileHitbox = level->tiles[index]->GetHitbox();

						if (collider->BoundingBox(leftRect, tileHitbox))
						{
							stopLeft = true;
							velocity.x = 0.0f;
						}
						if (collider->BoundingBox(rightRect, tileHitbox))
						{
							stopRight = true;
							velocity.x = 0.0f;
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

	if (worldPosition.x > level->flagPosition.x + width * 0.5f && level->hasEnd)
	{
		if (level->complete == false)
		{
			worldPosition.x = level->flagPosition.x + width * 0.5f;
			levelOverride = true;
			showPlayer = false;
			level->complete = true;
			(*effects).push_back(new Effect(level->flagPosition, Effects::EFFECT_FLAG, renderer, level->effectImages));
			if (state == States::REGULAR)
				(*effects).push_back(new Effect(worldPosition, Effects::EFFECT_PLAYER_REGULAR, renderer, level->effectImages,0, level->flagPosition.y));
			else
				(*effects).push_back(new Effect(worldPosition, Effects::EFFECT_PLAYER_SMALL, renderer, level->effectImages,0, level->flagPosition.y));
			(*(*sounds)[SOUND_THEME])->Stop();
		}
	}

	leftAction = ResolveLevelCollisions(leftCollisions, Directions::LEFT, level);
	rightAction = ResolveLevelCollisions(rightCollisions, Directions::RIGHT, level);
	upAction = ResolveLevelCollisions(upCollisions, Directions::UP, level);
	downAction = ResolveLevelCollisions(downCollisions, Directions::DOWN, level);

	//COLLISION RESOLUTION

	switch (leftAction[1])
	{
	case Collisions::COLLISION_COIN:
		AddScore(200, { level->tiles[leftAction[0]]->GetWorldPosition().x + level->tiles[leftAction[0]]->GetWidth() * 0.5f, level->tiles[leftAction[0]]->GetWorldPosition().y - level->tiles[leftAction[0]]->GetHeight() * 0.5f }, effects, level);
		level->tiles[leftAction[0]] = nullptr;
		(*(*sounds)[SOUND_COIN])->Stop();
		(*(*sounds)[SOUND_COIN])->Play();
		break;
	}

	switch (rightAction[1])
	{
	case Collisions::COLLISION_COIN:
		AddScore(200, { level->tiles[rightAction[0]]->GetWorldPosition().x + level->tiles[rightAction[0]]->GetWidth() * 0.5f, level->tiles[rightAction[0]]->GetWorldPosition().y - level->tiles[rightAction[0]]->GetHeight() * 0.5f }, effects, level);
		level->tiles[rightAction[0]] = nullptr;
		(*(*sounds)[SOUND_COIN])->Stop();
		(*(*sounds)[SOUND_COIN])->Play();
		break;
	}

	switch (downAction[1])
	{
	case Collisions::COLLISION_COIN:
		AddScore(200, { level->tiles[downAction[0]]->GetWorldPosition().x + level->tiles[downAction[0]]->GetWidth() * 0.5f, level->tiles[downAction[0]]->GetWorldPosition().y - level->tiles[downAction[0]]->GetHeight() * 0.5f }, effects, level);
		level->tiles[downAction[0]] = nullptr;
		(*(*sounds)[SOUND_COIN])->Stop();
		(*(*sounds)[SOUND_COIN])->Play();
		break;
	}

	switch (upAction[1])
	{
	case Collisions::COLLISION_BRICK:
		if (movingUp && state != SMALL)
		{
			(*effects).push_back(new Effect(level->tiles[upAction[0]]->GetWorldPosition(), Effects::EFFECT_BRICK, renderer, level->effectImages));
			level->tiles[upAction[0]] = nullptr;
			(*(*sounds)[SOUND_BRICK])->Stop();
			(*(*sounds)[SOUND_BRICK])->Play();
		}
		break;
	case Collisions::COLLISION_BRICK_COIN:
		if (movingUp)
		{
			if (level->tiles[upAction[0]]->active)
			{
				AddScore(200, Coordf(level->tiles[upAction[0]]->GetWorldPosition().x + level->tiles[upAction[0]]->GetWidth() * 0.5f, level->tiles[upAction[0]]->GetWorldPosition().y - level->tiles[upAction[0]]->GetHeight() * 0.5f), effects, level);
				(*effects).push_back(new Effect({ level->tiles[upAction[0]]->GetWorldPosition().x, level->tiles[upAction[0]]->GetWorldPosition().y - TILESIZE }, Effects::EFFECT_COIN, renderer, level->effectImages));
				(*(*sounds)[SOUND_COIN])->Stop();
				(*(*sounds)[SOUND_COIN])->Play();
			}
		}
		break;
	case Collisions::COLLISION_BRICK_STAR:
		if (movingUp)
		{
			if (level->tiles[upAction[0]]->active)
			{
				(*items).push_back(new Item({ level->tiles[upAction[0]]->GetWorldPosition().x, level->tiles[upAction[0]]->GetWorldPosition().y - TILESIZE }, renderer, direction, Items::ITEMS_STAR));
				level->tiles[upAction[0]]->active = false;
				(*(*sounds)[SOUND_POWERUP_SPAWN])->Stop();
				(*(*sounds)[SOUND_POWERUP_SPAWN])->Play();
			}
		}
		break;
	case Collisions::COLLISION_SPECIAL_COIN:
		if (movingUp)
		{
			if (level->tiles[upAction[0]]->active)
			{
				AddScore(200, Coordf(level->tiles[upAction[0]]->GetWorldPosition().x + level->tiles[upAction[0]]->GetWidth() * 0.5f, level->tiles[upAction[0]]->GetWorldPosition().y - level->tiles[upAction[0]]->GetHeight() * 0.5f), effects, level);
				(*effects).push_back(new Effect({ level->tiles[upAction[0]]->GetWorldPosition().x, level->tiles[upAction[0]]->GetWorldPosition().y - TILESIZE }, Effects::EFFECT_COIN, renderer, level->effectImages));
				level->tiles[upAction[0]]->SetUnactive();
				(*(*sounds)[SOUND_COIN])->Stop();
				(*(*sounds)[SOUND_COIN])->Play();
			}
		}
		break;
	case Collisions::COLLISION_SPECIAL_POWERUP:
		if (movingUp)
		{
			if (level->tiles[upAction[0]]->active)
			{
				if (state == States::SMALL)
					(*items).push_back(new Item({ level->tiles[upAction[0]]->GetWorldPosition().x, level->tiles[upAction[0]]->GetWorldPosition().y - TILESIZE }, renderer, direction,Items::ITEMS_POWERUP));
				else
					(*items).push_back(new Item({ level->tiles[upAction[0]]->GetWorldPosition().x, level->tiles[upAction[0]]->GetWorldPosition().y - TILESIZE }, renderer, direction, Items::ITEMS_FIRE));
				level->tiles[upAction[0]]->SetUnactive();
				(*(*sounds)[SOUND_POWERUP_SPAWN])->Stop();
				(*(*sounds)[SOUND_POWERUP_SPAWN])->Play();
			}
		}
		break;
	case Collisions::COLLISION_SPECIAL_1UP:
		if (movingUp)
		{
			if (level->tiles[upAction[0]]->active)
			{
				(*items).push_back(new Item({ level->tiles[upAction[0]]->GetWorldPosition().x, level->tiles[upAction[0]]->GetWorldPosition().y - TILESIZE }, renderer, direction, Items::ITEMS_1UP));
				level->tiles[upAction[0]]->SetUnactive();
				(*(*sounds)[SOUND_POWERUP_SPAWN])->Stop();
				(*(*sounds)[SOUND_POWERUP_SPAWN])->Play();
			}
		}
		break;
	case Collisions::COLLISION_COIN:
		AddScore(200, { level->tiles[upAction[0]]->GetWorldPosition().x + level->tiles[upAction[0]]->GetWidth() * 0.5f, level->tiles[upAction[0]]->GetWorldPosition().y - level->tiles[upAction[0]]->GetHeight() * 0.5f }, effects, level);
		level->tiles[upAction[0]] = nullptr;
		(*(*sounds)[SOUND_COIN])->Stop();
		(*(*sounds)[SOUND_COIN])->Play();
		break;
	}


	// World Edge Collisions

	if (worldPosition.x < width * 0.5f + leftWall)
		worldPosition.x = width * 0.5f + leftWall;
	else if (worldPosition.x > (level->levelSize.x*TILESIZE) - width * 0.5f)
		worldPosition.x = (level->levelSize.x*TILESIZE) - width * 0.5f;

	if (worldPosition.y > (level->levelSize.y*TILESIZE) - height * 0.5f)
		worldPosition.y = height * 0.5f;

	//Velocity Resolution

	if (upCollisions.size() > 0 && movingUp)
		velocity.y = 0.0f;
}

bool Player::EnemyCollisionChecking(std::vector<Enemy*> * enemies, Level * level, std::vector<Effect*> * effects, std::vector<SoundObject**>* sounds)
{
	hitbox = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height };

	SDL_Rect leftRect = { static_cast<int>(worldPosition.x - width * 0.5f - 1), static_cast<int>(worldPosition.y - height * 0.5f), 1, height };
	SDL_Rect rightRect = { static_cast<int>(worldPosition.x + width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f) , 1, height };
	SDL_Rect upRect = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f - 1), width, 1 };
	SDL_Rect downRect = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y + height * 0.5f), width, 1 };

	for (size_t i = 0; i < (*enemies).size(); i++)
	{

		if (collider->BoundingBox(downRect, (*enemies)[i]->hitbox))
		{
			if ((*enemies)[i]->type != Enemies::ENEMY_SHELL)
			{
				AddScore(100, (*enemies)[i]->worldPosition, effects, level);
				if ((*enemies)[i]->type == Enemies::ENEMY_GOOMBA)
				{
					(*effects).push_back(new Effect((*enemies)[i]->worldPosition, Effects::EFFECT_GOOMBA, renderer, level->effectImages));
					std::cout << "GOOMBA STOMP" << std::endl;
					if (!starPower)
						velocity.y = -0.1f;
				}
				else
				{
					std::cout << "KOOPA STOMP" << std::endl;
					(*enemies).push_back(new Enemy((*enemies)[i]->worldPosition, renderer, Enemies::ENEMY_SHELL));
					if (direction == Directions::RIGHT)
						(*enemies)[i]->velocity.x = 0.3f;
					else
						(*enemies)[i]->velocity.x = 0.3f;
					if (!starPower)
						velocity.y = -0.45f;
				}
				(*(*sounds)[SOUND_ENEMY_DEATH])->Stop();
				(*(*sounds)[SOUND_ENEMY_DEATH])->Play();
				(*enemies).erase((*enemies).begin() + i);
			}
			else
			{
				if (!(collider->BoundingBox(rightRect, (*enemies)[i]->hitbox)|| collider->BoundingBox(leftRect, (*enemies)[i]->hitbox)))
				{
					(*enemies)[i]->velocity.x = 0.0f;
					if (!starPower)
						velocity.y = -0.45f;
				}
			}


		}
		else if (collider->BoundingBox(leftRect, (*enemies)[i]->hitbox) || collider->BoundingBox(rightRect, (*enemies)[i]->hitbox) || collider->BoundingBox(upRect, (*enemies)[i]->hitbox))
		{
			if ((*enemies)[i]->type != Enemies::ENEMY_SHELL || ((*enemies)[i]->type == Enemies::ENEMY_SHELL && (*enemies)[i]->velocity.x != 0.0f))
			{
				if (!invunerable && !starPower)
				{
					if (state == States::REGULAR)
					{
						UpdateState(States::SMALL,sounds);
					}
					else if (!dying)
					{
						lives--;
						(*(*sounds)[SOUND_THEME])->Stop();
						(*(*sounds)[SOUND_THEME])->LoadFile("resources/audio/smb_mariodie.wav");
						(*(*sounds)[SOUND_THEME])->Play();
						levelOverride = true;
						showPlayer = false;
						dying = true;
						(*effects).push_back(new Effect(worldPosition, Effects::EFFECT_MARIO_DEATH	, renderer, level->effectImages));
					}
					invunerable = true;
					invunerableCounter = 2000;
				}
				else if (starPower)
				{
					AddScore(100, (*enemies)[i]->worldPosition, effects, level);
					if ((*enemies)[i]->type == Enemies::ENEMY_GOOMBA)
					{
						(*effects).push_back(new Effect((*enemies)[i]->worldPosition, Effects::EFFECT_GOOMBA, renderer, level->effectImages));
						std::cout << "GOOMBA STOMP" << std::endl;
					}
					else
					{
						std::cout << "KOOPA STOMP" << std::endl;
					}
					(*(*sounds)[SOUND_ENEMY_DEATH])->Stop();
					(*(*sounds)[SOUND_ENEMY_DEATH])->Play();
					(*enemies).erase((*enemies).begin() + i);
				}
			}
			else if ((*enemies)[i]->type == Enemies::ENEMY_SHELL && (*enemies)[i]->velocity.x == 0.0f)
			{
				if (direction == Directions::RIGHT)
					(*enemies)[i]->velocity.x = 0.3f;
				else
					(*enemies)[i]->velocity.x = 0.3f;
			}
		}
	}
	return false;
}

void Player::ItemCollisionChecking(std::vector<Item*> * items, Level * level, std::vector<Effect*> * effects, std::vector<SoundObject**>* sounds)
{
	hitbox = { static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height };

	for (size_t i = 0; i < (*items).size(); i++)
	{
		if (collider->BoundingBox((*items)[i]->hitbox, hitbox))
		{
			switch ((*items)[i]->type)
			{
			case Items::ITEMS_POWERUP:
				if (state == States::SMALL)
				{
					UpdateState(States::REGULAR,sounds);
				}
				AddScore(1000, (*items)[i]->worldPosition, effects, level);
				(*items).erase((*items).begin() + i);
				std::cout << "MUSHROOOOOOM" << std::endl;
				(*(*sounds)[SOUND_POWERUP])->Stop();
				(*(*sounds)[SOUND_POWERUP])->Play();
				break;
			case Items::ITEMS_STAR:
				(*items).erase((*items).begin() + i);
				(*(*sounds)[SOUND_THEME])->Stop();
				(*(*sounds)[SOUND_THEME])->LoadFile("resources/audio/StarTheme.mp3");
				(*(*sounds)[SOUND_THEME])->Play();
				starPower = true;
				starCounter = 7000;
				std::cout << "STAR POWER" << std::endl;
				break;
			case Items::ITEMS_1UP:
				(*items).erase((*items).begin() + i);
				lives++;
				std::cout << "EXTRA LIFE - 1UP" << std::endl;
				(*(*sounds)[SOUND_1UP])->Stop();
				(*(*sounds)[SOUND_1UP])->Play();
				break;
			case Items::ITEMS_FIRE:
				(*items).erase((*items).begin() + i);
				if (state == States::SMALL)
				{
					UpdateState(States::REGULAR,sounds);
				}
				AddScore(1500, (*items)[i]->worldPosition, effects, level);
				std::cout << "FIRE" << std::endl;
				(*(*sounds)[SOUND_1UP])->Stop();
				(*(*sounds)[SOUND_1UP])->Play();
				break;
			}
		}
	}
}


void Player::AddScore(int scoreToAdd, Coordf position, std::vector<Effect*> * effects, Level * level)
{
	score += scoreToAdd;
	(*effects).push_back(new Effect(position, Effects::EFFECT_TEXT, renderer, level->effectImages,0,0,std::to_string(scoreToAdd),8));
}

void Player::UpdateState(int istate, std::vector<SoundObject**>* sounds)
{
	this->state = istate;

	if (state == REGULAR)
	{
		currentFrameY = 0;
		height = 32;
		(*(*sounds)[SOUND_JUMP])->LoadFile("resources/audio/smb_jump-super.wav");
			
	}
	else
	{
		(*(*sounds)[SOUND_JUMP])->LoadFile("resources/audio/smb_jump-small.wav");
		currentFrameY = 1;
		height = 16;
		worldPosition.y += 16;
	}
}

bool Player::Jump()
{
	if (isGrounded)
	{
		isGrounded = false;
		velocity.y = -0.45f;
		return true;
	}
	return false;
	/*else if (wallSliding)
	{
		if (stopLeft && !movingLeft)
		{
			velocity.x = 3.0f;
			velocity.y = - 3.0f;
		}
		else if (stopRight && !movingRight)
		{
			velocity.x = - 3.0f;
			velocity.y = - 3.0f;
		}
	}*/
}

void Player::DrawDebug(Camera* camera, bool showPlayer, bool showPlayerSides, bool showPlayerGridPos)
{
	if (showPlayer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, 0, 0, static_cast<int>((this->worldPosition.x - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y) * camera->zoom + WINDOW_HEIGHT * 0.5f));
		SDL_RenderDrawLine(renderer, static_cast<int>((this->worldPosition.x - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>((this->worldPosition.x - camera->worldPosition.x + this->velocity.x * 100) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y + this->velocity.y * 100) * camera->zoom + WINDOW_HEIGHT * 0.5f));

		SDL_Rect playerRect = { static_cast<int>((this->worldPosition.x - camera->worldPosition.x - this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y - this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(this->width * camera->zoom), static_cast<int>(this->height * camera->zoom) };
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &playerRect);
		SDL_RenderDrawLine(renderer, static_cast<int>((this->worldPosition.x - camera->worldPosition.x - this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y - this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>((this->worldPosition.x - camera->worldPosition.x + this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y + this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f));
		if (showPlayerSides)
		{

			SDL_Rect leftRect = { static_cast<int>((this->worldPosition.x - camera->worldPosition.x - this->width * 0.5f - 1) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y - this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(camera->zoom), static_cast<int>(this->height * camera->zoom) };
			SDL_Rect rightRect = { static_cast<int>((this->worldPosition.x - camera->worldPosition.x + this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y - this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f) , static_cast<int>(camera->zoom), static_cast<int>(this->height * camera->zoom) };
			SDL_Rect upRect = { static_cast<int>((this->worldPosition.x - camera->worldPosition.x - this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y - this->height * 0.5f - 1) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(this->width * camera->zoom) , static_cast<int>(camera->zoom) };
			SDL_Rect downRect = { static_cast<int>((this->worldPosition.x - camera->worldPosition.x - this->width * 0.5f) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->worldPosition.y - camera->worldPosition.y + this->height * 0.5f) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(this->width * camera->zoom), static_cast<int>(camera->zoom) };

			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			SDL_RenderDrawRect(renderer, &leftRect);
			SDL_RenderDrawRect(renderer, &rightRect);
			SDL_RenderDrawRect(renderer, &upRect);
			SDL_RenderDrawRect(renderer, &downRect);
		}
		if (showPlayerGridPos)
		{
			SDL_Rect gridRect = { static_cast<int>((this->gridPosition.x * TILESIZE - camera->worldPosition.x) * camera->zoom + WINDOW_WIDTH * 0.5f), static_cast<int>((this->gridPosition.y * TILESIZE - camera->worldPosition.y) * camera->zoom + WINDOW_HEIGHT * 0.5f), static_cast<int>(TILESIZE * camera->zoom), static_cast<int>(TILESIZE * camera->zoom) };

			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			SDL_RenderDrawRect(renderer, &gridRect);
		}
	}

}

bool Player::Update(int elapsedTime, Level * level, std::vector<Enemy*> * enemies, std::vector<Effect*> * effects, std::vector<Item*> * items, std::vector<SoundObject**>* sounds)
{
	Animate(elapsedTime);

	if (level->complete == false)
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

	LevelCollisionChecking(level, effects, items,sounds);
	
	if (EnemyCollisionChecking(enemies, level, effects,sounds))
		return false;

	ItemCollisionChecking(items, level, effects,sounds);

	if (lives == 0)
	{
		gameOver = true;
	}

	velocity.x *= 1 - 0.005f * elapsedTime;	

	velocity.y *= 1 - 0.001f * elapsedTime;


	if (invunerable)
	{
		invunerableCounter -= elapsedTime;
		if (invunerableCounter <= 0)
		{
			invunerable = false;
		}
	}
	if (starPower)
	{
		starCounter -= elapsedTime;
		if (starCounter <= 0)
		{
			starPower = false;
			(*(*sounds)[SOUND_THEME])->Stop();
			(*(*sounds)[SOUND_THEME])->LoadFile("resources/audio/OverworldTheme.mp3");
			(*(*sounds)[SOUND_THEME])->Play();
		}
	}

	return true;
}
