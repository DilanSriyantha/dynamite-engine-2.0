#ifndef HEALTH_BAR_H
#define HEALTH_BAR_H

#include "ICompositeSceneObject.h"
#include "UIElements.h"
#include "EventTypes.h"

class HealthBar : public IScript {
public:
	HealthBar(AssetManager& asset_manager, EntityManager& entity_manager)
		: asset_manager(asset_manager), entity_manager(entity_manager) {}

	void init() override
	{
		auto& container = entity_manager.addEntity().addComponent<Panel>().setVisible(false);
		container.setTransform(0, 0, 1920, 1080, 1.f);

		auto& bar = entity_manager.addEntity().addComponent<ImagePanel>();
		bar.setSize(574, 65);
		bar.setScale(1.f);
		bar.setTexture(asset_manager.getTexture("bar-bg"));
		bar.dockOnTopLeft(container);
		bar.paddingTop(10);
		bar.paddingLeft(10);

		auto& fill = entity_manager.addEntity().addComponent<ImagePanel>();
		fill.setSize(552, 45);
		fill.setScale(1.f);
		fill.setTexture(asset_manager.getTexture("bar-fill"));
		fill.dockOnTopLeft(bar);
		fill.paddingTop(8);
		fill.paddingLeft(10);

		auto onPlayerTakeDamage = [&](const PlayerTakeDamageEvent& e)
		{
			auto& fill_transform = fill.entity->getComponent<Transform>();
			
			float damage_val = (e.damage_percentage / 100.f) * FILL_WIDTH;
			float new_fill = fill_transform.getData().width - damage_val;

			fill_transform.setWidth(new_fill);
			fill_transform.entity->getComponent<Sprite>().setTransformData(fill_transform.getData());
		};

		auto onPlayerHealthReset = [&](const PlayerHealthResetEvent& e)
		{
			auto& fill_transform = fill.entity->getComponent<Transform>();
			fill_transform.setWidth(FILL_WIDTH);
			fill_transform.entity->getComponent<Sprite>().setTransformData(fill_transform.getData());
		};

		Game::getInstance().getEventBus().subscribe<PlayerTakeDamageEvent>(onPlayerTakeDamage);
		Game::getInstance().getEventBus().subscribe<PlayerHealthResetEvent>(onPlayerHealthReset);
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;

	const int FILL_WIDTH = 552;
};

#endif