#include "Button.h"



Button::Button(Coordf position, Coordf origin, int  surfaceW, int surfaceH,Image * * image, SDL_Renderer * renderer, int width, int height, int offset, std::string text, std::string font, int fontSize, int color, int color2, int direction, std::function<void()> executable)
{
	this->origin = origin;
	this->offset = offset;
	this->position = position;
	this->renderer = renderer;
	this->image = *image;
	this->imageW = width;
	this->imageH = height;
	this->fontSize = fontSize;
	this->captionDirection = direction;
	this->surfaceW = surfaceW;
	this->surfaceH = surfaceH;
	this->executable = executable;
	if (direction == UP || direction == DOWN)
		caption = new Text({origin.x + position.x,origin.y + position.y }, {0,0}, text, font, fontSize, color, color2, TextRenderModes::WRAPPED, this->renderer, static_cast<int>(width*1.25));
	else
		caption = new Text({ origin.x + position.x,origin.y + position.y }, { 0,0 }, text, font, fontSize, color, color2, TextRenderModes::BLENDED, this->renderer, static_cast<int>(width));
	this->image->UpdatePosition({ static_cast<int>(this->position.x + this->origin.x), static_cast<int>(this->position.y + this->origin.y),width,height });
	this->clipRect = { static_cast<int>(origin.x + 16), static_cast<int>(origin.y + 16),surfaceW - 32,surfaceH - 32 };

	UpdateCaptionDirection(this->captionDirection);
}


Button::~Button()
{
	delete caption;
	image->destroy();
	SDL_DestroyRenderer(renderer);
}

void Button::UpdateText(std::string text)
{
	caption->UpdateText(text);
}

void Button::UpdateWrap(int wrapLength)
{
	caption->UpdateWrap(wrapLength);
}

void Button::UpdateFont(std::string path)
{
	caption->UpdateFont(path);
}

void Button::UpdateColor(int color, int colorNum)
{
	caption->UpdateColor(color, colorNum);
}

void Button::UpdatePosition(Coordf position)
{
	this->position = position;
	UpdateCaptionDirection(this->captionDirection);
}

void Button::UpdateOrigin(Coordf newOrigin)
{
	this->origin = newOrigin;
	UpdateCaptionDirection(captionDirection);
}

void Button::UpdateRenderMode(int renderMode)
{
	caption->UpdateRenderMode(renderMode);
}

void Button::UpdateSize(int ptSize)
{
	caption->UpdateSize(ptSize);
}

void Button::UpdateHitbox()
{
	hitbox = { static_cast<int>(origin.x + position.x),static_cast<int>(origin.y + position.y),width,height };
}

void Button::NormalizeFontSize(int boxWidth)
{
	if (caption->GetWidth() > imageW)
		fontSize = 0;
	while (caption->GetWidth() <= imageW || fontSize == 0)
	{
		caption->UpdateSize(fontSize++);
	}
	if (caption->GetWidth() > imageW)
		caption->UpdateSize(--fontSize);
}

void Button::UpdateCaptionDirection(int direction)
{
	captionDirection = direction;
	switch (captionDirection)
	{
	case Directions::NONE:
		image->UpdatePosition({ static_cast<int>(origin.x + position.x),static_cast<int>(origin.y + position.y),imageW,imageH });

		UpdateHitbox();
		break;
	case Directions::LEFT:
		//NormalizeFontSize();
		this->width = imageW + caption->GetWidth() + offset;
		if (imageH < caption->GetHeight())
			this->height = caption->GetHeight();
		else
			this->height = imageH;

		caption->UpdateOrigin({ static_cast<float>(origin.x + position.x), static_cast<float>(origin.y + position.y + (imageH - caption->GetHeight()) * 0.5f) });
		image->UpdatePosition({ static_cast<int>(origin.x + position.x + caption->GetWidth() + offset),static_cast<int>(origin.y + position.y),imageW,imageH });

		UpdateHitbox();
		break;
	case Directions::RIGHT:
		//NormalizeFontSize();
		this->width = imageW + caption->GetWidth() + offset;
		if (imageH < caption->GetHeight())
			this->height = caption->GetHeight();
		else
			this->height = imageH;

		caption->UpdateOrigin({ static_cast<float>(origin.x + position.x + imageW + offset), static_cast<float>(origin.y + position.y + (imageH - caption->GetHeight())*0.5f) });
		image->UpdatePosition({ static_cast<int>(origin.x + position.x),static_cast<int>(origin.y + position.y),imageW,imageH });

		UpdateHitbox();

		break;
	case Directions::DOWN:
		NormalizeFontSize();
		this->height = imageH + caption->GetHeight() + offset;
		if (imageW < caption->GetWidth())
			this->width  = caption->GetWidth();
		else
			this->width = imageW;


		caption->UpdateOrigin({ static_cast<float>(origin.x + position.x), static_cast<float>(origin.y + position.y + imageH + offset)});
		image->UpdatePosition({ static_cast<int>(origin.x + position.x),static_cast<int>(origin.y + position.y),imageW,imageH });

		UpdateHitbox();
		break;
	case Directions::UP:
		NormalizeFontSize();
		this->height = imageH + caption->GetHeight() + offset;
		if (imageW < caption->GetWidth())
			this->width = caption->GetWidth();
		else
			this->width = imageW;

		caption->UpdateOrigin({ static_cast<float>(origin.x + position.x), static_cast<float>(origin.y + position.y) });
		image->UpdatePosition({ static_cast<int>(origin.x + position.x),static_cast<int>(origin.y + position.y + caption->GetHeight() + offset),imageW,imageH });

		UpdateHitbox();
		break;

	}
}

void Button::UpdateImage(Coordf boxPosition)
{
	UpdateOrigin(boxPosition);
}

void Button::Draw()
{
	SDL_RenderSetClipRect(renderer, &clipRect);
	if (image != nullptr)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderFillRect(renderer, &image->renderQuad);
		image->Draw();
	}
	if (caption != nullptr)
	{
		caption->Draw();
	}
	SDL_RenderSetClipRect(renderer, nullptr);
}

void Button::DrawDebug()
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderSetClipRect(renderer, &clipRect);
	if (image != nullptr)
	{
		SDL_Rect widgetRect = { static_cast<int>(position.x + origin.x),static_cast<int>(position.y + origin.y),static_cast<int>(width),static_cast<int>(height) };
		SDL_RenderDrawRect(renderer, &widgetRect);
		SDL_RenderDrawLine(renderer, static_cast<int>(position.x + origin.x), static_cast<int>(position.y + origin.y), static_cast<int>(position.x + origin.x + width), static_cast<int>(position.y + origin.y + height));
	}
	if (caption != nullptr)
	{
		caption->DrawDebug();
	}
	SDL_RenderSetClipRect(renderer, nullptr);
}

void Button::Draw(double angle, SDL_RendererFlip flip)
{
	SDL_RenderSetClipRect(renderer, &clipRect);
	if (image != nullptr)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderFillRect(renderer, &image->renderQuad);
		image->Draw(angle, flip);
	}
	if (caption != nullptr)
	{
		caption->Draw(angle, flip);
	}
	SDL_RenderSetClipRect(renderer, nullptr);
}