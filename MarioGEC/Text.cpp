#include "Text.h"



Text::Text(Coordf origin, Coordf position, std::string text, std::string fontPath, int fontSize, int color, int color2, int renderMode, SDL_Renderer * renderer, int wrapLength)
{

	this->text = text;
	this->font = new Font(fontPath,fontSize);
	this->fontSize = fontSize;
	this->color = color;
	this->color2 = color2;
	this->origin = origin;
	this->position = position;
	this->renderMode = renderMode;
	this->wrapLength = wrapLength;
	this->renderer = renderer;
	image = new Image({ 0, 0, 0, 0 });
	image->LoadRenderer(this->renderer);
	UpdateImage();
}

void Text::NormalizeFontSize(int boxSize)
{
	if (width > boxSize)
		fontSize = 0;
	while (width < boxSize || fontSize == 0)
	{
		this->UpdateSize(++fontSize);
	}
	if (width > boxSize)
	{
		fontSize--;
		this->UpdateSize(fontSize);
	}
}

Text::~Text()
{
	image->destroy();
	SDL_DestroyRenderer(renderer);
	delete font;
}

void Text::UpdateText(std::string text)
{
	this->text = text;
	UpdateImage();
}

void Text::UpdateWrap(int wrapLength)
{
	this->wrapLength = wrapLength;
	UpdateImage();
}

void Text::UpdateFont(std::string path)
{
	font->UpdateFont(path);
	UpdateImage();
}

void Text::UpdateColor(int color, int colorNum)
{
	if (colorNum == 1)
		this->color = color;
	else
		this->color2 = color;
	UpdateImage();
}

void Text::UpdatePosition(Coordf position)
{
	this->position.x = position.x;
	this->position.y = position.y;
	image->UpdatePosition({ static_cast<int>(this->position.x + this->origin.x),static_cast<int>(this->position.y + this->origin.y),width,height });
}

void Text::UpdateOrigin(Coordf newOrigin)
{
	this->origin = newOrigin;
	UpdatePosition(this->position);
}

void Text::UpdateRenderMode(int renderMode)
{
	this->renderMode = renderMode;
	UpdateImage();
}

void Text::UpdateImage()
{
	image->LoadText(text, font->font, color, color2, renderMode,wrapLength);
	width = image->textureW;
	height = image->textureH;
	image->UpdatePosition({ static_cast<int>(this->position.x + this->origin.x),static_cast<int>(this->position.y + this->origin.y),width,height });

}

void Text::UpdateSize(int ptSize)
{
	font->UpdateSize(ptSize);
	UpdateImage();
}

void Text::Draw()
{
	if (image != nullptr)
	{
		image->Draw();
	}
}

void Text::DrawDebug()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	if (image != nullptr)
	{
		SDL_Rect widgetRect = { static_cast<int>(position.x + origin.x),static_cast<int>(position.y + origin.y),static_cast<int>(width),static_cast<int>(height) };
		SDL_RenderDrawRect(renderer, &widgetRect);
		SDL_RenderDrawLine(renderer, static_cast<int>(position.x + origin.x), static_cast<int>(position.y + origin.y), static_cast<int>(position.x + origin.x + width), static_cast<int>(position.y + origin.y + height));
	}
}

void Text::Draw(double angle, SDL_RendererFlip flip)
{
	if (image != nullptr)
	{
		image->Draw(angle, flip);
	}
}