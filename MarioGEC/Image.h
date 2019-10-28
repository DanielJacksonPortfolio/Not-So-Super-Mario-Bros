#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

class Image
{
private:
	SDL_Texture * texture = nullptr;
	SDL_Renderer * renderer = nullptr;
public:
	SDL_Rect textureRect;
	SDL_Rect renderQuad;
	SDL_Point centre;

	int textureW = 0, textureH = 0;

	Image(SDL_Rect textureRect); 
	void destroy() { delete this; }
	void LoadRenderer(SDL_Renderer * renderer);
	void Load(std::string path);
	void LoadText(std::string text, TTF_Font * font, int color, int color2, int renderMode, int wrapLength);
	SDL_Surface * GetTextSurface(std::string text, TTF_Font * font, int color, int color2, int renderMode, int wrapLength);
	void Draw(double angle, SDL_RendererFlip flip);
	void Draw();
	void UpdatePosition(SDL_Rect renderQuad);
	void UpdateTextureRect(SDL_Rect textureRect);

protected:
	~Image();
};

