#include "SceneManager.h"
#include "Game.h"
#include "Input.h"

void SceneManager::registerScene(const std::string& title, std::function<std::unique_ptr<IScene>()> factory)
{
	factories[title] = std::move(factory);
}

void SceneManager::requestSceneChange(const std::string& title)
{
	pending_scene = title;
}

void SceneManager::applyPendingChange()
{
	if (pending_scene.empty()) return;

	if (active_scene)
	{
		Game::getInstance().getEventBus().clear();
		active_scene->unload();
		active_scene.reset();
	}

	auto it = factories.find(pending_scene);
	if (it == factories.end())
	{
		pending_scene.clear();
		return;
	}

	active_scene = it->second();
	active_scene->init();
	
	pending_scene.clear();
}

IScene& SceneManager::getActiveScene() const
{
	return *active_scene.get();
}

void SceneManager::handleEvent(const SDL_Event& event)
{
	if (active_scene)
		active_scene->entity_manager->handleEvents(event);
}

void SceneManager::init()
{
	if (active_scene)
		active_scene->init();
}

void SceneManager::update(float delta_time)
{
	if (active_scene)
		active_scene->update(delta_time);

	applyPendingChange();
}

void SceneManager::updateWhilePause(float delta_time)
{
	if (active_scene)
		active_scene->updateWhilePause(delta_time);

	applyPendingChange();
}

void SceneManager::draw()
{
	if (active_scene)
		active_scene->draw();
}