#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "SDL.h"
#include "SDL_mixer.h"
#include <map>
#include <unordered_map>
#include <string>

class AudioManager {
public:
	static std::unordered_map<std::string, int> sfx_channels;

	AudioManager() = default;

	AudioManager(int freq, Uint16 format, int channels, int chunk_size) :
		frequency(freq), format(format), channels(channels), chunk_size(chunk_size) 
	{}

	~AudioManager();

	int init();
	int addMusic(std::string id, std::string file_name);
	int addSfx(std::string id, std::string file_name);

	void playMusic(std::string id, int loops);
	void playSfx(std::string id, int loops, size_t halt_or_fade_out_prev_playback = 0);
	void playLoopingSfx(std::string id);
	void stopLoopingSfx(std::string id);

	int setVolumeChannel(int channel, int new_volume);

	int setVolumeMusic(int new_volume);
	int getVolumeMusic();

	int setVolumeSfx(std::string sfx_id, int new_volume);
	int getVolumeSfx();

	bool isMusicPlaying();
	bool isMusicPaused();
	bool isSfxPlaying(std::string sfx_id);

	void clear();

private:
	int frequency = 44100;
	int channels = 2; /* setreo */
	int chunk_size = 1024; /* sample rate */ 
	int music_volume = MIX_MAX_VOLUME;
	int sfx_volume = MIX_MAX_VOLUME;
	Uint16 format = MIX_DEFAULT_FORMAT;
	std::map<std::string, Mix_Music*> music_sounds;
	std::map<std::string, Mix_Chunk*> sfx_sounds;
};

#endif