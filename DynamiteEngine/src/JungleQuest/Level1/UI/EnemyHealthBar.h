#ifndef ENEMY_HEALTH_BAR_H
#define ENEMY_HEALTH_BAR_H

#include "IScript.h"
#include "Components.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "UIElements.h"
#include "EventTypes.h"

class EnemyHealthBar : public IScript {
public:
	EnemyHealthBar(AssetManager& asset_manager, EntityManager& entity_manager)
		: asset_manager(asset_manager), entity_manager(entity_manager) {}

	~EnemyHealthBar()
	{
		Game::getInstance().getEventBus().unsubscribe(onTakeDamageListener);

		std::cout << "enemy health bar destroyed\n";
	}

	void init() override
	{
		enemy_transform = &entity->getComponent<Transform>();

		if (!enemy_transform) return;

		bar = &entity_manager.addEntity().addComponent<ImagePanel>();
		bar->setSize(574, 65);
		bar->setScale(SCALE);
		bar->setTexture(asset_manager.getTexture("bar-bg"));

		fill = &entity_manager.addEntity().addComponent<ImagePanel>();
		fill->setSize(FILL_WIDTH, 45);
		fill->setScale(SCALE);
		fill->setTexture(asset_manager.getTexture("bar-fill"));

		bar->setGroup(GRP_PLAYERS);
		fill->setGroup(GRP_OBJECTS);

		auto onEnemyTakeDamage = [&](const EnemyTakeDamageEvent& e)
		{
			if (!fill) return;

			auto& fill_transform = fill->entity->getComponent<Transform>();

			float damage_val = (e.damage_percentage / 100.f) * FILL_WIDTH;
			float new_fill = fill_transform.getData().width - damage_val;

			fill_transform.setWidth(new_fill);
			fill_transform.entity->getComponent<Sprite>().setTransformData(fill_transform.getData());
		};

		onTakeDamageListener = Game::getInstance().getEventBus().subscribe<EnemyTakeDamageEvent>(onEnemyTakeDamage);
	}

	void update(float delta_time) override
	{
		if (bar)
		{
			bar->setPosition(enemy_transform->getPosition());
			bar->dockOnTopCenter(*enemy_transform);
		}

		if (fill)
		{
			fill->setPosition(enemy_transform->getPosition());
			fill->dockOnTopLeft(*bar);
			fill->paddingTop(2.f);
		}
	}

	void destroy()
	{
		if (bar)
		{
			bar->entity->destroy();
			bar = nullptr;
		}

		if (fill)
		{
			fill->entity->destroy();
			fill = nullptr;
		}
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;

	Transform* enemy_transform = nullptr;
	ImagePanel* bar = nullptr;
	ImagePanel* fill = nullptr;

	const int FILL_WIDTH = 552;
	const float SCALE = .3f;

	SubscriptionHandle onTakeDamageListener;
};

#endif