#include "AudioManager.h"
#include <iostream>

std::unordered_map<std::string, int> AudioManager::sfx_channels = {};

AudioManager::~AudioManager()
{
	clear();
}

int AudioManager::init()
{
	// init only for .wav files
	// change the flag to use other file types
	if (Mix_Init(0) < 0)
	{
		std::cout << "Couldn't iniitialize SDL_Mixer: " << Mix_GetError() << "\n";
		return -1;
	}

	// channel finish callback
	Mix_ChannelFinished([](int channel) {
		std::cout << "channel " << channel << " finished\n";

		// remove from sfx_channels
		for (auto it = AudioManager::sfx_channels.begin(); it != AudioManager::sfx_channels.end();)
		{
			if (it->second == channel)
			{
				it = AudioManager::sfx_channels.erase(it);
			}
			else
			{
				++it;
			}
		}
	});

	// allocate 24 channels instead of default 8 channels
	Mix_AllocateChannels(24);

	// audio channel is open
	Mix_OpenAudio(frequency, format, channels, chunk_size);

	std::cout << "Audio initialized\n";
	return 0;
}

int AudioManager::addMusic(std::string id, std::string file_name)
{
	Mix_Music* music = Mix_LoadMUS(file_name.c_str());
	
	if (!music)
	{
		std::cout << "Couldn't load music file: " << file_name << " " << Mix_GetError() << "\n";
		return -1;
	}

	music_sounds.emplace(id, std::move(music));
	return 0;
}

int AudioManager::addSfx(std::string id, std::string file_name)
{
	Mix_Chunk* sfx = Mix_LoadWAV(file_name.c_str());

	if (!sfx)
	{
		std::cout << "Couldn't load sfx file: " << file_name << " " << Mix_GetError() << "\n";
		return -1;
	}

	sfx_sounds.emplace(id, std::move(sfx));
	return 0;
}

void AudioManager::playMusic(std::string id, int loops)
{
	/* set loops parameter to -1 for infinite loop, 0 for once */
	Mix_PlayMusic(music_sounds[id], loops);
}

/// <summary>
/// 0 = fade out previous playback
/// 1 = halt previous playback
/// </summary>
/// <param name="id"></param>
/// <param name="loops"></param>
/// <param name="halt_or_fade_out_prev_playback"</param>
void AudioManager::playSfx(std::string id, int loops, size_t halt_or_fade_out_prev_playback)
{
	auto it = sfx_channels.find(id);
	if (it != sfx_channels.end())
	{
		if (halt_or_fade_out_prev_playback == 0)
		{
			Mix_FadeOutChannel(sfx_channels[id], 100);
		}
		else if (halt_or_fade_out_prev_playback == 1)
		{
			Mix_HaltChannel(sfx_channels[id]);
		}
	}

	/* can specify the playback channel by
	altering the channel number. can use -1
	if no specific channel*/
	int channel = Mix_PlayChannel(-1, sfx_sounds[id], loops);
	if (channel == -1)
	{
		std::cout << "couldn't play sfx: " << id << " " << Mix_GetError() << "\n";
		return;
	}

	sfx_channels[id] = channel;

	if(id._Equal("enemy-sword"))
		std::cout << "enemy-sword sfx channel: " << channel << "\n";
}

void AudioManager::playLoopingSfx(std::string id)
{
	if (isSfxPlaying(id)) return;
	int channel = Mix_PlayChannel(-1, sfx_sounds[id], -1);
	sfx_channels[id] = channel;

	if (id._Equal("enemy-step-1"))
		std::cout << "enemy-step sfx channel: " << channel << "\n";
}

void AudioManager::stopLoopingSfx(std::string id)
{
	auto it = sfx_channels.find(id);
	if (it == sfx_channels.end()) return;

	Mix_HaltChannel(it->second);
}

int AudioManager::setVolumeChannel(int channel, int new_volume)
{
	if (Mix_Volume(channel, new_volume) < 0)
	{
		std::cout << Mix_GetError() << "\n";
		return -1;
	}

	return 0;
} 

int AudioManager::setVolumeMusic(int new_volume)
{
	if (Mix_VolumeMusic(new_volume) < 0)
	{
		std::cout << Mix_GetError() << "\n";
		return -1;
	}

	music_volume = new_volume;
	return 0;
}

int AudioManager::getVolumeMusic()
{
	return music_volume;
}

int AudioManager::setVolumeSfx(std::string sfx_id, int new_volume)
{
	if (Mix_VolumeChunk(sfx_sounds[sfx_id], new_volume) < 0)
	{
		std::cout << sfx_id << " " << Mix_GetError() << "\n";
		return -1;
	}
	sfx_volume = new_volume;
	return 0;
}

int AudioManager::getVolumeSfx()
{
	return sfx_volume;
}

bool isMusicPlaying()
{
	return Mix_PlayingMusic() != 0;
}

bool isMusicPaused()
{
	return Mix_PausedMusic() != 0;
}

bool AudioManager::isSfxPlaying(std::string sfx_id)
{
	auto it = sfx_channels.find(sfx_id);
	if (it == sfx_channels.end()) return false;
	return Mix_Playing(it->second);
}

void AudioManager::clear()
{
	for (auto& m : music_sounds)
	{
		Mix_FreeMusic(m.second);
		m.second = nullptr;
	}
	music_sounds.clear();
	
	for (auto& s : sfx_sounds)
	{
		Mix_FreeChunk(s.second);
		s.second = nullptr;
	}
	sfx_sounds.clear();
}