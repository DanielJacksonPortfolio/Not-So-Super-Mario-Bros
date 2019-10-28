#include "Font.h"



Font::Font(std::string fontPath, int fontSize)
{
	this->path = fontPath;
	this->size = fontSize;
	Update();
}


Font::~Font()
{
	TTF_CloseFont(font);
	delete font;
}

void Font::UpdateSize(int size)
{
	this->size = size;
	Update();
}

void Font::UpdateFont(std::string path)
{
	this->path = path;
	Update();
}

void Font::Update()
{
	TTF_CloseFont(font);
	font = TTF_OpenFont(path.c_str(), size);
}