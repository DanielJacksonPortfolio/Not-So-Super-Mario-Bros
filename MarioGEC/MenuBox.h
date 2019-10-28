#pragma once

#include "Box.h"

class MenuBox :public Box
{
private:
	std::vector<Image*> buttonImages = {
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
		new Image({ 64, 16, 16, 16 }), // 6
		new Image({ 80, 16, 16, 16 }), // 7
		new Image({ 0, 16, 16, 16 }),// 8
		new Image({ 16, 16, 16, 16 }),// 9
		new Image({ 64, 32, 16, 16 }),// 10
		new Image({ 80, 32, 16, 16 }),// 11
		new Image({ 0, 32, 16, 16 }),// 12
		new Image({ 16, 32, 16, 16 }),// 13
		new Image({ 32, 16, 16, 16 }),// 14
		new Image({ 32, 32, 16, 16 }),// 15
		new Image({ 48, 16, 16, 16 }),// 16
		new Image({ 48, 32, 16, 16 }),// 17
		new Image({ 96, 16, 16, 16 }),// 18
		new Image({ 112, 16, 16, 16 }),// 19
		new Image({ 96, 32, 16, 16 }),// 20
		new Image({ 112, 32, 16, 16 }),// 21
		new Image({ 128, 16, 16, 16 }),// 22
		new Image({ 144, 16, 16, 16 }),// 23
		new Image({ 128, 32, 16, 16 }),// 24
		new Image({ 144, 32, 16, 16 })// 25
	};


public:
	MenuBox(Coordf position, SDL_Renderer * renderer, int width, int height, bool lockWidth, bool lockHeight, Coordf widgetSpacing);
	~MenuBox();
	void virtual Update() override;
	void virtual AddText(Coordf position, std::string text, std::string font, int fontSize, int color, int color2, int renderMode, int wrapLength) override;
	void virtual AddButton(Coordf position, int imageID, int width, int height, int offset, std::string font, int fontSize, int color, int color2, int direction) override;
	void AutoScale();
	void DrawDebug();
	void virtual LoadImages() override;
};

