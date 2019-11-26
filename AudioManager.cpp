#include "AudioManager.h"



WAV_Chunk AudioManager::GetChunk()
{
	return m_Chunk;
}

void AudioManager::SetChunk(WAV_Chunk chunk)
{
	this->m_Chunk = chunk;
}

void AudioManager::Play()
{
	Mix_PlayChannel(channelNumber, channelName, looping);
	Mix_Volume(channelNumber, volume);
}

void AudioManager::Hault()
{
	Mix_HaltChannel(channelNumber);
}

AudioManager::AudioManager(const char* file, int channelNumber, int looping, float volume)
	: channelNumber(channelNumber), looping(looping)
{
	this->volume = MIX_MAX_VOLUME * (volume / 100);
	channelName = Mix_LoadWAV(file);
	Mix_Volume(channelNumber, volume);
}


AudioManager::~AudioManager()
{
}
