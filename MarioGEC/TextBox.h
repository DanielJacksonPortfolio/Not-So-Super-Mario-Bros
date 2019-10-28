#pragma once

#include "Box.h"

class TextBox :public Box
{
private:
	bool wrapText = false;
	int wrapLength = 16;

public:
	TextBox(Coordf position, SDL_Renderer * renderer, int width, int height, bool wrapText,bool lockWidth, bool lockHeight, Coordf widgetSpacing, std::string text, std::string font, int fontSize, int color, int color2, int renderMode);
	~TextBox();
	void virtual Update() override;

	void AutoScale();
	void DrawDebug();
};

