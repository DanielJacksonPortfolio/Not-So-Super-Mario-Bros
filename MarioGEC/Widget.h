#pragma once
#include "Image.h"
#include "Camera.h"
#include "Constants.h"
#include "Font.h"
#include "string"
#include <functional>

class Widget
{
public:
	Widget();
	~Widget();
	void virtual Draw(double angle, SDL_RendererFlip flip) {}
	void virtual Draw() {}
	void virtual DrawDebug() {}
	int const GetWidth() { return width; }
	int const GetHeight() { return height; }
	Coordf const GetPosition() { return position; }
	Coordf const GetOrigin() { return origin; }
	void virtual UpdateText(std::string text) {}
	void virtual UpdateFont(std::string path) {}
	void virtual UpdateColor(int color, int colorNum){}
	void virtual UpdateSize(int ptSize) {}
	void virtual UpdateImage(Coordf boxPosition) {}
	void virtual UpdatePosition(Coordf position) {}
	void virtual UpdateOrigin(Coordf position) {}
	void virtual UpdateRenderMode(int renderMode) {}
	void virtual UpdateWrap(int wrapLength) {}
	std::string virtual GetText() { return ""; }
	std::string virtual GetType() { return ""; }
	void virtual UpdateHitbox() {}
	void virtual NormalizeFontSize(int boxSize) {}
	std::function<void()> GetExecutable() { return executable; }

	Image * image = nullptr;
	int width = 1, height = 1;
	Coordf position = { 0.0f,0.0f };
	Coordf origin = { 0.0f,0.0f };
	SDL_Rect clipRect = { 0,0,0,0 };
	SDL_Rect hitbox = { 0,0,0,0 };
	SDL_Renderer * renderer = nullptr;
	std::function<void()> executable;

};

