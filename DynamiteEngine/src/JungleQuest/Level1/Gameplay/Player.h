#ifndef PLAYER_H
#define PLAYER_H

#include "IScene.h"
#include "Components.h"
#include "IScript.h"
#include "EventTypes.h"

class Player : public IScript {
public:
	Player(Camera& active_camera, AssetManager& asset_manager, AudioManager& audio_manager, EntityManager& entity_manager)
		: active_camera(active_camera), asset_manager(asset_manager), audio_manager(audio_manager), entity_manager(entity_manager) {}

	void init() override
	{
		// create water whirl entity
		water_whirl = &entity_manager.addEntity();
		water_whirl->addComponent<Transform>(0, 0, 195, 195, 2.f);
		water_whirl->addComponent<Sprite>(asset_manager.getTexture("vfx-water-whirl"), true);
		water_whirl->getComponent<Sprite>().addAnimation("water-whirl", 0, 42, .008f, false);
		water_whirl->getComponent<Sprite>().setVisible(false);
		//water_whirl->getComponent<Sprite>().play("water-whirl");

		// create player entity
		transform = &entity->addComponent<Transform>(100, 680, 200, 200, 2.f); // x 100, y 680

		sprite = &entity->addComponent<Sprite>(asset_manager.getTexture("player"), true);
		entity->getComponent<Sprite>().addAnimation("idle", 0, 17, .05f);
		entity->getComponent<Sprite>().addAnimation("idle-blink", 1, 17, .05f);
		entity->getComponent<Sprite>().addAnimation("run", 2, 11, .06f);
		entity->getComponent<Sprite>().addAnimation("attack", 3, 11, .06f, false);
		entity->getComponent<Sprite>().addAnimation("hurt", 4, 11, .06f, false);
		entity->getComponent<Sprite>().play("idle");

		input = &entity->addComponent<Input>();
		collider = &entity->addComponent<BoxCollider>(COL_PLAYER, true);
		//entity->getComponent<BoxCollider>().showGizmo();

		active_camera.setWorldBounds(0, 0, 1920, 1080);
		active_camera.setInfiniteMode(true);
		active_camera.follow(&entity->getComponent<Transform>());

		entity->addGroup(GRP_PLAYERS);
		water_whirl->addGroup(GRP_OBJECTS);

		addPLayerTakeDamageEventListener();
		addPlayerHealthResetEventListener();
	}

	void update(float delta_time)
	{
		handleInput(delta_time);
		updateAttackTimer(delta_time);
		updateAnimation();
		playSound();
		updateWaterWhirlPosition();
		playResetHealthEffect();
		invokeHeartChallenge();
	}

	bool isMovingRight()
	{
		return is_running && facing_right;
	}

	bool isMovingLeft()
	{
		return is_running && !facing_right;
	}

private:
	float health = 100.f;
	float attack_cooldown = 1.f;
	float attack_timer = 0.f;

	bool was_health_reset = false;
	bool is_health_reset = false;
	bool was_attacking = false;
	bool is_attacking = false;
	bool was_running = false;
	bool is_running = false;
	bool was_hurting = false;
	bool is_hurting = false;
	bool facing_right = true;

	Camera& active_camera;
	AssetManager& asset_manager;
	AudioManager& audio_manager;
	EntityManager& entity_manager;

	Transform* transform = nullptr;
	BoxCollider* collider = nullptr;
	Sprite* sprite = nullptr;
	Input* input = nullptr;
	Entity* water_whirl = nullptr;

	void addPLayerTakeDamageEventListener()
	{
		auto onPlayerTakeDamage = [&](const PlayerTakeDamageEvent& e)
		{
			takeDamage(e.damage_percentage);
		};

		Game::getInstance().getEventBus().subscribe<PlayerTakeDamageEvent>(onPlayerTakeDamage);
	}

	void addPlayerHealthResetEventListener()
	{
		auto onPlayerHealthReset = [&](const PlayerHealthResetEvent& e)
		{
			is_health_reset = true;
			health = 100.f;
		};

		Game::getInstance().getEventBus().subscribe<PlayerHealthResetEvent>(onPlayerHealthReset);
	}

	void invokeHeartChallenge()
	{
		if (health > 0.f) return;
		if (is_hurting) return;

		/*Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_CHALLENGE, true });*/

		Game::getInstance().getEventBus().emit(PlayerUseLifeLineEvent{});
	}

	void playResetHealthEffect()
	{
		auto& water_whirl_anim = water_whirl->getComponent<Sprite>();

		if (!is_health_reset && water_whirl_anim.isAnimationFinished())
			water_whirl_anim.setVisible(false);

		if (is_health_reset)
		{
			if (water_whirl_anim.isAnimationPlaying("water-whirl")) return;

			water_whirl_anim.setVisible(true);
			water_whirl_anim.replay("water-whirl");

			if (!audio_manager.isSfxPlaying("cute-level-up-1"))
				audio_manager.playSfx("cute-level-up-1", 0, 1);

			is_health_reset = false;
		}
	}

	void takeDamage(float damage_percentage)
	{
		if (health <= 0.f) return;

		health -= damage_percentage;
		is_hurting = true;
	}

	void attack()
	{
		if (is_attacking) return;
		if (attack_timer > 0.f) return;

		is_attacking = true;
		attack_timer = attack_cooldown;

		//std::cout << "player attack triggered\n";

		if (collider->hit)
		{
			Game::getInstance().getEventBus().emit(EnemyTakeDamageEvent{ 30.f });
		}
	}

	void moveLeft()
	{
		is_running = true;
		facing_right = false;
		active_camera.setFollowOffset(-400.f); // -400

		if (transform->getPositionX() < 0.f)
		{
			transform->setPositionX(0.f);
			return;
		}
		transform->setVelocity({ -6.f, 0.f });

		Game::getInstance().getEventBus().emit(PlayerMoveLeftEvent());
	}

	void moveRight()
	{
		is_running = true;
		facing_right = true;
		active_camera.setFollowOffset(800.f); // 800

		transform->setVelocity({ 6.f, 0.f });

		Game::getInstance().getEventBus().emit(PlayerMoveRightEvent());
	}

	void stopMoving()
	{
		is_running = false;

		transform->setVelocity({ 0,0 });

		Game::getInstance().getEventBus().emit(PlayerStoppedEvent{});
	}

	void handleInput(float delta_time)
	{
		if (is_hurting) return;
		if (is_attacking) return;

		if (input->isKeyPressed(SDL_SCANCODE_D))
		{
			moveRight();
		}
		else if (input->isKeyPressed(SDL_SCANCODE_A))
		{
			moveLeft();
		}
		else
		{
			stopMoving();
		}

		if (input->isKeyPressed(SDL_SCANCODE_KP_5))
		{
			attack();
		}
	}

	void updateWaterWhirlPosition()
	{
		auto& w_transform = water_whirl->getComponent<Transform>();

		float x = transform->getPositionX() - transform->getWidth() / 2.f + w_transform.getWidth() / 2.f;
		float y = transform->getPositionY() - transform->getHeight() / 2.f + w_transform.getHeight() / 2.f;

		w_transform.setPositionX(x);
		w_transform.setPositionY(y);
	}

	void updateAttackTimer(float delta_time)
	{
		if (attack_timer > 0.f)
		{
			attack_timer -= delta_time;
		}
	}

	void updateAnimation()
	{
		if (is_hurting)
		{
			sprite->play("hurt");
			if (sprite->isAnimationFinished()) is_hurting = false;

			return;
		}

		if (is_attacking)
		{
			sprite->play("attack");
			if (sprite->isAnimationFinished()) is_attacking = false;

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

	void playSound()
	{
		if (is_hurting && !was_hurting)
		{
			audio_manager.playSfx("player-hurt", 0, 1);
		}

		if (is_attacking && !was_attacking)
		{
			audio_manager.playSfx("player-sword", 0, 1);
		}

		if (is_running && !was_running)
		{
			audio_manager.playLoopingSfx("player-step-1");
		}
		else if (!is_running && was_running)
		{
			audio_manager.stopLoopingSfx("player-step-1");
		}

		was_hurting = is_hurting;
		was_attacking = is_attacking;
		was_running = is_running;
	}
};

#endif