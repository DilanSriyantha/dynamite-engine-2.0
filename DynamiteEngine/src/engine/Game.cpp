#include "Game.h"
#include <iostream>
#include "Input.h"
#include "JungleQuest/Menu/Menu.h"
#include "JungleQuest/Level1/Level1.h"

SDL_Renderer* Game::getRendererPtr()
{
	return renderer;
}

SDL_Event& Game::getSDLEvent()
{
	return event;
}

SceneManager& Game::getSceneManager()
{
	return scene_manager;
}

EventBus& Game::getEventBus()
{
	return event_bus;
}

HttpRequestManager& Game::getHttpRequestManager()
{
	return http_request_manager;
}

FileSystemManager& Game::getFileSystemManager()
{
	return file_system_manager;
}

bool Game::isPaused()
{
	return is_paused;
}

void Game::setPaused(bool paused)
{
	is_paused = paused;
}

bool Game::isRunning()
{
	return is_running;
}

void Game::setRunning(bool running)
{
	is_running = running;
}

void Game::init(const char* title, int x, int y, int w, int h, bool full_scr)
{
	// handle full_scr flag
	int flags = 0;
	if (full_scr)
		flags = SDL_WINDOW_FULLSCREEN;

	// init sdl subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Count not initialize sdl subsystems\n";
		return;
	}

	std::cout << "sdl subsystems initialized...\n";

	// init window
	window = SDL_CreateWindow(title, x, y, w, h, flags);
	if (!window)
	{
		std::cout << "Could not create a window!\n";
		return;
	}
	std::cout << "window created\n";

	// init renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		std::cout << "Could not create the renderer!\n";
		return;
	}
	// draw color
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	std::cout << "renderer created\n";

	// init sdl ttf
	if (TTF_Init() != 0)
	{
		std::cout << "Could not initialize SDL2_TTF\nTTF_ERROR: " << TTF_GetError() << "\n";
	}

	// set running flag
	is_running = true;
	std::cout << "game is running\n";

	scene_manager.registerScene("menu", [](){
		return std::make_unique<Menu>();
	});

	scene_manager.registerScene("lvl1", []() {
		return std::make_unique<Level1>();
	});

	scene_manager.requestSceneChange("menu");
}

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	if (event.type == SDL_QUIT)
		is_running = false;

	scene_manager.handleEvent(event);
}

void Game::update(float delta_time)
{
	scene_manager.update(delta_time);
}

void Game::updateWhilePause(float delta_time)
{
	scene_manager.updateWhilePause(delta_time);
}

void Game::render()
{
	SDL_RenderClear(renderer);

	scene_manager.draw();

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::shutdown()
{
	clean();
	is_running = false;
}