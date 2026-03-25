#ifndef LEVEL1_H
#define LEVEL1_H

#include "IScene.h"
#include "Components.h"
#include "UI/HUD.h"
#include "UI/PauseMenu.h"
#include "UI/GameOverPanel.h"
#include "UI/HeartChallenge.h"
#include "UI/EnemyHealthBar.h"
#include "UI/ScoreCounter.h"
#include "Gameplay/Player.h"
#include "Gameplay/ParallaxBackground.h"
#include "Gameplay/EnemySpawnSystem.h"

class Level1 : public IScene {
public:
	Level1() = default;

	~Level1()
	{
		IScene::~IScene();
	}

	void init() override
	{
		IScene::init();

		asset_manager->addTexture("loading", "Assets/game/ui/load_bar/loading_sheet.png");
		asset_manager->addTexture("bar-bg", "Assets/game/ui/bubble/bgload.png");
		asset_manager->addTexture("heart", "Assets/game/ui/bubble/heart.png");
		asset_manager->addTexture("bar-fill", "Assets/game/ui/bubble/load.png");
		asset_manager->addTexture("pause-bg", "Assets/game/ui/pause/bg.png");
		asset_manager->addTexture("pause-table", "Assets/game/ui/pause/table.png");
		asset_manager->addTexture("pause-hint", "Assets/game/ui/pause/text.png");
		asset_manager->addTexture("pause-header", "Assets/game/ui/pause/header.png");
		asset_manager->addTexture("challenge-header", "Assets/game/ui/pause/challenge_header.png");
		asset_manager->addTexture("game-over-header", "Assets/game/ui/pause/game_over_header.png");
		asset_manager->addTexture("btn-empty", "Assets/game/ui/btn/empty.png");
		asset_manager->addTexture("btn-play", "Assets/game/ui/btn/play.png");
		asset_manager->addTexture("btn-quit", "Assets/game/ui/btn/close.png");
		asset_manager->addTexture("btn-restart", "Assets/game/ui/btn/restart.png");

		asset_manager->addFont("def-font-24", "Assets/game/fonts/soupofjustice.ttf", 24);
		asset_manager->addFont("def-font-46", "Assets/game/fonts/soupofjustice.ttf", 46);

		asset_manager->addTexture("player", "Assets/game/PNG/player.png");
		asset_manager->addTexture("enemy", "Assets/game/PNG/enemy.png");

		asset_manager->addTexture("vfx-fire-puff", "Assets/game/PNG/vfx_fire_puff.png");
		asset_manager->addTexture("vfx-water-whirl", "Assets/game/PNG/vfx_water_whirl.png");

		audio_manager->addSfx("ambience", "Assets/game/sounds/forest-ambience.wav");
		audio_manager->setVolumeSfx("ambience", 80);
		audio_manager->addMusic("music", "Assets/game/sounds/chill-jungle-music.wav");

		audio_manager->addSfx("player-sword", "Assets/game/sounds/player-sword.wav");
		audio_manager->setVolumeSfx("player-sword", 50);
		audio_manager->addSfx("player-step-1", "Assets/game/sounds/stepstone_1.wav");
		audio_manager->setVolumeSfx("player-step-1", 40);
		audio_manager->addSfx("player-hurt", "Assets/game/sounds/player-hurt.wav");
		audio_manager->setVolumeSfx("player-hurt", 50);

		audio_manager->addSfx("enemy-sword", "Assets/game/sounds/enemy-sword.wav");
		audio_manager->setVolumeSfx("enemy-sword", 50);
		audio_manager->addSfx("enemy-step-1", "Assets/game/sounds/stepstone_1.wav");
		audio_manager->setVolumeSfx("enemy-step-1", 40);
		audio_manager->addSfx("enemy-hurt", "Assets/game/sounds/enemy-hurt.ogg");
		audio_manager->setVolumeSfx("enemy-hurt", 75);
		audio_manager->addSfx("fiery-explosion", "Assets/game/sounds/flame-spell-impact.wav");
		audio_manager->setVolumeSfx("fiery-explosion", 40);
		audio_manager->addSfx("cute-level-up-1", "Assets/game/sounds/cute-level-up-1.wav");
		audio_manager->setVolumeSfx("cute-level-up-1", 60);

		auto& ui_root(entity_manager->addEntity());
		ui_root.addComponent<LoadingOverlay>(getAssetManager(), getEntityManager());
		ui_root.addComponent<HUD>(getAssetManager(), getAudioManager(), getEntityManager());
		ui_root.addComponent<PauseMenu>(getAssetManager(), getAudioManager(), getEntityManager());
		ui_root.addComponent<GameOverPanel>(getAssetManager(), getEntityManager());
		ui_root.addComponent<HeartChallenge>(getAssetManager(), getEntityManager());

		auto& parallax_background_root(entity_manager->addEntity());
		parallax_background_root.addComponent<ParallaxBackground>(getActiveCamera(), getAssetManager(), getEntityManager());

		auto& player(entity_manager->addEntity());
		player.addComponent<Player>(getActiveCamera(), getAssetManager(), getAudioManager(), getEntityManager());

		/*auto& enemy(entity_manager->addEntity());
		enemy.addComponent<Enemy>(getAssetManager(), getEntityManager(), player.getComponent<Player>());
		enemy.addComponent<EnemyHealthBar>(getAssetManager(), getEntityManager());*/

		//auto& puff(entity_manager->addEntity());
		//puff.addComponent<Transform>(1000, 100, 400, 400, 3.f);
		//puff.addComponent<Sprite>(asset_manager->getTexture("vfx-fire-puff"), true);
		//puff.getComponent<Sprite>().addAnimation("puff", 0, 15, .05f, true);
		//puff.getComponent<Sprite>().play("puff");
		//puff.addGroup(GRP_OBJECTS);

		//auto& water_whirl(entity_manager->addEntity());
		//water_whirl.addComponent<Transform>(150, 500, 195, 195, 2.f);
		//water_whirl.addComponent<Sprite>(asset_manager->getTexture("vfx-water-whirl"), true);
		//water_whirl.getComponent<Sprite>().addAnimation("water-whirl", 0, 42, .05f, false);
		//water_whirl.getComponent<Sprite>().play("water-whirl");
		//water_whirl.addGroup(GRP_OBJECTS);

		auto& enemy_spawn_system(entity_manager->addEntity());
		enemy_spawn_system.addComponent<EnemySpawnSystem>(getAssetManager(), getAudioManager(), getEntityManager(), player.getComponent<Player>());

		audio_manager->playLoopingSfx("ambience");
		audio_manager->playMusic("music", -1);
	}

	void update(float delta_time) override
	{
		IScene::update(delta_time);
	}

	void draw() override
	{
		IScene::draw();
	}
};

#endif