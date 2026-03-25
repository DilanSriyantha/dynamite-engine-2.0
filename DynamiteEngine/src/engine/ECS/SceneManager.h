#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <map>
#include <string>
#include <functional>
#include "IScene.h"

class SceneManager {
public:
	EventBus event_bus;

	SceneManager() = default;
	
	template<typename T, typename... Args>
	T& createScene(const std::string& title, Args&&... args)
	{
		auto scene = std::make_unique<T>(std::forward<Args>(args)...);
		T& ref = *scene;
		factories[title] = std::move(scene);

		return ref;
	}

	void registerScene(const std::string& title, std::function <std::unique_ptr<IScene>()> factory);
	void requestSceneChange(const std::string& title);
	void applyPendingChange();

	void addScene(const std::string& scene_title, IScene* scene);
	void removeScene(const std::string& scene_title);
	
	void setActiveScene(const std::string& scene_title);
	IScene& getActiveScene() const;

	void handleEvent(const SDL_Event& event);
	void init();
	void update(float delta_time);
	void updateWhilePause(float delta_time);
	void draw();

private:
	std::map<std::string, std::function<std::unique_ptr<IScene>()>> factories;
	std::unique_ptr<IScene> active_scene;
	std::string pending_scene;
};

#endif