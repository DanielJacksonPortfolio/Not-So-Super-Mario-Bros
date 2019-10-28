#pragma once
#include "SDL.h"
#include "Constants.h"

class Camera
{
private:
	int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;
public:
	Camera();
	~Camera();
	Coordf worldPosition = Coordf(0, 0 );
	float zoom = 4.0f;
};

