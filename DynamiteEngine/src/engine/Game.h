#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include <map>
#include "SceneManager.h"
#include <string>
#include "AssetManager.h"
#include "EntityManager.h"
#include "HttpRequestManager.h"
#include "FileSystemManager.h"

// singleton
class Game {
public:
	static Game& getInstance()
	{
		static Game instance;
		return instance;
	}

	// to prevent duplication
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

	SDL_Renderer* getRendererPtr();
	SDL_Event& getSDLEvent();
	SceneManager& getSceneManager();
	EventBus& getEventBus();
	HttpRequestManager& getHttpRequestManager();
	FileSystemManager& getFileSystemManager();

	bool isRunning();
	void setRunning(bool running);

	bool isPaused();
	void setPaused(bool paused);

	void init(const char* title, int x, int y, int w, int h, bool full_scr);
	void handleEvents();
	void update(float delta_time);
	void updateWhilePause(float delta_time);
	void render();
	void clean();
	void shutdown();

private:
	Game() = default;
	~Game() = default;
	
	bool is_running = false;
	bool is_paused = false;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event;
	SceneManager scene_manager;
	EventBus event_bus;
	HttpRequestManager http_request_manager;
	FileSystemManager file_system_manager;
};

#endif