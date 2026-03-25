#ifndef MENU_H
#define MENU_H

#include "IScene.h"
#include "Components.h"
#include "UIElements.h"
#include "UI/MenuOptions.h"
#include "UI/HighScorePanel.h"
#include "UI/AboutPanel.h"
#include "UI/RegisterPanel.h"
#include "UI/LoginPanel.h"
#include "UI/LoadingOverlay.h"
#include "Gameplay/ScrollBackground.h"

#include "UIElements/TextInputBox.h"

class Menu : public IScene {
public:
	Menu() = default;

	~Menu()
	{
		IScene::~IScene();
	}

	void init() override
	{
		IScene::init();

		asset_manager->addTexture("logo", "Assets/game/ui/menu/logo_mod.png");
		asset_manager->addTexture("btn-play", "Assets/game/ui/menu/play.png");
		asset_manager->addTexture("btn-leader-board", "Assets/game/ui/menu/leader.png");
		asset_manager->addTexture("btn-about", "Assets/game/ui/menu/about.png");
		asset_manager->addTexture("btn-quit", "Assets/game/ui/btn/close.png");
		asset_manager->addTexture("header-high-score", "Assets/game/ui/pause/high_score_header.png");
		asset_manager->addTexture("header-about", "Assets/game/ui/pause/about_header.png");
		asset_manager->addTexture("header-register", "Assets/game/ui/pause/register_header.png");
		asset_manager->addTexture("header-login", "Assets/game/ui/pause/login_header.png");
		asset_manager->addTexture("high-score-panel-bg", "Assets/game/ui/pause/bg.png");
		asset_manager->addTexture("table", "Assets/game/ui/pause/table.png");
		asset_manager->addTexture("btn-close", "Assets/game/ui/btn/close.png");
		asset_manager->addTexture("btn-ok", "Assets/game/ui/btn/ok.png");
		asset_manager->addTexture("btn-pill", "Assets/game/ui/shop/btn.png");
		asset_manager->addTexture("face", "Assets/game/ui/rating/face.png");
		asset_manager->addTexture("sh-bg", "Assets/game/ui/shop/bg.png");
		asset_manager->addTexture("loading", "Assets/game/ui/load_bar/loading_sheet.png");

		asset_manager->addFont("def-font-16", "Assets/game/fonts/soupofjustice.ttf", 16);
		asset_manager->addFont("def-font-24", "Assets/game/fonts/soupofjustice.ttf", 24);
		asset_manager->addFont("def-font-46", "Assets/game/fonts/soupofjustice.ttf", 46);
		asset_manager->addFont("calibri-regular-24", "Assets/game/fonts/calibri-regular.ttf", 24);

		audio_manager->addSfx("ambience", "Assets/game/sounds/forest-ambience.wav");

		auto& ui_root(entity_manager->addEntity());
		ui_root.addComponent<LoadingOverlay>(getAssetManager(), getEntityManager());
		ui_root.addComponent<MenuOptions>(getAssetManager(), getEntityManager());
		ui_root.addComponent<HighScorePanel>(getAssetManager(), getEntityManager());
		ui_root.addComponent<AboutPanel>(getAssetManager(), getEntityManager());
		ui_root.addComponent<RegisterPanel>(getAssetManager(), getEntityManager());
		ui_root.addComponent<LoginPanel>(getAssetManager(), getEntityManager());

		auto& scroll_bg(entity_manager->addEntity().addComponent<ScrollBackground>(getAssetManager(), getEntityManager()));

		audio_manager->playLoopingSfx("ambience");

		if (Game::getInstance().isPaused())
			Game::getInstance().setPaused(false);
	}

	void update(float delta_time) override
	{
		IScene::update(delta_time);
	}

	void draw() override
	{
		IScene::draw();
	}

	void unload() override
	{
		IScene::unload();
	}
};

#endif