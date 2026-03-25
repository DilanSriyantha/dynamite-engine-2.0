#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "Game.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class GameLoop {
public:
	GameLoop() = default;
	~GameLoop();

	int init();
};

#endif