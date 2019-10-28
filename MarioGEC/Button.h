#pragma once
#include "Widget.h"
#include "Text.h"

class Button : public Widget
{
private:

	Text* caption = nullptr;
	int captionDirection = Directions::DOWN;
	int ID = 0;
	int offset = 16;
	int imageW;
	int	imageH;
	int	surfaceW;
	int	surfaceH;
	int fontSize;


public:
	Button(Coordf position, Coordf origin, int  surfaceW, int surfaceH, Image * * image, SDL_Renderer * renderer, int width, int height, int offset, std::string text, std::string font, int fontSize, int color, int color2, int direction, std::function<void()> executable);
	~Button(); 
	void UpdateText(std::string text);
	void UpdateFont(std::string path);
	void UpdateColor(int color, int colorNum);
	void UpdateSize(int ptSize);
	void UpdateImage(Coordf boxPosition) override;
	void UpdatePosition(Coordf position);
	void UpdateOrigin(Coordf position);
	void UpdateRenderMode(int renderMode);
	void UpdateWrap(int wrapLength);
	void Draw(double angle, SDL_RendererFlip flip) override;
	void Draw() override;
	void DrawDebug() override;
	std::string GetText() { return caption->GetText(); }
	std::string virtual GetType() { return "Button"; }
	void UpdateCaptionDirection(int direction);
	void UpdateHitbox();
	void NormalizeFontSize(int = 0) override;

};