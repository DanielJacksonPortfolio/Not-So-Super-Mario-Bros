#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <iostream>

#include "Constants.h"

class SoundObject
{
private:
	int type = 0;
	bool loop = false;
	bool pause = false;
	Mix_Music* music = nullptr;
	Mix_Chunk* effect = nullptr;
	int channel = 0;
	std::string id = "";
public:
	SoundObject(std::string filePath, int type, int channel,std::string id, bool loop = false);
	~SoundObject();

	void Play(bool fade = false);
	void Pause();
	void Resume();
	void Stop(bool fade = false);
	void ToggleLooping() {loop = !loop;}
	void LoadFile(std::string filePath);

	bool const GetLooping() { return loop; }
	bool const GetPaused() { return pause; }
			   
	int const GetType() {return type;}

};

