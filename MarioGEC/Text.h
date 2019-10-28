#pragma once
#include "Widget.h"

class Text : public Widget
{
private:
	std::string text = " ";
	Font * font = nullptr;
	int color = 0, color2 = 0;
	int renderMode = 0;
	int wrapLength = 0;
	int fontSize = 0;

public:
	Text(Coordf origin, Coordf position, std::string text, std::string fontPath, int fontSize, int color, int color2, int renderMode, SDL_Renderer * renderer, int wrapLength);
	~Text();
	void UpdateText(std::string text);
	void UpdateFont(std::string path);
	void UpdateColor(int color, int colorNum);
	void UpdateSize(int ptSize);
	void UpdateImage();
	void UpdatePosition(Coordf position);
	void UpdateOrigin(Coordf position);
	void UpdateRenderMode(int renderMode);
	void NormalizeFontSize(int boxSize);
	void UpdateWrap(int wrapLength);
	void virtual Draw(double angle, SDL_RendererFlip flip) override;
	void virtual Draw() override;
	void virtual DrawDebug() override;
	std::string GetText() { return text; }
	std::string virtual GetType() { return "Text"; }
};

