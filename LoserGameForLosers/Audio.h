#pragma once
#include <SDL_mixer.h>

class Audio
{
public:
	virtual ~Audio() = default;
	virtual void play_sound(int sound_id) = 0;
	virtual void play_music(int music_id, int loops) = 0;
	virtual void stop_music() = 0;
};