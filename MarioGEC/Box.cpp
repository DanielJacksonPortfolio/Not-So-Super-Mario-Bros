#include "Box.h"


Box::Box(Coordf position, SDL_Renderer * renderer, int width, int height, bool lockWidth, bool lockHeight, Coordf defaultSpacing)
{
	this->tabs.push_back({});
	this->tabs[0].pages.push_back({});
	this->tabs[0].pages[0].rows.push_back({});
	this->UpdatePageDimensions(0, 0);
	this->defaultSpacing = defaultSpacing;
	this->position = position;
	this->renderer = renderer;
	if (width > 0)
		this->width = width;
	else
		this->width = 1;
	this->height = height;

	this->lockWidth = lockWidth;
	this->lockHeight = lockHeight;
	this->LoadImages();
	this->UpdateHitbox();
	this->UpdateTiles();
}

Box::~Box()
{
	SDL_DestroyRenderer(renderer);
	for (size_t i = 0; i < tileImages.size(); i++)
	{
		tileImages[i]->destroy();
	}
	for (size_t i = 0; i < buttonImages.size(); i++)
	{
		buttonImages[i]->destroy();
	}

	for (size_t i = 0; i < tiles.size(); i++)
	{
		tiles[i]->destroy();
	}

	for (size_t i = 0; i < tabs.size(); i++)
	{
		for (size_t j = 0; j < tabs[i].pages.size(); j++)
		{
			for (size_t k = 0; k < tabs[i].pages[j].rows.size(); k++)
			{
				for (size_t l = 0; l < tabs[i].pages[j].rows[k].widgets.size(); l++)
				{
					delete tabs[i].pages[j].rows[k].widgets[l];
				}
			}
		}
	}
}

void Box::LoadImages()
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


void Box::AddText(std::string text, std::string font, int fontSize, int color, int color2, int renderMode,bool wrapText, int wrapLength, int tab, int page, int row)
{
	tabs[tab].pages[page].rows[row].widgets.push_back(new Text({ this->position.x, this->position.y }, { 0,0 }, text, font, fontSize, color, color2, renderMode, this->renderer, wrapLength));
	this->wrapText = wrapText;
	if (wrapText)
	{
		this->lockHeight = false;
		this->UpdateRenderMode(tab,page,row, tabs[tab].pages[page].rows[row].widgets.size()-1, TextRenderModes::WRAPPED);
	}
}

void Box::AddButton(int imageID, int width, int height, int offset, std::string font, int fontSize, int color, int color2, int direction, int tab, int page, int row, std::function<void()>function)
{
	switch (imageID)
	{
	case Buttons::BUTTONS_PLAYGAME:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({0,0}, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, " PLAY GAME  ", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_QUIT:																																												    
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({0,0}, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, " QUIT       ", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_BRICK:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({0,0}, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BRICK", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_BLOCK:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({0,0}, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BLOCK", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_SPECIAL:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({0,0}, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "SPECIAL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_FLOOR:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "FLOOR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE1:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE2:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE3:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE TL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE4:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE TR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE5:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BT", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE6:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BB", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE7:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE8:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE BR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE9:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE LT", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE10:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE LB", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE11:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE RT", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE12:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE RB", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE13:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE ITL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE14:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE ITR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE15:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE IBL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE16:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE IBR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE17:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE CTL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE18:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE CTR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE19:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE CBL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_PIPE20:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "PIPE CBR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CLOUDTL:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD TL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CLOUDTM:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD TM", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CLOUDTR:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD TR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CLOUDBL:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD BL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CLOUDBM:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD BM", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CLOUDBR:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CLOUD BR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_BUSHL:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BUSH LEFT", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_BUSHM:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BUSH MID", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_BUSHR:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "BUSH RIGHT", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CASTLEARCH1:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE A1", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CASTLEARCH2:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE A2", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CASTLECREN1:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE C1", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CASTLECREN2:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE C2", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CASTLEWINLEFT:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE WL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_CASTLEWINRIGHT:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "CASTLE WR", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_HILLBERRY:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL BERRY", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_HILLLEFT:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL LEFT", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_HILLTOP:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL TOP", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_HILLRIGHT:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL RIGHT", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_HILLFILL:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "HILL", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_FLAGPOLE:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "FLAGPOLE", font, fontSize, color, color2, direction, function)); break;
	case Buttons::BUTTONS_FLAGTOP:
		tabs[tab].pages[page].rows[row].widgets.push_back(new Button({ 0,0 }, { this->position.x, this->position.y }, this->width, this->height, &buttonImages[imageID], this->renderer, width, height, offset, "FLAG TOP", font, fontSize, color, color2, direction, function)); break;
	}
}

void Box::UpdateTiles()
{
	int  const tilesX = roundUp(width, 32) / 32;
	int  const tilesY = roundUp(height, 32) / 32;

	width = tilesX * 32;
	height = tilesY * 32;

	std::vector<Image*> tempTiles = {};
	tiles.swap(tempTiles);

	for (int j = 0; j < tilesY; j++)
	{
		for (int i = 0; i < tilesX; i++)
		{
			if (j == 0 && i == 0)
			{
				tiles.push_back(new Image(*tileImages[0]));
			}
			else if (j == 0 && i == (tilesX - 1))
			{
				tiles.push_back(new Image(*tileImages[2]));
			}
			else if (j == (tilesY - 1) && i == 0)
			{
				tiles.push_back(new Image(*tileImages[6]));
			}
			else if (j == (tilesY - 1) && i == (tilesX - 1))
			{
				tiles.push_back(new Image(*tileImages[8]));
			}
			else if (j == 0)
			{
				tiles.push_back(new Image(*tileImages[1]));
			}
			else if (j == (tilesY - 1))
			{
				tiles.push_back(new Image(*tileImages[7]));
			}
			else if (i == 0)
			{
				tiles.push_back(new Image(*tileImages[3]));
			}
			else if (i == (tilesX - 1))
			{
				tiles.push_back(new Image(*tileImages[5]));
			}
			else
			{
				tiles.push_back(new Image(*tileImages[4]));
			}
			tiles[j * tilesX + i]->renderQuad = { static_cast<int>(position.x + i * 32), static_cast<int>(position.y + j * 32),32,32 };
		}
	}
	UpdateHitbox();
}

void Box::UpdateText(int tab, int page, int row, int widgetID, std::string text)
{
	if (tabs[tab].pages[page].rows[row].widgets[widgetID] != nullptr)
	{
		this->tabs[tab].pages[page].rows[row].widgets[widgetID]->UpdateText(text);
	}
	UpdateSpacing();
	this->Update();
}

void Box::UpdateWrap(int tab, int page, int row, int widgetID, int wrapLength)
{
	if (tabs[tab].pages[page].rows[row].widgets[widgetID] != nullptr)
	{
		tabs[tab].pages[page].rows[row].widgets[widgetID]->UpdateWrap(wrapLength);
	}
	UpdateSpacing();
}

void Box::UpdateFont(int tab, int page, int row, int widgetID, std::string path)
{
	if (tabs[tab].pages[page].rows[row].widgets[widgetID] != nullptr)
	{
		tabs[tab].pages[page].rows[row].widgets[widgetID]->UpdateFont(path);
	}
	UpdateSpacing();
	this->Update();
}

void Box::UpdateColor(int tab, int page, int row, int widgetID, int color, int colorNum)
{
	if (tabs[tab].pages[page].rows[row].widgets[widgetID] != nullptr)
	{
		tabs[tab].pages[page].rows[row].widgets[widgetID]->UpdateColor(color, colorNum);
	}
}

void Box::UpdatePosition(int tab, int page, int row, int widgetID, Coordf position)
{
	if (tabs[tab].pages[page].rows[row].widgets[widgetID] != nullptr)
	{
		tabs[tab].pages[page].rows[row].widgets[widgetID]->UpdateOrigin(position);
	}
	UpdateSpacing();
	this->Update();
}

void Box::UpdateTextPosition(int tab, int page, int row, int widgetID)
{
	float const deltaX = defaultSpacing.x - oldOffset.x;
	float const deltaY = defaultSpacing.y - oldOffset.y;
	if (tabs[tab].pages[page].rows[row].widgets[widgetID] != nullptr)
	{
		tabs[tab].pages[page].rows[row].widgets[widgetID]->UpdatePosition({ tabs[tab].pages[page].rows[row].widgets[widgetID]->GetPosition().x - tabs[tab].pages[page].rows[row].widgets[widgetID]->GetOrigin().x + deltaX , tabs[tab].pages[page].rows[row].widgets[widgetID]->GetPosition().y - tabs[tab].pages[page].rows[row].widgets[widgetID]->GetOrigin().y + deltaY });
	}
	UpdateSpacing();
}

void Box::UpdateRenderMode(int tab, int page, int row, int widgetID, int renderMode)
{
	if (tabs[tab].pages[page].rows[row].widgets[widgetID] != nullptr)
	{
		tabs[tab].pages[page].rows[row].widgets[widgetID]->UpdateRenderMode(renderMode);
	}
	UpdateSpacing();
	this->Update();
}

void Box::UpdateSize(int tab, int page, int row, int widgetID, int ptSize)
{
	if (tabs[tab].pages[page].rows[row].widgets[widgetID] != nullptr)
	{
		tabs[tab].pages[page].rows[row].widgets[widgetID]->UpdateSize(ptSize);
	}
	UpdateSpacing();
	this->Update();
}

void Box::UpdateRowDimensions(int tabID, int pageID, int rowID)
{
	tabs[tabID].pages[pageID].rows[rowID].width = 0;// static_cast<int>(defaultSpacing.x);
	tabs[tabID].pages[pageID].rows[rowID].height = 0;// static_cast<int>(defaultSpacing.y);
	int tallestWidget = 0;
	for (size_t l = 0; l < tabs[tabID].pages[pageID].rows[rowID].widgets.size(); l++)
	{
		tabs[tabID].pages[pageID].rows[rowID].width += (tabs[tabID].pages[pageID].rows[rowID].widgets[l]->GetWidth());
		if (tabs[tabID].pages[pageID].rows[rowID].widgets[l]->GetHeight() + defaultSpacing.y > tallestWidget)
			tallestWidget = static_cast<int>(tabs[tabID].pages[pageID].rows[rowID].widgets[l]->GetHeight() + defaultSpacing.y);
	}
	tabs[tabID].pages[pageID].rows[rowID].height = tallestWidget;
}

void Box::UpdatePageDimensions(int tabID, int pageID)
{
	int totalHeight = static_cast<int>(defaultSpacing.y);
	if (lockWidth == false)
		tabs[tabID].pages[pageID].width = 0;
	else
		tabs[tabID].pages[pageID].width = width;
	for (size_t k = 0; k < tabs[tabID].pages[pageID].rows.size(); k++)
	{
		UpdateRowDimensions(tabID, pageID, k);
		totalHeight += tabs[tabID].pages[pageID].rows[k].height;
		if (lockWidth == false)
		{
			if (tabs[tabID].pages[pageID].rows[k].width > tabs[tabID].pages[pageID].width)
			{
				tabs[tabID].pages[pageID].width = tabs[tabID].pages[pageID].rows[k].width;
			}
		}
	}
	tabs[tabID].pages[pageID].height = totalHeight;
	this->tabs[tabID].pages[pageID].maxHeight = static_cast<int>(this->height - 2 * defaultSpacing.y);
	this->tabs[tabID].pages[pageID].maxWidth = static_cast<int>(this->width - 2 * defaultSpacing.x);

}

void Box::UpdateAllPageDimensions()
{

	for (size_t i = 0; i < tabs.size(); i++)
	{
		for (size_t j = 0; j < tabs[i].pages.size(); j++)
		{
			UpdatePageDimensions(i, j);
		}
	}
}

void Box::UpdateWidgetRows()
{
	for (size_t i = 0; i < tabs.size(); i++)
	{
		for (size_t j = 0; j < tabs[i].pages.size(); j++)
		{
			for (size_t k = 0; k < tabs[i].pages[j].rows.size(); k++)
			{
				while (tabs[i].pages[j].rows[k].width > tabs[i].pages[j].maxWidth && lockWidth == true)
				{
					if (tabs[i].pages[j].rows[k].widgets.back()->GetType() == "Text")
					{
						tabs[i].pages[j].rows[k].widgets.back()->NormalizeFontSize(tabs[i].pages[j].maxWidth);
						UpdateRowDimensions(i, j, k);
					}
					else
					{
						if (k == tabs[i].pages[j].rows.size() - 1)
						{
							tabs[i].pages[j].rows.push_back({});
						}
						for (size_t m = k + 1; m < tabs[i].pages[j].rows.size(); m++)
						{
							int widgetID = 0;
							int widgetRunningWidth = 0;
							for (size_t l = 0; l < tabs[i].pages[j].rows[k].widgets.size(); l++)
							{
								widgetRunningWidth += tabs[i].pages[j].rows[k].widgets[l]->GetWidth();
								if (tabs[i].pages[j].maxWidth < widgetRunningWidth)
								{
									widgetID = l;
									break;
								}
							}
							if (tabs[i].pages[j].maxWidth - tabs[i].pages[j].rows[m].width > tabs[i].pages[j].rows[k].widgets[widgetID]->GetWidth())
							{
								tabs[i].pages[j].rows[m].widgets.push_back(tabs[i].pages[j].rows[k].widgets[widgetID]);
								tabs[i].pages[j].rows[k].widgets.erase(tabs[i].pages[j].rows[k].widgets.begin() + widgetID);

								UpdateRowDimensions(i, j, k);
								UpdateRowDimensions(i, j, m);
								break;
							}
							if (m == tabs[i].pages[j].rows.size() - 1)
							{
								tabs[i].pages[j].rows.push_back({});
							}
						}
					}
				}
			}
		}
	}
}

void Box::UpdateRowPages()
{

	for (size_t i = 0; i < tabs.size(); i++)
	{
		for (size_t j = 0; j < tabs[i].pages.size(); j++)
		{
			while (tabs[i].pages[j].height > tabs[i].pages[j].maxHeight)
			{

				if (j == tabs[i].pages.size() - 1)
				{
					tabs[i].pages.push_back({});
					UpdatePageDimensions(i, tabs[i].pages.size() - 1);
				}
				for (size_t m = j + 1; m < tabs[i].pages.size(); m++)
				{
					int rowID = 0;
					int rowRunningHeight = 0;
					for (size_t k = 0; k < tabs[i].pages[j].rows.size(); k++)
					{
						rowRunningHeight += tabs[i].pages[j].rows[k].height;
						if (tabs[i].pages[j].maxHeight < rowRunningHeight)
						{
							rowID = k;
							break;
						}
					}
					if (tabs[i].pages[m].maxHeight - tabs[i].pages[m].height > tabs[i].pages[j].rows[rowID].height)
					{
						tabs[i].pages[m].rows.push_back(tabs[i].pages[j].rows[rowID]);
						tabs[i].pages[j].rows.erase(tabs[i].pages[j].rows.begin() + rowID);

						UpdatePageDimensions(i, j);
						UpdatePageDimensions(i, m);

						break;
					}
					if (m == tabs[i].pages.size() - 1)
					{
						tabs[i].pages.push_back({});
						UpdatePageDimensions(i, tabs[i].pages.size() - 1);
					}
				}
			}
		}
	}
}

void Box::UpdateContainers()
{

	UpdateAllPageDimensions();

	UpdateWidgetRows();

	if (lockHeight == false)
	{
		height = static_cast<int>(tabs[currentTab].pages[currentPage].height + 2 * defaultSpacing.y);
		if (height % 32 == 0) height += 32;
	}

	UpdateAllPageDimensions();

	UpdateRowPages();

	//Update Page Dimensions to largest Row

	for (size_t i = 0; i < tabs.size(); i++)
	{
		for (size_t j = 0; j < tabs[i].pages.size(); j++)
		{
			tabs[i].pages[j].width = 0;
			tabs[i].pages[j].height = 0;
			for (size_t k = 0; k < tabs[i].pages[j].rows.size(); k++)
			{
				if (tabs[i].pages[j].rows[k].width > tabs[i].pages[j].width)
				{
					tabs[i].pages[j].width = tabs[i].pages[j].rows[k].width;
				}
				tabs[i].pages[j].height += tabs[i].pages[j].rows[k].height;
			}
		}
	}

}

void Box::UpdateSpacing()
{
	for (size_t i = 0; i < tabs.size(); i++)
	{
		for (size_t j = 0; j < tabs[i].pages.size(); j++)
		{
			int totalRowH = 0;
			for (size_t k = 0; k < tabs[i].pages[j].rows.size(); k++)
			{
				totalRowH += tabs[i].pages[j].rows[k].height;
			}
			tabs[i].pages[j].spacing = (tabs[i].pages[j].maxHeight - totalRowH) / (tabs[i].pages[j].rows.size() + 1);

			float rowPosition = defaultSpacing.y;
			for (size_t k = 0; k < tabs[i].pages[j].rows.size(); k++) 
			{
				tabs[i].pages[j].rows[k].position = rowPosition;
				int totalWidgetW = 0;

				for (size_t l = 0; l < tabs[i].pages[j].rows[k].widgets.size(); l++)
				{
					totalWidgetW += tabs[i].pages[j].rows[k].widgets[l]->GetWidth();
				}
				tabs[i].pages[j].rows[k].spacing = (tabs[i].pages[j].maxWidth - totalWidgetW) / (tabs[i].pages[j].rows[k].widgets.size() + 1);

				if (tabs[i].pages[j].rows[k].widgets.back()->GetType() == "Text")
				{
					while (tabs[i].pages[j].rows[k].widgets.back()->GetWidth() + tabs[i].pages[j].rows[k].spacing * 2 > tabs[i].pages[j].maxWidth)
					{
						tabs[i].pages[j].rows[k].widgets.back()->NormalizeFontSize(tabs[i].pages[j].maxWidth - tabs[i].pages[j].rows[k].spacing * 2);
					}
				}


				float widgetPosition = (this->width - this->tabs[i].pages[j].maxWidth)*0.5f;

				for (size_t l = 0; l < tabs[i].pages[j].rows[k].widgets.size(); l++)
				{
					widgetPosition += tabs[i].pages[j].rows[k].spacing;
					tabs[i].pages[j].rows[k].widgets[l]->UpdatePosition({ widgetPosition,tabs[i].pages[j].rows[k].position });
					widgetPosition += tabs[i].pages[j].rows[k].widgets[l]->GetWidth();
				}

				rowPosition += tabs[i].pages[j].rows[k].height;
			}
		}
	}
}

void Box::Update()
{
	UpdateContainers();

	if (lockWidth == false)
	{
		width = static_cast<int>(tabs[currentTab].pages[currentPage].width);
		if (width % 32 == 0) width += 32;
	}

	if (wrapText)
	{
		int const oldWrapLength = wrapLength;
		this->wrapLength = static_cast<int>(width - defaultSpacing.x * 2);
		if (wrapLength != oldWrapLength)
		{
			this->UpdateWrap(0,0,0,0, wrapLength);
			return;
		}
	}

	if (lockHeight == false) // Adjust earlier required overcompensation
	{
		height -= static_cast<int>(defaultSpacing.y);
	}

	UpdateTiles();

}

void Box::Draw()
{
	for (size_t i = 0; i < tiles.size(); i++)
	{
		tiles[i]->Draw();
	}

	for (size_t k = 0; k < tabs[currentTab].pages[currentPage].rows.size(); k++)
	{
		for (size_t l = 0; l < tabs[currentTab].pages[currentPage].rows[k].widgets.size(); l++)
		{
			tabs[currentTab].pages[currentPage].rows[k].widgets[l]->Draw();
		}
	}
}

void Box::Draw(double angle, SDL_RendererFlip flip)
{
	for (size_t i = 0; i < tiles.size(); i++)
	{
		tiles[i]->Draw(angle, flip);
	}
	for (size_t k = 0; k < tabs[currentTab].pages[currentPage].rows.size(); k++)
	{
		for (size_t l = 0; l < tabs[currentTab].pages[currentPage].rows[k].widgets.size(); l++)
		{
			tabs[currentTab].pages[currentPage].rows[k].widgets[l]->Draw(angle, flip);
		}
	}
}

void Box::DrawDebug()
{
	SDL_SetRenderDrawColor(renderer, 0, 127, 255, 255);
	SDL_Rect boxRect = { static_cast<int>(position.x),static_cast<int>(position.y),width,height };
	SDL_RenderDrawRect(renderer, &boxRect);
	SDL_RenderDrawLine(renderer, static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.x + width), static_cast<int>(position.y + height));

	for (size_t k = 0; k < tabs[currentTab].pages[currentPage].rows.size(); k++)
	{
		for (size_t l = 0; l < tabs[currentTab].pages[currentPage].rows[k].widgets.size(); l++)
		{
			tabs[currentTab].pages[currentPage].rows[k].widgets[l]->DrawDebug();
		}
	}

}