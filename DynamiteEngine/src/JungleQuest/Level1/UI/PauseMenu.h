#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "ICompositeSceneObject.h"
#include "UIElements.h"
#include "EventTypes.h"

class PauseMenu : public IScript {
public:
	PauseMenu(AssetManager& asset_manager, AudioManager& audio_manager, EntityManager& entity_manager)
		: asset_manager(asset_manager), audio_manager(audio_manager), entity_manager(entity_manager) {}

	void init() override
	{
		auto& pause_panel = entity_manager.addEntity().addComponent<Panel>();
		pause_panel.setSize(1920, 1080);

		auto& pause_bg = entity_manager.addEntity().addComponent<Panel>();
		pause_bg.setSize(948, 881);
		pause_bg.setScale(.5f);
		pause_bg.entity->addComponent<Sprite>(asset_manager.getTexture("pause-bg")).setVisible(false);
		pause_bg.center(pause_panel);

		auto& pause_table = entity_manager.addEntity().addComponent<ImagePanel>();
		pause_table.setSize(824, 554);
		pause_table.setScale(.5f);
		pause_table.entity->addComponent<Sprite>(asset_manager.getTexture("pause-table")).setVisible(false);
		pause_table.dockOnTopCenter(pause_bg);
		pause_table.setGroup(GRP_UI_ELEMENTS_LAYER_2);

		auto& pause_hint = entity_manager.addEntity().addComponent<ImagePanel>();
		pause_hint.setSize(628, 227);
		pause_hint.setScale(.5f);
		pause_hint.entity->addComponent<Sprite>(asset_manager.getTexture("pause-hint")).setVisible(false);
		pause_hint.center(pause_table);
		pause_hint.setGroup(GRP_UI_ELEMENTS_LAYER_2);

		auto& pause_header = entity_manager.addEntity().addComponent<Panel>();
		pause_header.setSize(1003, 396);
		pause_header.setScale(.5f);
		pause_header.entity->addComponent<Sprite>(asset_manager.getTexture("pause-header")).setVisible(false);
		pause_header.dockOnTopCenter(pause_bg);
		pause_header.paddingTop(-100);
		pause_header.setGroup(GRP_UI_ELEMENTS_LAYER_1);

		auto& btn_play = entity_manager.addEntity().addComponent<Button>();
		btn_play.setTransform(0, 0, 214, 215, .5f);
		btn_play.addTexture("idle", asset_manager.getTexture("btn-play"), true);
		btn_play.center(pause_bg);
		btn_play.paddingLeft(-100);
		btn_play.paddingTop(120);
		btn_play.setVisible(false);
		btn_play.setOnClickListener([]() {
			std::cout << "clicked\n";
			Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_PAUSE, false });
			Game::getInstance().getEventBus().emit(GamePauseEvent{false});
		});

		auto& btn_quit = entity_manager.addEntity().addComponent<Button>();
		btn_quit.setTransform(0, 0, 214, 215, .5f);
		btn_quit.addTexture("idle", asset_manager.getTexture("btn-quit"), true);
		btn_quit.center(pause_bg);
		btn_quit.paddingLeft(100);
		btn_quit.paddingTop(120);
		btn_quit.setVisible(false);
		btn_quit.setOnClickListener([]() {
			Game::getInstance().getEventBus().emit(GameQuitRequestEvent{});
		});

		auto onPausePanelRequested = [&](const MenuPanelRequestEvent& e)
		{
			if (e.pnl != PNL_PAUSE) return;

			if (e.open)
			{
				pause_panel.setVisible(true);
				pause_bg.entity->getComponent<Sprite>().setVisible(true);
				pause_header.entity->getComponent<Sprite>().setVisible(true);
				pause_table.entity->getComponent<Sprite>().setVisible(true);
				pause_hint.entity->getComponent<Sprite>().setVisible(true);
				btn_play.setVisible(true);
				btn_quit.setVisible(true);
			}
			else
			{
				pause_panel.setVisible(false);
				pause_bg.entity->getComponent<Sprite>().setVisible(false);
				pause_header.entity->getComponent<Sprite>().setVisible(false);
				pause_table.entity->getComponent<Sprite>().setVisible(false);
				pause_hint.entity->getComponent<Sprite>().setVisible(false);
				btn_play.setVisible(false);
				btn_quit.setVisible(false);
			}
		};

		auto onGameQuit = [&](const GameQuitEvent& e)
		{
			Game::getInstance().setPaused(false);
			Game::getInstance().getSceneManager().requestSceneChange("menu");
		};

		Game::getInstance().getEventBus().subscribe<MenuPanelRequestEvent>(onPausePanelRequested);
		Game::getInstance().getEventBus().subscribe<GameQuitEvent>(onGameQuit);
	}

private:
	AssetManager& asset_manager;
	AudioManager& audio_manager;
	EntityManager& entity_manager;
};

#endif