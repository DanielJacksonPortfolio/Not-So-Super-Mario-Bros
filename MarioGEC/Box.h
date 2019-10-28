#pragma once
#include <SDL.h>
#include "Image.h"
#include "Constants.h"
#include "Text.h" 
#include "Button.h" 
#include <string>
#include <vector>
#include "Widget.h"
#include <functional>

class Box
{
private:
	SDL_Renderer * renderer;


	Coordf defaultSpacing = Coordf(32, 32);
	Coordf oldOffset = defaultSpacing;
	bool wrapText = false;
	int wrapLength = 16;
	std::vector<Image*> tileImages =
	{
		new Image({ 0, 0, 32, 32 }),
		new Image({ 32, 0, 32, 32 }),
		new Image({ 64, 0, 32, 32 }),
		new Image({ 0, 32, 32, 32 }),
		new Image({ 32, 32, 32, 32 }),
		new Image({ 64, 32, 32, 32 }),
		new Image({ 0, 64, 32, 32 }),
		new Image({ 32, 64, 32, 32 }),
		new Image({ 64, 64, 32, 32 }),
	};

	std::vector<Image*> buttonImages = {
		new Image({ 0, 0, 16, 16 }),
		new Image({ 16, 64, 16, 16 }),
		new Image({ 0, 0, 16, 16 }),
		new Image({ 112, 0, 16, 16 }),
		new Image({ 16, 64, 16, 16 }),
		new Image({ 16, 0, 16, 16 }),
		new Image({ 48, 0, 16, 16 }),
		new Image({ 80, 0, 16, 16 }),
		new Image({ 96, 0, 16, 16 }),
		new Image({ 0, 48, 16, 16 }),
		new Image({ 16, 48, 16, 16 }),
		new Image({ 32, 48, 16, 16 }),
		new Image({ 48, 48, 16, 16 }),
		new Image({ 64, 48, 16, 16 }),
		new Image({ 80, 48, 16, 16 }),
		new Image({ 0, 112, 16, 16 }),
		new Image({ 16, 112, 16, 16 }),
		new Image({ 32, 112, 16, 16 }),
		new Image({ 48, 112, 16, 16 }),
		new Image({ 64, 112, 16, 16 }),
		new Image({ 0, 80, 16, 16 }),
		new Image({ 16, 80, 16, 16 }),
		new Image({ 32, 80, 16, 16 }),
		new Image({ 48, 80, 16, 16 }),
		new Image({ 64, 80, 16, 16 }),
		new Image({ 80, 80, 16, 16 }),
		new Image({ 0, 96, 16, 16 }),
		new Image({ 16, 96, 16, 16 }),
		new Image({ 32, 96, 16, 16 }),
		new Image({ 64, 16, 16, 16 }),
		new Image({ 80, 16, 16, 16 }),
		new Image({ 0, 16, 16, 16 }),
		new Image({ 16, 16, 16, 16 }),
		new Image({ 64, 32, 16, 16 }),
		new Image({ 80, 32, 16, 16 }),
		new Image({ 0, 32, 16, 16 }),
		new Image({ 16, 32, 16, 16 }),
		new Image({ 32, 16, 16, 16 }),
		new Image({ 32, 32, 16, 16 }),
		new Image({ 48, 16, 16, 16 }),
		new Image({ 48, 32, 16, 16 }),
		new Image({ 96, 16, 16, 16 }),
		new Image({ 112, 16, 16, 16 }),
		new Image({ 96, 32, 16, 16 }),
		new Image({ 112, 32, 16, 16 }),
		new Image({ 128, 16, 16, 16 }),
		new Image({ 144, 16, 16, 16 }),
		new Image({ 128, 32, 16, 16 }),
		new Image({ 144, 32, 16, 16 }),
	};

	Coordf position = Coordf(0, 0);
	int width = 0, height = 0;

	std::vector<Image*> tiles = {};

	SDL_Rect clipRect = { 0,0,0,0 };

	bool lockWidth = true;
	bool lockHeight = true;

	struct Tab;
	struct Page;
	struct Row;

	struct Tab
	{
		std::vector<Page> pages = {};
	};

	struct Page
	{
		int width = 0;
		int height = 0;
		int maxWidth = 0;
		int maxHeight = 0;
		int spacing = 0;
		std::vector<Row> rows = {};
	};

	struct Row
	{
		int width = 0;
		int height = 0;
		int spacing = 0;
		float position = 0;
		std::vector<Widget*> widgets = {};
	};


public:
	SDL_Rect hitbox = { 0,0,0,0 };
	std::vector<Tab> tabs = {};
	int currentTab = 0;
	int currentPage = 0;

	Box(Coordf position, SDL_Renderer * renderer, int width, int height, bool lockWidth, bool lockHeight, Coordf defaultSpacing);
	~Box();

	void LoadImages();

	void AddText(std::string text, std::string font, int fontSize, int color, int color2, int renderMode,bool wrapText, int wrapLength, int tab, int page, int row);
	void AddButton(int imageID, int width, int height, int offset, std::string font, int fontSize, int color, int color2, int direction, int tab, int page, int row, std::function<void()>function);

	void Update();
	void UpdateTiles();
	void UpdateText(int tab, int page, int row, int widgetID, std::string text);
	void UpdateFont(int tab, int page, int row, int widgetID, std::string path);
	void UpdateColor(int tab, int page, int row, int widgetID, int color, int colorNum);
	void UpdateSize(int tab, int page, int row, int widgetID, int ptSize);
	void UpdateTextPosition(int tab, int page, int row, int widgetID);
	void UpdatePosition(int tab, int page, int row, int widgetID, Coordf origin);
	void UpdateRenderMode(int tab, int page, int row, int widgetID, int renderMode);
	void UpdateWrap(int tab, int page, int row, int widgetID, int wrapLength);
	void UpdateHitbox() { this->hitbox = { static_cast<int>(position.x), static_cast<int>(position.y),width,height }; }
	void UpdateRowDimensions(int tabID, int pageID, int rowID);
	void UpdatePageDimensions(int tabID, int pageID);
	void UpdateContainers();
	void UpdateSpacing();
	void UpdateAllPageDimensions();
	void UpdateWidgetRows();
	void UpdateRowPages();
	void Draw();
	void Draw(double angle, SDL_RendererFlip flip);
	void DrawDebug();
};