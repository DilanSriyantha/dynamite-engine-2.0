#ifndef SCROLL_BACKGROUND_H
#define SCROLL_BACKGROUND_H

#include "IScript.h"
#include "Components.h"
#include "UIElements.h"

class ScrollBackground : public IScript {
public:
	ScrollBackground(AssetManager& asset_manager, EntityManager& entity_manager)
		: asset_manager(asset_manager), entity_manager(entity_manager) {}

	void init() override
	{
		asset_manager.addTexture("menu-bg", "Assets/game/ui/menu/bg_layer_1.png");
		asset_manager.addTexture("clouds", "Assets/game/ui/menu/clouds.png");

		auto& bg = entity->addComponent<ImagePanel>();

		bg.setSize(1920, 1080);
		bg.setScale(1.f);
		bg.setTexture(asset_manager.getTexture("menu-bg"));
		bg.setGroup(GRP_UI_ELEMENTS_LAYER_3);

		createScrollLayers();
	}

	void update(float delta_time) override
	{
		updateScrollLayers();
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;

	std::vector<Entity*> layers;

	void createScrollLayers()
	{
		const int LAYER_WIDTH = 1920;
		const int LAYER_HEIGHT = 1080;

		auto& layer_a = entity_manager.addEntity();
		auto& layer_b = entity_manager.addEntity();

		layer_a.addComponent<Transform>(0, 0, LAYER_WIDTH, LAYER_HEIGHT, 1.f);
		layer_a.addComponent<Sprite>(asset_manager.getTexture("clouds"));
		layer_a.addGroup(GRP_MAP_LAYER_6);

		layer_b.addComponent<Transform>(LAYER_WIDTH, 0, LAYER_WIDTH, LAYER_HEIGHT, 1.f);
		layer_b.addComponent<Sprite>(asset_manager.getTexture("clouds"));
		layer_b.addGroup(GRP_MAP_LAYER_6);

		layers.push_back(&layer_a);
		layers.push_back(&layer_b);
	}

	void updateScrollLayers()
	{
		const int LAYER_WIDTH = 1920;

		if (layers.size() < 2) return;

		auto& a = layers[0]->getComponent<Transform>();
		auto& b = layers[1]->getComponent<Transform>();

		a.setVelocity({ -1.f, 0.f });
		b.setVelocity({ -1.f, 0.f });

		if (a.getPositionX() + LAYER_WIDTH < 0)
			a.setPositionX(b.getPositionX() + LAYER_WIDTH);
		else if (b.getPositionX() + LAYER_WIDTH < 0)
			b.setPositionX(a.getPositionX() + LAYER_WIDTH);
	}
};

#endif