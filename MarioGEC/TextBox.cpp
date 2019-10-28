#include "TextBox.h"

TextBox::TextBox(Coordf position, SDL_Renderer * renderer, int width, int height, bool wrapText, bool lockWidth, bool lockHeight, Coordf widgetSpacing, std::string text, std::string font,int fontSize, int color, int color2, int renderMode)
{
	this->widgetSpacing = widgetSpacing;
	this->position = position;
	this->renderer = renderer;
	if (width > 0)
		this->width = width;
	else
		this->width = 1;
	this->height = height;
	this->wrapText = wrapText;
	this->lockWidth  = lockWidth;
	this->lockHeight = lockHeight;
	if (wrapText)
		this->lockHeight = false;
	this->LoadImages();

	widgets.push_back(new Text({position.x, position.y}, { widgetSpacing.x, widgetSpacing.y },text, font, fontSize, color, color2, renderMode, this->renderer, this->wrapLength));

	if (wrapText)
	{
		this->UpdateRenderMode(0,TextRenderModes::WRAPPED);
	}
	
	this->Update();
}

TextBox::~TextBox()
{
	delete renderer;
	for (size_t i = 0; i < tileImages.size(); i++)
	{
		tileImages[i]->destroy();
	}
	for (size_t i = 0; i < widgets.size(); i++)
	{
		delete widgets[i];
	}
}


void TextBox::DrawDebug()
{
	SDL_SetRenderDrawColor(renderer, 0, 127, 255, 255);
	SDL_Rect boxRect = {position.x,position.y,width,height };
	SDL_RenderDrawRect(renderer, &boxRect );
	SDL_RenderDrawLine(renderer, position.x, position.y, position.x + width, position.y + height);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (size_t i = 0; i < widgets.size(); i++)
	{
		SDL_Rect widgetRect = { widgets[i]->GetPosition().x,widgets[i]->GetPosition().y,widgets[i]->GetWidth(),widgets[i]->GetHeight() };
		SDL_RenderDrawRect(renderer, &widgetRect);
		SDL_RenderDrawLine(renderer, widgets[i]->GetPosition().x, widgets[i]->GetPosition().y, widgets[i]->GetPosition().x + widgets[i]->GetWidth(), widgets[i]->GetPosition().y + widgets[i]->GetHeight());
	}

}

void TextBox::Update()
{

	int totalWidgetW = 0;
	int totalWidgetH = 0;

	for (size_t i = 0; i < widgets.size(); i++)
	{
		totalWidgetW += widgets[i]->GetWidth();
		totalWidgetH += widgets[i]->GetHeight();
	}

	if (lockWidth == false)
	{
		width = totalWidgetW + widgetSpacing.x * (widgets.size() + 1);
		if (width % 32 == 0) width += 32;
	}
	if (lockHeight == false)
	{
		height= totalWidgetH + widgetSpacing.y * (widgets.size() + 1);
		if (height % 32 == 0) height += 32;
	}

	if (wrapText)
	{
		int oldWrapLength = wrapLength;
		this->wrapLength = width - widgetSpacing.x * 2;
		if (wrapLength != oldWrapLength)
		{
			this->UpdateWrap(0,wrapLength);
		}
	}

	//AutoScale();

	UpdateTiles();

}

void TextBox::AutoScale()
{
	int totalWidgetW = 0;
	int totalWidgetH = 0;

	for (size_t i = 0; i < widgets.size(); i++)
	{
		totalWidgetW += widgets[i]->GetWidth();
		totalWidgetH += widgets[i]->GetHeight();
	}

	oldOffset = widgetSpacing;

	widgetSpacing.x = (width - totalWidgetW) / (widgets.size() + 1);
	widgetSpacing.y = (height - totalWidgetH) / (widgets.size() + 1);

	if(widgetSpacing.x * (widgets.size() + 1) >= 48)
		widgetSpacing.x -= 32;
	if (widgetSpacing.y * (widgets.size() + 1) >= 48)
		widgetSpacing.y -= 32;


	for (size_t i = 0; i < widgets.size(); i++)
	{
		UpdateTextPosition(i);
	}

}