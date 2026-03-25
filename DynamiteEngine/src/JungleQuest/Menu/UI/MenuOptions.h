#ifndef MENU_OPTIONS_H
#define MENU_OPTIONS_H

#include "IScript.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "UIElements.h"
#include <functional>
#include "EventTypes.h"

class MenuOptions : public IScript {
public:
	MenuOptions(AssetManager& asset_manager, EntityManager& entity_manager)
		: asset_manager(asset_manager), entity_manager(entity_manager) {}

	void init() override
	{
		auto& container = entity->addComponent<Panel>();
		auto& logo = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& btn_new_game = entity_manager.addEntity().addComponent<Button>();
		auto& btn_high_score = entity_manager.addEntity().addComponent<Button>();
		auto& btn_quit = entity_manager.addEntity().addComponent<Button>();
		auto& btn_about = entity_manager.addEntity().addComponent<Button>();
		auto& user_det_container = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& user_det_face = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& user_det_lbl_1 = entity_manager.addEntity().addComponent<Label>();
		auto& user_det_lbl_2 = entity_manager.addEntity().addComponent<Label>();
		auto& btn_logout = entity_manager.addEntity().addComponent<Button>();

		std::function<void()> loadNewGame = []()
		{
			Game::getInstance().getSceneManager().requestSceneChange("lvl1");
		};

		std::function<void()> loadHighScore = []()
		{
			Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_LEADERBOARD, true });
		};

		std::function<void()> loadAbout = []()
		{
			Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_ABOUT, true });
		};

		std::function<void()> quit = []()
		{
			Game::getInstance().shutdown();
		};

		container.setSize(1920, 1080);
		container.setVisible(false);

		logo.setSize(1360, 754);
		logo.setScale(.8f);
		logo.setTexture(asset_manager.getTexture("logo"));
		logo.center(container);
		logo.paddingTop(-100);
		logo.setVisible(false);

		btn_new_game.setSize(498, 500);
		btn_new_game.setScale(.4f);
		btn_new_game.addTexture("idle", asset_manager.getTexture("btn-play"), true);
		btn_new_game.center(container);
		btn_new_game.paddingTop(250);
		btn_new_game.setVisible(false);
		btn_new_game.setOnClickListener(loadNewGame);

		btn_high_score.setSize(300, 301);
		btn_high_score.setScale(.5);
		btn_high_score.addTexture("idle", asset_manager.getTexture("btn-leader-board"), true);
		btn_high_score.dockOnBottomRight(container);
		btn_high_score.paddingLeft(-10);
		btn_high_score.paddingTop(-10);
		btn_high_score.setVisible(false);
		btn_high_score.setOnClickListener(loadHighScore);

		btn_about.setSize(300, 301);
		btn_about.setScale(.35f);
		btn_about.addTexture("idle", asset_manager.getTexture("btn-about"), true);
		btn_about.dockOnTopLeft(container);
		btn_about.paddingLeft(10);
		btn_about.paddingTop(10);
		btn_about.setVisible(false);
		btn_about.setOnClickListener(loadAbout);

		btn_quit.setSize(214, 215);
		btn_quit.setScale(.5f);
		btn_quit.addTexture("idle", asset_manager.getTexture("btn-quit"), true);
		btn_quit.dockOnTopRight(container);
		btn_quit.paddingTop(10);
		btn_quit.paddingLeft(-10);
		btn_quit.setVisible(false);
		btn_quit.setOnClickListener(quit);

		user_det_container.setSize(1298, 972);
		user_det_container.setScale(.2f);
		user_det_container.setTexture(asset_manager.getTexture("sh-bg"));
		user_det_container.dockOnBottomLeft(container);
		user_det_container.paddingLeft(10);
		user_det_container.paddingTop(-10);
		user_det_container.setVisible(false);

		user_det_face.setSize(177, 177);
		user_det_face.setScale(.5f);
		user_det_face.setTexture(asset_manager.getTexture("face"));
		user_det_face.dockOnTopLeft(user_det_container);
		user_det_face.paddingTop(15);
		user_det_face.paddingLeft(20);
		user_det_face.setVisible(false);

		btn_logout.setSize(308, 107);
		btn_logout.setScale(.4f);
		btn_logout.addTexture("idle", asset_manager.getTexture("btn-pill"), true);
		btn_logout.setFont(asset_manager.getFont("def-font-24"));
		btn_logout.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		btn_logout.setText("Logout");
		btn_logout.centerVertical(user_det_container);
		btn_logout.paddingLeft(125);
		btn_logout.paddingTop(-35);
		btn_logout.setVisible(false);
		btn_logout.setOnClickListener([]() {
			AuthResult empty{};
			Game::getInstance().getFileSystemManager().write(AUTH_CACHE_FILE, empty, []() {
				Game::getInstance().getEventBus().emit<MenuPanelRequestEvent>({ PNL_LOGIN, true });
			});
		});

		user_det_lbl_1.setFont(asset_manager.getFont("def-font-24"));
		user_det_lbl_1.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		user_det_lbl_1.setText("Logged In");
		user_det_lbl_1.dockOnBottomLeft(user_det_container);
		user_det_lbl_1.paddingTop(-50);
		user_det_lbl_1.paddingLeft(20);
		user_det_lbl_1.setVisible(false);

		user_det_lbl_2.setFont(asset_manager.getFont("calibri-regular-24"));
		user_det_lbl_2.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		user_det_lbl_2.setText("NULL");
		user_det_lbl_2.dockOnBottomLeft(user_det_container);
		user_det_lbl_2.dockBelow(user_det_lbl_1);
		user_det_lbl_2.paddingLeft(20);
		user_det_lbl_2.setVisible(false);

		auto onMenuPanelRequest = [&](const MenuPanelRequestEvent& e)
		{
			if (e.pnl == PNL_MENU_OPTIONS && e.open)
			{
				logo.setVisible(true);
				btn_new_game.setVisible(true);
				btn_high_score.setVisible(true);
				btn_about.setVisible(true);
				btn_quit.setVisible(true);
				user_det_container.setVisible(true);
				user_det_face.setVisible(true);
				user_det_face.setVisible(true);
				user_det_lbl_1.setVisible(true);
				user_det_lbl_2.setVisible(true);
			}
			else if(e.open)
			{
				logo.setVisible(false);
				btn_new_game.setVisible(false);
				btn_high_score.setVisible(false);
				btn_about.setVisible(false);
				btn_quit.setVisible(false);
				user_det_container.setVisible(false);
				user_det_face.setVisible(false);
				btn_logout.setVisible(false);
				user_det_lbl_1.setVisible(false);
				user_det_lbl_2.setVisible(false);
			}
			else
			{
				logo.setVisible(true);
				btn_new_game.setVisible(true);
				btn_high_score.setVisible(true);
				btn_about.setVisible(true);
				btn_quit.setVisible(true);
				user_det_container.setVisible(true);
				user_det_face.setVisible(true);
				btn_logout.setVisible(true);
				user_det_lbl_1.setVisible(true);
				user_det_lbl_2.setVisible(true);
			}
		};

		auto onUserLoggedIn = [&](const UserLoggedInEvent& e)
		{
			user_det_lbl_2.setText(e.auth_result.email);

			Game::getInstance().getEventBus().emit<MenuPanelRequestEvent>({ PNL_MENU_OPTIONS, true });
		};

		Game::getInstance().getEventBus().subscribe<MenuPanelRequestEvent>(onMenuPanelRequest);
		Game::getInstance().getEventBus().subscribe<UserLoggedInEvent>(onUserLoggedIn);
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;
};

#endif