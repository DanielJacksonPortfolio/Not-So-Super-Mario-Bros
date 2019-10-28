#pragma once
#include <SDL_ttf.h>
#include <string>

class Font
{
private:
	int size = 0;
	std::string path = "";
public:
	Font(std::string fontPath, int fontSize);
	~Font();
	void UpdateSize(int size);
	void Update();
	void UpdateFont(std::string path);
	TTF_Font * font = nullptr;
};

