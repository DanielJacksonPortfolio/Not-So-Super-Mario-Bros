#include "SoundObject.h"



SoundObject::SoundObject(std::string filePath, int type, int channel,std::string id,bool loop)
{
	this->type = type;
	this->loop = loop;
	this->channel = channel;
	this->id = id;
	LoadFile(filePath);
}


SoundObject::~SoundObject()
{
	Mix_FreeChunk(effect);
	Mix_FreeMusic(music);
}

void SoundObject::Play(bool fade)
{
	std::cout << "Play " + id << std::endl;
	if (this->type == MUSIC)
	{
		if (music != nullptr)
		{
			if (loop)
				if (fade)
					Mix_FadeInMusic(music,-1,2000);
				else
					Mix_PlayMusic(music, -1);
			else
				if (fade)
					Mix_FadeInMusic(music,1, 2000);
				else
					Mix_PlayMusic(music, 1);
		}

		else
			std::cout << "No Music Loaded" << std::endl;
	}
	else
	{
		if (effect != nullptr)
		{
			if (loop)
				Mix_PlayChannel(channel, effect, -1);
			else
				Mix_PlayChannel(channel, effect, 0);
		}
		else
			std::cout << "No Effect Loaded" << std::endl;
	}
} 

void SoundObject::Pause()
{
	std::cout << "Pause " + id << std::endl;
	if (!pause)
	{
		Mix_PauseMusic();
		pause = true;
	}
}

void SoundObject::Resume()
{
	std::cout << "Resume " + id << std::endl;
	if (pause)
	{
		Mix_ResumeMusic();
		pause = false;
	}
}

void SoundObject::Stop(bool fade)
{
	std::cout << "Stop " + id << std::endl;
	if (fade)
	{
		while (!Mix_FadeOutMusic(1000) && Mix_PlayingMusic())
		{
			// wait for any fades to complete
			SDL_Delay(100);
		}
	}
	else
	{
		if (type == MUSIC)
			Mix_HaltMusic();
		else
			Mix_HaltChannel(channel);
	}
}

void SoundObject::LoadFile(std::string filePath)
{
	std::cout << "Load " + id << std::endl;
	if (this->type == MUSIC)
	{
		music = Mix_LoadMUS(filePath.c_str());
		//effect = nullptr;
	}
	else
	{
		effect = Mix_LoadWAV(filePath.c_str());
		//music = nullptr;
	}
}