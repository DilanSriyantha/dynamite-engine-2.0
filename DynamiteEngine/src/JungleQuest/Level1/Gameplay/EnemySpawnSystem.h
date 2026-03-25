#ifndef ENEMY_SPAWN_SYSTEM_H
#define ENEMY_SPAWN_SYSTEM_H

#include "IScript.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "Components.h"
#include "Player.h"
#include "Enemy.h"
#include "JungleQuest/Level1/UI/EnemyHealthBar.h"
#include "EventTypes.h"

class EnemySpawnSystem : public IScript {
public:
	EnemySpawnSystem(AssetManager& asset_manager, AudioManager& audio_manager, EntityManager& entity_manager, Player& player) :
		asset_manager(asset_manager), audio_manager(audio_manager), entity_manager(entity_manager), player(player) {}

	void update(float delta_time)
	{
		updateSpawnTime(delta_time);
		spawnEnemy();

		//std::cout << "onTakeDamageEventListeners size: " << Game::getInstance().getEventBus().getSize<EnemyTakeDamageEvent>() << "\n";
	}

private:
	AssetManager& asset_manager;
	AudioManager& audio_manager;
	EntityManager& entity_manager;
	Player& player;

	float spawn_cooldown = 7.f;
	float spawn_timer = 0.f;

	bool is_enemy_in_action = false;

	void updateSpawnTime(float delta_time)
	{
		if (is_enemy_in_action) return;

		if (spawn_timer > 0.f)
			spawn_timer -= delta_time;
	}

	void spawnEnemy()
	{
		bool enemy_can_spawn = player.isMovingRight() && spawn_timer <= 0.f && !is_enemy_in_action;

		//std::cout << "enemy spawn timer: " << spawn_timer << "\n";

		if (is_enemy_in_action && enemy && enemy->is_died)
		{
			spawn_timer = spawn_cooldown;
			is_enemy_in_action = false;
		}

		if (!enemy_can_spawn) return;

		std::cout << "enemy can spawn\n";

		auto& enemy_container(entity_manager.addEntity());
		enemy = &enemy_container.addComponent<Enemy>(asset_manager, audio_manager, entity_manager, player);
		enemy_container.addComponent<EnemyHealthBar>(asset_manager, entity_manager);

		std::cout << "enemy spawned\n";

		is_enemy_in_action = true;
	}

private:
	Enemy* enemy = nullptr;
};

#endif