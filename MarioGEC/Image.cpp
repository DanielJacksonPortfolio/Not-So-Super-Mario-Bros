#include "Image.h"
#include <iostream>
#include "Constants.h"

Image::Image(SDL_Rect textureRect)
{
	this->textureRect = textureRect;
	this->renderQuad = { 0, 0, 0, 0 };
	this->centre = { 0, 0 };
}

void Image::LoadRenderer(SDL_Renderer * renderer)
{
	this->renderer = renderer;
}

Image::~Image()
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
	}
	SDL_DestroyRenderer(renderer);
}

void Image::Load(std::string path)
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
	}
	SDL_Surface * surface = IMG_Load(path.c_str());
	if (surface == nullptr)
	{
		std::cout << "Image Load failed: %s! SDL_image Error: %s\n" << path.c_str() << IMG_GetError() << std::endl;
	}
	else
	{
		textureW = surface->w;
		textureH = surface->h;
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == nullptr)
		{
			std::cout << "Texture Creation Failed: %s! SDL Error: %s\n" << path.c_str() << SDL_GetError() << std::endl;
		}

		SDL_FreeSurface(surface);
	}
}

SDL_Surface * Image::GetTextSurface(std::string text, TTF_Font * font, int color, int color2, int renderMode, int wrapLength)
{
	switch (renderMode)
	{
	case TextRenderModes::SOLID:
		return TTF_RenderText_Solid(font, text.c_str(), colors[color]);	
	case TextRenderModes::SHADED:
		return TTF_RenderText_Shaded(font, text.c_str(), colors[color], colors[color2]);
	case TextRenderModes::BLENDED:
		return TTF_RenderText_Blended(font, text.c_str(), colors[color]);
	case TextRenderModes::WRAPPED:
		return TTF_RenderText_Blended_Wrapped(font, text.c_str(), colors[color], static_cast<Uint32>(wrapLength));
	default:
		return TTF_RenderText_Solid(font, text.c_str(), colors[color]);
	}
}

void Image::LoadText(std::string text, TTF_Font * font, int color, int color2, int renderMode, int wrapLength)
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
	}
	SDL_Surface * surface = GetTextSurface(text,font,color,color2,renderMode,wrapLength);

	if (surface == nullptr)
	{
		std::cout << "Text Load failed" << std::endl;
	}
	else
	{
		textureW = surface->w;
		textureH = surface->h;
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture == nullptr)
		{
			std::cout << "Texture Creation Failed: %s! SDL Error: %s\n" << text.c_str() << SDL_GetError() << std::endl;
		}

		SDL_FreeSurface(surface);
	}

	textureRect = {0,0,textureW,textureH};
}

void Image::Draw(double angle, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(renderer, texture, &textureRect, &renderQuad, angle, &centre, flip);
}

void Image::Draw()
{
	SDL_RenderCopy(renderer, texture, &textureRect, &renderQuad);
}

void Image::UpdatePosition(SDL_Rect renderQuad)
{
	this->renderQuad = renderQuad;
}

void Image::UpdateTextureRect(SDL_Rect textureRect)
{
	this->textureRect = textureRect;
}
