#ifndef ENEMY_H
#define ENENY_H

#include "IScript.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "Components.h"
#include "Globals.h"
#include "Player.h"

class Enemy : public IScript {
public:
	bool is_died = false;

	Enemy(AssetManager& asset_manager, AudioManager& audio_manager, EntityManager& entity_manager, Player& player)
		: asset_manager(asset_manager), audio_manager(audio_manager), entity_manager(entity_manager), player(player) {}

	~Enemy()
	{
		Game::getInstance().getEventBus().unsubscribe(onTakeDamageListener);
		std::cout << "enemy destroyed\n";
	}

	void init() override
	{
		fire_puff = &entity_manager.addEntity();
		fire_puff->addComponent<Transform>(0, 0, 400, 400, 4.f);
		fire_puff->addComponent<Sprite>(asset_manager.getTexture("vfx-fire-puff"), true);
		fire_puff->getComponent<Sprite>().addAnimation("puff", 0, 15, .05f, false);
		fire_puff->getComponent<Sprite>().setVisible(false);

		auto spawn_x = SCREEN_WIDTH + player.entity->getComponent<Transform>().getPositionX() + 100;

		transform = &entity->addComponent<Transform>(spawn_x, 680, 200, 200, 2.f);

		sprite = &entity->addComponent<Sprite>(asset_manager.getTexture("enemy"), true);
		sprite->addAnimation("idle", 0, 17, .05f);
		sprite->addAnimation("run", 1, 11, .06f);
		sprite->addAnimation("attack", 2, 11, .06f, false);
		sprite->addAnimation("hurt", 3, 11, .06f, false);
		sprite->addAnimation("die", 4, 14, 0.05f, false);
		sprite->play("run");

		collider = &entity->addComponent<BoxCollider>(COL_ENEMY, true);
		//collider->showGizmo();

		entity->addGroup(GRP_PLAYERS);
		fire_puff->addGroup(GRP_OBJECTS);

		auto onTakeDamage = [&](const EnemyTakeDamageEvent& e)
		{
			takeDamage(e.damage_percentage);
		};

		onTakeDamageListener = Game::getInstance().getEventBus().subscribe<EnemyTakeDamageEvent>(onTakeDamage);
	}

	void update(float delta_time) override
	{
		checkDistanceToPlayer();
		moveTowardsPlayer();
		updateAttackTimer(delta_time);
		updateAnimation();
		playSounds();
		updateFirePuffPosition();
		updateDespawnTimer(delta_time);
		despawn();
	}

	void draw(SDL_Rect)
	{

	}

	void checkDistanceToPlayer()
	{
		if (is_died) return;

		auto player_transform = &player.entity->getComponent<Transform>();

		far_to_right_from_player = player_transform->getPositionX() + player_transform->getWidth() < transform->getPositionX() - 10.f;
		far_to_left_from_player = player_transform->getPositionX() - player_transform->getWidth() > transform->getPositionX() + 10.f;
	
		/*std::cout << "player x: " << player_transform->getPositionX() << ", enemy x: " << transform->getPositionX() << "\n";
		std::cout << "far to right from player: " << far_to_right_from_player << ", far to left from player: " << far_to_left_from_player << "\n";
		
		if (is_attacking)
			std::cout << "enemy is attacking\n";*/
	}

	void updateAttackTimer(float delta_time)
	{
		if (attack_timer > 0.f)
			attack_timer -= delta_time;
	}

	void attack()
	{
		if (is_died || is_hurting || is_attacking || attack_timer > 0.f) return;

		is_attacking = true;
		attack_timer = attack_cooldown;

		audio_manager.playSfx("enemy-sword", 0, 1);

		if (collider->hit)
		{
			Game::getInstance().getEventBus().emit(PlayerTakeDamageEvent{ 10.f });
		}
	}

	void takeDamage(float damage_percentage)
	{
		if (is_died) return;

		if (health - damage_percentage <= 0.f)
		{
			die();
		}

		health -= damage_percentage;
		is_hurting = true;
	}

	void die()
	{
		transform->setVelocity({ 0,0 });
		is_died = true;
		
		despawn_timer = despawn_cooldown;
	}

	void updateDespawnTimer(float delta_time)
	{
		if (despawn_timer > 0.f)
			despawn_timer -= delta_time;
	}

	void despawn()
	{
		if (!is_died) return;

		if (despawn_timer > 0.f) return;

		auto& fire_puff_anim = fire_puff->getComponent<Sprite>();
		
		if (fire_puff_anim.isAnimationPlaying("puff") && fire_puff_anim.getCurrentAnimationFrame() == 7)
			sprite->setVisible(false);

		if (fire_puff_anim.isAnimationPlaying("puff")) return;

		if (!audio_manager.isSfxPlaying("fiery-explosion"))
			audio_manager.playSfx("fiery-explosion", 0);

		fire_puff_anim.setVisible(true);
		fire_puff_anim.play("puff");

		if (fire_puff_anim.isAnimationFinished())
		{
			if (entity->isActive())
			{
				fire_puff->destroy();
				if (entity->hasComponent<EnemyHealthBar>())
					entity->getComponent<EnemyHealthBar>().destroy();
				entity->destroy();
			}
		}
	}

	void moveTowardsPlayer()
	{
		if (is_died) return;

		/*if (is_hurting)
		{
			transform->setVelocity({ 0.f, 0.f });
			return;
		}*/

		if (far_to_right_from_player)
		{
			transform->setVelocity({ -4.f, 0.f });
			is_running = true;
			facing_right = false;
		}
		else if(far_to_left_from_player)
		{
			transform->setVelocity({ 4.0, 0.f });
			is_running = true;
			facing_right = true;
		}
		else
		{
			transform->setVelocity({ 0.f, 0.f });
			is_running = false;
			
			attack();
		}
	}

	void updateAnimation()
	{
		if (is_died)
		{
			is_attacking = false;
			is_hurting = false;

			sprite->play("die");
			return;
		}

		if (is_hurting)
		{
			is_attacking = false;

			sprite->play("hurt");
			if (sprite->isAnimationFinished()) is_hurting = false;

			return;
		}

		if (is_attacking)
		{
			if (!sprite->isAnimationPlaying("attack"))
			{
				sprite->play("attack");
			}

			if (sprite->isAnimationFinished())
			{
				is_attacking = false;
				sprite->play("idle");
			}
			
			return;
		}

		if (is_running)
		{
			sprite->play("run");
		}
		else
		{
			sprite->play("idle");
		}

		sprite->setFlip(!facing_right);
	}

	void updateFirePuffPosition()
	{
		auto& fp_transform = fire_puff->getComponent<Transform>();
		
		float x = transform->getPositionX() - transform->getWidth() * 3.f;
		float y = transform->getPositionY() - transform->getHeight() * 2.7f;

		//std::cout << "fp x: " << x << ", y: " << y << "\n";

		fp_transform.setPositionX(x);
		fp_transform.setPositionY(y);
	}

	void playSounds()
	{
		if (is_hurting && !was_hurting)
		{
			audio_manager.playSfx("enemy-hurt", 0, 1);
		}

		/*if (is_attacking && !was_attacking)
		{
			audio_manager.playSfx("enemy-sword", 0, 1);
		}*/

		if (is_running && !was_running)
		{
			audio_manager.playLoopingSfx("enemy-step-1");
		}
		else if (!is_running && was_running)
		{
			audio_manager.stopLoopingSfx("enemy-step-1");
		}

		was_hurting = is_hurting;
		was_attacking = is_attacking;
		was_running = is_running;
	}

private:
	float health = 100.f;

	bool was_attacking = false;
	bool is_attacking = false;
	bool was_running = false;
	bool is_running = false;
	bool was_hurting = false;
	bool is_hurting = false;
	bool facing_right = false;
	bool far_to_right_from_player = false;
	bool far_to_left_from_player = false;

	float attack_cooldown = 2.f;
	float attack_timer = 0.f;

	float despawn_cooldown = 2.f;
	float despawn_timer = 0.f;

	AssetManager& asset_manager;
	AudioManager& audio_manager;
	EntityManager& entity_manager;
	Player& player;

	Transform* transform = nullptr;
	BoxCollider* collider = nullptr;
	Sprite* sprite = nullptr;
	Input* input = nullptr;
	Entity* fire_puff = nullptr;

	SubscriptionHandle onTakeDamageListener;
};

#endif