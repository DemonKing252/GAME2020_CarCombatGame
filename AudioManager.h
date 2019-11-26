#pragma once
#ifndef __AUDIOMANAGER__
#define __AUDIOMANAGER__
#include "SDL_mixer.h"
// const char* fileName, int channelNumber, int looping, int volumePercentage
enum WAV_Chunk { TITLE_MUSIC, GAME_MUSIC, FIRE, EXPLOSION, CAR_HORN, CAR_IDLE, CAR_STANDARD_DAMAGE, WIN_STATE };
class AudioManager
{
private:
	WAV_Chunk m_Chunk;
public:
	int looping, channelNumber;
	float volume;
	WAV_Chunk GetChunk();
	void SetChunk(WAV_Chunk);

	Mix_Chunk* channelName;
	
	void Play();
	void Hault();
	AudioManager(const char*, int, int, float);
	~AudioManager();
};
#endif //!__AUDIOMANAGER__
typedef AudioManager Audio;