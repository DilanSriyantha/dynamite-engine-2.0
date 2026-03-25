#include "GameLoop.h"

GameLoop::~GameLoop()
{
	Game::getInstance().clean();
}

int GameLoop::init() 
{
	// cap framerate 
	const int fps = 60;
	const int frame_delay = 1000 / fps;

	Uint32 frame_start;
	Uint32 frame_time = 0;
	float delta_time = 0.f;

	Game::getInstance().init("DynamiteEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	while (Game::getInstance().isRunning())
	{
		// get frame start millis
		frame_start = SDL_GetTicks();

		Game::getInstance().handleEvents();

		if (Game::getInstance().isPaused())
			Game::getInstance().updateWhilePause(delta_time);
		else
			Game::getInstance().update(delta_time);

		Game::getInstance().render();

		// get frametime
		frame_time = SDL_GetTicks() - frame_start;

		// delay the frame and cap fps
		if (frame_delay > frame_time)
		{
			SDL_Delay(frame_delay - frame_time);
			frame_time = SDL_GetTicks() - frame_start;
		}

		// convert millis to sec
		delta_time = static_cast<float>(frame_time) / 1000.f;
	}

	return 0;
}