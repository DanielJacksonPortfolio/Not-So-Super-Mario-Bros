#pragma once

#include "Constants.h"
#include "Image.h"
#include "Camera.h"
#include "Level.h"
#include "Text.h"
#include "SoundObject.h"

class Effect
{
private:

	std::vector<Image *> images = {};
	std::vector<Image *> currentImages = {};
	Coordf worldPosition = Coordf(0.0f, 0.0f);
	int width = TILESIZE, height = TILESIZE;
	SDL_Renderer* renderer = nullptr;
	int effectType = 0;
	float length = 0;
	float initialLength = 0;
	Coordf startPosition = Coordf(0, 0);
	float startX = 0, finalX = 0;
	std::string text;
	Text* textObj = nullptr; 
	float x = 0;
	int elapsedTime = 0;
	double angleOfRotation = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	int stage = 0;

public:

	Effect(Coordf position, int effectType, SDL_Renderer* renderer, std::vector<Image*> effectImages, float = -1, float = -1, std::string = "N/A", int fontSize = 20);
	~Effect();
	void Draw(Camera * camera, std::vector<SoundObject**>* sounds);
	void Draw(Camera * camera, double angle, SDL_RendererFlip flip, std::vector<SoundObject**>* sounds);
	void UpdateImage(Image* image, Camera * camera);
	void InitializeImages(std::vector<Image*>  effectImages);

	Coordf const GetWorldPosition();
	int const GetWidth();
	int const GetHeight();
	int const GetEffectType() { return effectType; }

	virtual void Update(int elapsedTime, Level* level, std::vector<SoundObject**>* sounds);
	virtual void Animate(int elapsedTime, Camera * camera, std::vector<SoundObject**>* sounds);

	int animationTimer = 0;
	int currentFrame = 0;
	bool complete = false;
	int fontSize = 20;
};