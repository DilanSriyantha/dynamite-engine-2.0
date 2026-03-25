#ifndef ISCENE_H
#define ISCENE_H

#include <vector>
#include "SDL.h"
#include "Entity.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "EventBus.h"
#include "EntityManager.h"
#include "Camera.h"
#include "Globals.h"

class IScene {
public:
	std::unique_ptr<EntityManager> entity_manager;
	std::unique_ptr<AssetManager> asset_manager;
	std::unique_ptr<AudioManager> audio_manager;

	IScene() = default;

	virtual ~IScene()
	{
		if (entity_manager)
			entity_manager->clear();

		if (asset_manager)
			asset_manager->clear();

		if (audio_manager)
			audio_manager->clear();

		entity_manager.release();
		audio_manager.release();
		asset_manager.release();

		cameras.clear();
		active_camera = nullptr;
	}

	virtual void init()
	{
		entity_manager = std::make_unique<EntityManager>();
		asset_manager = std::make_unique<AssetManager>();
		audio_manager = std::make_unique<AudioManager>();

		audio_manager->init();

		cameras.emplace_back(Camera(SCREEN_WIDTH, SCREEN_HEIGHT));
		active_camera = &cameras[0];
	}

	virtual void update(float delta_time) 
	{
		entity_manager->refresh();
		entity_manager->update(delta_time);

		if (active_camera) active_camera->update(delta_time);
	}

	virtual void updateWhilePause(float delta_time)
	{
		entity_manager->refresh();
		entity_manager->updateWhilePause(delta_time);
	}

	virtual void draw() 
	{
		entity_manager->draw(&active_camera->getViewport());
	}

	virtual void unload()
	{

	}

	void setActiveCamera(int index)
	{
		if (index >= 0 && index < cameras.size())
			active_camera = &cameras[index];
	}

	Camera& getActiveCamera() const
	{
		return *active_camera;
	}

	EntityManager& getEntityManager()
	{
		return *entity_manager;
	}

	AssetManager& getAssetManager()
	{
		return *asset_manager;
	}

	AudioManager& getAudioManager()
	{
		return *audio_manager;
	}

protected:
	std::vector<Camera> cameras;
	Camera* active_camera = nullptr;
};

#endif // !ISCENE_H