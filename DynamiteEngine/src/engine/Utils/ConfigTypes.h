#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include "Globals.h"

#define PLAYER_CONFIG_FILE getExecutableDir() + "\\" + "player-configuration.bin"
#define AUTH_CACHE_FILE getExecutableDir() + "\\" + "user.bin"
#define SCORE_MAP_FILE getExecutableDir() + "\\" + "score-map.bin"

struct PlayerConfiguration {
	char name[255];
	size_t high_score;
};

#endif