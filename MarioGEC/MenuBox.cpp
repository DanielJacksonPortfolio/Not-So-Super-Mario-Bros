#include "MenuBox.h"

MenuBox::MenuBox(Coordf position, SDL_Renderer * renderer, int width, int height, bool lockWidth, bool lockHeight, Coordf widgetSpacing)
{
	this->widgetSpacing = widgetSpacing;
	this->position = position;
	this->renderer = renderer;
	this->width = width;
	this->height = height;
	this->lockWidth = lockWidth;
	this->lockHeight = lockHeight;
	this->LoadImages();
	this->relativePosition = { 0,0 };
	this->UpdateHitbox();
	this->Update();
}

MenuBox::~MenuBox()
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

void MenuBox::DrawDebug()
{
	SDL_SetRenderDrawColor(renderer, 0, 127, 255, 255);
	SDL_Rect boxRect = { position.x,position.y,width,height };
	SDL_RenderDrawRect(renderer, &boxRect);
	SDL_RenderDrawLine(renderer, position.x, position.y, position.x + width, position.y + height);
	for (size_t i = 0; i < widgets.size(); i++)
	{
		if (widgets[i]->GetType() == "Button")
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		else
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect widgetRect = { widgets[i]->GetPosition().x,widgets[i]->GetPosition().y,widgets[i]->GetWidth(),widgets[i]->GetHeight() };
		SDL_RenderDrawRect(renderer, &widgetRect);
		SDL_RenderDrawLine(renderer, widgets[i]->GetPosition().x, widgets[i]->GetPosition().y, widgets[i]->GetPosition().x + widgets[i]->GetWidth(), widgets[i]->GetPosition().y + widgets[i]->GetHeight());
	}

}

void MenuBox::Update()
{

	if (relativePosition.y > 0) relativePosition.y = 0;

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
		height = totalWidgetH + widgetSpacing.y * (widgets.size() + 1);
		if (height % 32 == 0) height += 32;
	}


	//AutoScale();

	for (size_t i = 0; i < widgets.size(); i++)
	{
		widgets[i]->UpdateImage(relativePosition);
		widgets[i]->UpdateHitbox();
	}

	UpdateTiles();

}

void MenuBox::LoadImages()
{
	for (size_t i = 0; i < tileImages.size(); i++)
	{
		tileImages[i]->LoadRenderer(this->renderer);
		tileImages[i]->Load("resources/textures/menuTiles.png");
	}
	for (size_t i = 0; i < buttonImages.size(); i++)
	{
		buttonImages[i]->LoadRenderer(this->renderer);
		buttonImages[i]->Load("resources/textures/tiles.png");
	}
}

void MenuBox::AutoScale()
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

	if (widgetSpacing.x * (widgets.size() + 1) >= 48)
		widgetSpacing.x -= 32;
	if (widgetSpacing.y * (widgets.size() + 1) >= 48)
		widgetSpacing.y -= 32;


	for (size_t i = 0; i < widgets.size(); i++)
	{
		UpdateTextPosition(i);
	}

}


void MenuBox::AddText(Coordf position, std::string text, std::string font, int fontSize, int color, int color2, int renderMode, int wrapLength)
{
	widgets.push_back(new Text({ this->position.x, this->position.y }, position, text, font, fontSize, color, color2, renderMode, this->renderer, wrapLength));
}

void MenuBox::AddButton(Coordf position, int imageID, int width, int height, int offset, std::string font, int fontSize, int color, int color2,int direction)
{
	switch (imageID)
	{
	case Buttons::BRICK:
		widgets.push_back(new Button(position, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BRICK", font, fontSize, color, color2,direction));
		break;
	case Buttons::BLOCK:
		widgets.push_back(new Button(position, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BLOCK", font, fontSize, color, color2, direction));
		break;
	case Buttons::SPECIAL:
		widgets.push_back(new Button(position, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "SPECIAL", font, fontSize, color, color2, direction));
		break;
	case Buttons::FLOOR:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "FLOOR", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE1:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BL", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE2:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BR", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE3:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE TL", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE4:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE TR", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE5:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BT", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE6:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BB", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE7:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BL", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE8:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BR", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE9:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE LT", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE10:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE LB", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE11:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE RT", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE12:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE RB", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE13:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE ITL", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE14:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE ITR", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE15:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE IBL", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE16:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE IBR", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE17:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE CTL", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE18:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE CTR", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE19:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE CBL", font, fontSize, color, color2, direction));
		break;
	case Buttons::PIPE20:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE CBR", font, fontSize, color, color2, direction));
		break;
	case Buttons::CLOUDTL:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD TL", font, fontSize, color, color2, direction));
		break;
	case Buttons::CLOUDTM:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD TM", font, fontSize, color, color2, direction));
		break;
	case Buttons::CLOUDTR:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD TR", font, fontSize, color, color2, direction));
		break;
	case Buttons::CLOUDBL:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD BL", font, fontSize, color, color2, direction));
		break;
	case Buttons::CLOUDBM:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD BM", font, fontSize, color, color2, direction));
		break;
	case Buttons::CLOUDBR:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD BR", font, fontSize, color, color2, direction));
		break;
	case Buttons::BUSHL:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BUSH LEFT", font, fontSize, color, color2, direction));
		break;
	case Buttons::BUSHM:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BUSH MID", font, fontSize, color, color2, direction));
		break;
	case Buttons::BUSHR:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BUSH RIGHT", font, fontSize, color, color2, direction));
		break;
	case Buttons::CASTLEARCH1:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE A1", font, fontSize, color, color2, direction));
		break;
	case Buttons::CASTLEARCH2:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE A2", font, fontSize, color, color2, direction));
		break;
	case Buttons::CASTLECREN1:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE C1", font, fontSize, color, color2, direction));
		break;
	case Buttons::CASTLECREN2:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE C2", font, fontSize, color, color2, direction));
		break;
	case Buttons::CASTLEWINLEFT:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE WL", font, fontSize, color, color2, direction));
		break;
	case Buttons::CASTLEWINRIGHT:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE WR", font, fontSize, color, color2, direction));
		break;
	case Buttons::HILLBERRY:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL BERRY", font, fontSize, color, color2, direction));
		break;
	case Buttons::HILLLEFT:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL LEFT", font, fontSize, color, color2, direction));
		break;
	case Buttons::HILLTOP:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL TOP", font, fontSize, color, color2, direction));
		break;
	case Buttons::HILLRIGHT:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL RIGHT", font, fontSize, color, color2, direction));
		break;
	case Buttons::HILLFILL:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL", font, fontSize, color, color2, direction));
		break;
	case Buttons::FLAGPOLE:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "FLAGPOLE", font, fontSize, color, color2, direction));
		break;
	case Buttons::FLAGTOP:
		widgets.push_back(new Button(position, { this->position.x, this->position.y },this->width,this->height, &buttonImages[imageID], this->renderer, width, height, offset, "FLAG TOP", font, fontSize, color, color2, direction));
		break;
	}
	
}