#ifndef GAME_OVER_PANEL_H
#define GAME_OVER_PANEL_H

#include "IScript.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "UIElements.h"

class GameOverPanel : public IScript {
public:
	GameOverPanel(AssetManager& asset_manager, EntityManager& entity_manager) : asset_manager(asset_manager), entity_manager(entity_manager) {}

	void init() override
	{
		auto& panel = entity_manager.addEntity().addComponent<Panel>();
		auto& container = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& header = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& table = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& lbl_caption = entity_manager.addEntity().addComponent<Label>();
		auto& btn_close = entity_manager.addEntity().addComponent<Button>();

		panel.setSize(1920, 1080);

		container.setSize(948, 881);
		container.setScale(.5f);
		container.setTexture(asset_manager.getTexture("pause-bg"));
		container.center({ 0, 0, 1920, 1080 });
		container.setVisible(false);

		header.setSize(1153, 455);
		header.setScale(.5f);
		header.setTexture(asset_manager.getTexture("game-over-header"));
		header.dockOnTopCenter(container);
		header.paddingTop(-100);
		header.setVisible(false);

		table.setSize(824, 554);
		table.setScale(.5f);
		table.setTexture(asset_manager.getTexture("pause-table"));
		table.dockOnTopCenter(container);
		table.setVisible(false);

		lbl_caption.setFont(asset_manager.getFont("def-font-24"));
		lbl_caption.setText("You have consumed\nall the life-lines.\nWanna play again?");
		lbl_caption.center(table);
		lbl_caption.setVisible(false);

		header.setGroup(GRP_UI_ELEMENTS_LAYER_1);

		auto onClose = []()
		{
			auto& event_bus = Game::getInstance().getEventBus();

			event_bus.emit(GameQuitRequestEvent{});
			event_bus.emit(MenuPanelRequestEvent{ PNL_GAME_OVER, false });
		};

		btn_close.setSize(214, 215);
		btn_close.setScale(.5);
		btn_close.addTexture("idle", asset_manager.getTexture("btn-restart"), true);
		btn_close.center(container);
		btn_close.paddingTop(120);
		btn_close.setOnClickListener(onClose);
		btn_close.setVisible(false);

		auto onGameOverPanelRequest = [&](const MenuPanelRequestEvent& e)
		{
			if (e.pnl != PNL_GAME_OVER) return;

			if (e.open)
			{
				std::cout << "game over panel requested\n";
				panel.setVisible(true);
				container.setVisible(true);
				header.setVisible(true);
				table.setVisible(true);
				lbl_caption.setVisible(true);
				btn_close.setVisible(true);
			}
			else
			{
				panel.setVisible(false);
				container.setVisible(false);
				header.setVisible(false);
				table.setVisible(false);
				lbl_caption.setVisible(false);
				btn_close.setVisible(false);
			}
		};

		Game::getInstance().getEventBus().subscribe<MenuPanelRequestEvent>(onGameOverPanelRequest);
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;
};

#endif