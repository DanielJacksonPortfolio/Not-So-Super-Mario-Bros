#include "Effect.h"

Effect::Effect(Coordf position, int effectType, SDL_Renderer * renderer, std::vector<Image*>  effectImages, float startX, float finalX, std::string text, int fontSize)
{
	this->worldPosition = position;
	this->effectType = effectType;
	this->renderer = renderer;

	this->text = text;
	this->fontSize = fontSize;
	this->startX = startX;
	this->finalX = finalX;

	startPosition = position;

	switch (effectType)
	{
	case Effects::EFFECT_GOOMBA:
		initialLength = 500;
		break;
	case Effects::EFFECT_COIN:
		initialLength = 1000;
		this->startX = -5.657f;
		this->finalX = 5.657f;
		break;
	case Effects::EFFECT_TEXT:
		initialLength = 2000;
		this->startX = 0;
		this->finalX = 8;
		break;
	case Effects::EFFECT_TRANSITION:
		initialLength = 3000;
		this->startX = 0;
		this->finalX = 0;
		break;
	case Effects::EFFECT_BRICK:
		initialLength = 1500;
		this->startX = -60;
		this->finalX = 60;
		break;
	case Effects::EFFECT_FLAG:
		initialLength = 4000;
		this->startX = 0;
		this->finalX = 8*TILESIZE;
		break;
	case Effects::EFFECT_PLAYER_REGULAR:
		this->finalX = 8 * TILESIZE - position.y + finalX - 8;
		initialLength = 2000 * (this->finalX / (8 * TILESIZE));
		this->startX = 0;
		break;
	case Effects::EFFECT_PLAYER_SMALL:
		this->finalX = 8 * TILESIZE - position.y + finalX;
		initialLength = 2000 * (this->finalX / (8 * TILESIZE));
		this->startX = 0;
		break;
	case Effects::EFFECT_MARIO_DEATH:
		initialLength = 2750;
		this->startX = -4;
		this->finalX = 16.0f;
		break;
	}

	length = initialLength;

	InitializeImages(effectImages);
	if (images.size() > 0)
		currentImages.push_back(images[0]);
}

Effect::~Effect()
{
	delete textObj;
	for (size_t i = 0; i < images.size(); i++)
	{
		images[i]->destroy();
	}
	for (size_t i = 0; i < currentImages.size(); i++)
	{
		currentImages[i]->destroy();
	}
	SDL_DestroyRenderer(renderer);
}

void Effect::InitializeImages(std::vector<Image*>  effectImages)
{
	switch (effectType)
	{
	case Effects::EFFECT_GOOMBA:
		images.push_back(effectImages[0]);
		break;
	case Effects::EFFECT_COIN:
		images.push_back(effectImages[1]);
		images.push_back(effectImages[2]);
		images.push_back(effectImages[3]);
		images.push_back(effectImages[4]);
		break;
	case Effects::EFFECT_PLAYER_REGULAR:
		images.push_back(effectImages[10]);
		images.push_back(effectImages[11]);
		images.push_back(effectImages[12]);
		images.push_back(effectImages[13]);
		images.push_back(effectImages[14]);


		height = 32;
		break;
		case Effects::EFFECT_PLAYER_SMALL:
		images.push_back(effectImages[15]);
		images.push_back(effectImages[16]);
		images.push_back(effectImages[17]);
		images.push_back(effectImages[18]);
		images.push_back(effectImages[19]);
		break;
	case Effects::EFFECT_BRICK:
		images.push_back(effectImages[5]);
		images.push_back(effectImages[6]);
		images.push_back(effectImages[7]);
		images.push_back(effectImages[8]);
		width = 8;
		height = 8;
		currentImages = images;
		currentImages[0]->centre = { 4,4 };
		currentImages[1]->centre = { 4,4 };
		currentImages[2]->centre = { 4,4 };
		currentImages[3]->centre = { 4,4 };
		break;
	case Effects::EFFECT_MARIO_DEATH:
		images.push_back(effectImages[20]);
		break;
	case Effects::EFFECT_FLAG:
		images.push_back(effectImages[9]);
		break;
	case Effects::EFFECT_TRANSITION:
		textObj = new Text(startPosition, { 0,0 }, text, "resources/fonts/emulogic.ttf", fontSize, Colors::WHITE, NULL, TextRenderModes::SOLID, renderer, 0);
		currentImages.push_back(textObj->image);
		width = textObj->GetWidth();
		height = textObj->GetHeight();
		break;
	case Effects::EFFECT_TEXT:
		textObj = new Text(startPosition, { 0,0 }, text, "resources/fonts/emulogic.ttf", fontSize, Colors::WHITE, NULL, TextRenderModes::SOLID, renderer, 0);
		currentImages.push_back(textObj->image);
		width = textObj->GetWidth();
		height = textObj->GetHeight();
		break;
	}
}

void Effect::Update(int elapsedTime, Level* level, std::vector<SoundObject**>* sounds)
{
	this->elapsedTime = elapsedTime;
	length -= elapsedTime;
	if (length <= 0)
	{
		if (effectType != Effects::EFFECT_FLAG && effectType !=  Effects::EFFECT_PLAYER_SMALL && effectType != Effects::EFFECT_PLAYER_REGULAR)
		{
			complete = true;
		}
		else if (effectType == Effects::EFFECT_PLAYER_SMALL || effectType == Effects::EFFECT_PLAYER_REGULAR)
		{
			stage++;
			if (stage == 1)
			{
				startX = 0;
				finalX = 32;
				initialLength = 1000;
				length = initialLength;
				worldPosition.y += 1; // Slight offset
				startPosition = worldPosition;
				currentImages[0] = images[4];
			}
			else if (stage == 2)
			{
				startX = 0;
				finalX = 80;
				initialLength = 5500;
				length = initialLength;
				startPosition = worldPosition;
				(*(*sounds)[SOUND_THEME])->LoadFile("resources/audio/LevelEnd.mp3");
				(*(*sounds)[SOUND_THEME])->Play();
			}
			else if (stage == 3)
			{
				complete = true;
				level->nextLevel = true;
			}

		}
	}
}

void Effect::Animate(int elapsedTime, Camera * camera, std::vector<SoundObject**>* sounds)
{
	switch (effectType)
	{
	case Effects::EFFECT_GOOMBA:
		currentImages[0] = images[0];
		break;
	case Effects::EFFECT_COIN:
		animationTimer += elapsedTime;
		if (animationTimer >= 200)
		{
			currentFrame++;
			currentFrame %= 7;
			int imageFrame = 0;
			if (currentFrame <= 3)
				imageFrame = currentFrame;
			else
				imageFrame = 7 - currentFrame;
			currentImages[0] = images[imageFrame];
			animationTimer = 0;
		}
		x = startX + (finalX - startX)*((initialLength - length) / initialLength);
		worldPosition.y = startPosition.y - (-(x*x) + 2 * TILESIZE);
		break;
	case Effects::EFFECT_MARIO_DEATH:
		currentImages[0] = images[0];
		x = startX + (finalX - startX) * ((initialLength - length) / initialLength);
		worldPosition.y = startPosition.y - (-(x * x) + 2 * TILESIZE);
		break;
	case Effects::EFFECT_BRICK:

		x = -startX * ((initialLength - length) / initialLength);
		worldPosition.x = startPosition.x + x;
		worldPosition.y = startPosition.y - (-0.05f*x*x + TILESIZE);
		UpdateImage(currentImages[1], camera);

		x = startX * ((initialLength - length) / initialLength);
		worldPosition.x = startPosition.x + x;
		worldPosition.y = startPosition.y - (-0.05f*x*x + TILESIZE);
		UpdateImage(currentImages[3], camera);

		x = -startX * ((initialLength - length) / initialLength);
		worldPosition.x = startPosition.x + x;
		worldPosition.y = startPosition.y - (-0.05f*x*x + TILESIZE) + 16;
		UpdateImage(currentImages[0], camera);

		x = startX * ((initialLength - length) / initialLength);
		worldPosition.x = startPosition.x + x;
		worldPosition.y = startPosition.y - (-0.05f*x*x + TILESIZE) + 16;
		UpdateImage(currentImages[2], camera);

		this->angleOfRotation += elapsedTime;

		break;

	case Effects::EFFECT_TEXT:
		x = startX + (finalX - startX)*((initialLength - length) / initialLength);
		worldPosition.x = startPosition.x + x;
		worldPosition.y = startPosition.y - x;
		break;

	case Effects::EFFECT_TRANSITION:
		worldPosition.x = (WINDOW_WIDTH)* 0.5f;
		worldPosition.y = (WINDOW_HEIGHT) * 0.5f;
		break;

	case Effects::EFFECT_FLAG:
		if (length >= 0)
		{
			x = finalX * ((initialLength - length) / initialLength);
			worldPosition.y = startPosition.y + x;
		}
		break;
	case Effects::EFFECT_PLAYER_REGULAR:
	case Effects::EFFECT_PLAYER_SMALL:
		if (stage == 0)
		{
			x = finalX * ((initialLength - length) / initialLength);
			worldPosition.y = startPosition.y + x;
		}
		if (stage == 1)
		{
			x = finalX * ((initialLength - length) / initialLength);
			worldPosition.x = startPosition.x + x;
			worldPosition.y = startPosition.y -( -0.091066f *x*x + 2.41415f * x);
		}
		if (stage == 2)
		{
			animationTimer += elapsedTime;
			if (animationTimer >= 100)
			{
				currentFrame++;
				currentFrame %= 3;
				currentImages[0] = images[1+currentFrame];
				animationTimer = 0;
			}
			x = finalX*((initialLength - length) / initialLength);
			worldPosition.x = startPosition.x + x;
		}
		break;
	}
}

void Effect::UpdateImage(Image* image,Camera * camera)
{
	if (effectType != Effects::EFFECT_TRANSITION)
		image->UpdatePosition(screenQuadCalculation);
	else
		image->UpdatePosition({ static_cast<int>(worldPosition.x - width * 0.5f), static_cast<int>(worldPosition.y - height * 0.5f), width, height });
}

void Effect::Draw(Camera * camera,std::vector<SoundObject**>* sounds)
{
	Animate(elapsedTime, camera,sounds);

	switch (effectType)
	{
	case Effects::EFFECT_GOOMBA:
	case Effects::EFFECT_COIN:
	case Effects::EFFECT_TEXT:
	case Effects::EFFECT_FLAG:
	case Effects::EFFECT_MARIO_DEATH:
		for (size_t i = 0; i < currentImages.size(); i++)
		{
			UpdateImage(currentImages[i], camera);
			currentImages[i]->Draw();
		}
		break;
	case Effects::EFFECT_PLAYER_REGULAR:
	case Effects::EFFECT_PLAYER_SMALL:
		if (stage == 2)
		{
			SDL_Rect clipRect = { 0,0, static_cast<int>((startPosition.x - camera->worldPosition.x + 4.5f*TILESIZE) * camera->zoom + WINDOW_WIDTH * 0.5f), WINDOW_HEIGHT };
			SDL_RenderSetClipRect(renderer, &clipRect);
		}
		for (size_t i = 0; i < currentImages.size(); i++)
		{
			UpdateImage(currentImages[i], camera);
			currentImages[i]->Draw();
		}
		if (stage == 2)
		{
			SDL_RenderSetClipRect(renderer, nullptr);
		}
		break;

	case Effects::EFFECT_BRICK:
		for (size_t i = 0; i < currentImages.size(); i++)
		{
			//currentImages[i]->Draw();
			currentImages[i]->Draw(this->angleOfRotation, SDL_FLIP_NONE);
		}
		break;
	case Effects::EFFECT_TRANSITION:
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		for (size_t i = 0; i < currentImages.size(); i++)
		{
			UpdateImage(currentImages[i], camera);
			currentImages[i]->Draw();
		}
		break;
	}
}

void Effect::Draw(Camera * camera, double angle, SDL_RendererFlip flip, std::vector<SoundObject**>* sounds)
{
	Animate(elapsedTime, camera, sounds);

	switch (effectType)
	{
	case Effects::EFFECT_GOOMBA:
	case Effects::EFFECT_COIN:
	case Effects::EFFECT_TEXT:
	case Effects::EFFECT_FLAG:
	case Effects::EFFECT_MARIO_DEATH:
		for (size_t i = 0; i < currentImages.size(); i++)
		{
			UpdateImage(currentImages[i], camera);
			currentImages[i]->Draw(angle, flip);
		}
		break;
	case Effects::EFFECT_PLAYER_REGULAR:
	case Effects::EFFECT_PLAYER_SMALL:
		if (stage == 2)
		{
			SDL_Rect clipRect = { 0,0,static_cast<int>(WINDOW_WIDTH - startPosition.x + 4 * TILESIZE), WINDOW_HEIGHT };
			SDL_RenderSetClipRect(renderer, &clipRect);
		}
		for (size_t i = 0; i < currentImages.size(); i++)
		{
			UpdateImage(currentImages[i], camera);
			currentImages[i]->Draw(angle, flip);
		}
		if (stage == 2)
		{
			SDL_RenderSetClipRect(renderer, nullptr);
		}
		break;

	case Effects::EFFECT_BRICK:
		for (size_t i = 0; i < currentImages.size(); i++)
		{
			currentImages[i]->Draw(this->angleOfRotation + angle, flip);
		}
		break;
	}
}

Coordf const  Effect::GetWorldPosition()
{
	return worldPosition;
}

int const Effect::GetWidth()
{
	return width;
}

int const Effect::GetHeight()
{
	return height;
}
