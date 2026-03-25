#ifndef ABOUT_PANEL_H
#define ABOUT_PANEL_H

#include "IScript.h"
#include "UIElements.h"

class AboutPanel : public IScript {
public:
	AboutPanel(AssetManager& asset_manager, EntityManager& entity_manager)
		: asset_manager(asset_manager), entity_manager(entity_manager) {}

	void init() override
	{
		auto& container = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& header = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& table = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& lbl_caption = entity_manager.addEntity().addComponent<Label>();
		auto& btn_close = entity_manager.addEntity().addComponent<Button>();

		container.setSize(948, 881);
		container.setScale(.5f);
		container.setTexture(asset_manager.getTexture("high-score-panel-bg"));
		container.center({ 0, 0, 1920, 1080 });
		container.setVisible(false);

		header.setSize(1153, 455);
		header.setScale(.5f);
		header.setTexture(asset_manager.getTexture("header-about"));
		header.dockOnTopCenter(container);
		header.paddingTop(-100);
		header.setVisible(false);

		table.setSize(824, 554);
		table.setScale(.5f);
		table.setTexture(asset_manager.getTexture("table"));
		table.dockOnTopCenter(container);
		table.setVisible(false);
		
		lbl_caption.setFont(asset_manager.getFont("def-font-24"));
		lbl_caption.setText("\n           Controls\n\nW               Move Right\nA                 Move Left\nNum 5               Attack");
		lbl_caption.center(table);
		lbl_caption.setVisible(false);

		header.setGroup(GRP_UI_ELEMENTS_LAYER_1);

		auto onClose = []()
		{
			Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_ABOUT, false });
		};

		btn_close.setSize(214, 215);
		btn_close.setScale(.5);
		btn_close.addTexture("idle", asset_manager.getTexture("btn-close"), true);
		btn_close.center(container);
		btn_close.paddingTop(120);
		btn_close.setOnClickListener(onClose);
		btn_close.setVisible(false);

		auto onAboutPanelRequest = [&](const MenuPanelRequestEvent& e)
		{
			if (e.pnl != PNL_ABOUT) return;

			if (e.open)
			{
				std::cout << "leaderboard requested\n";
				container.setVisible(true);
				header.setVisible(true);
				table.setVisible(true);
				lbl_caption.setVisible(true);
				btn_close.setVisible(true);
			}
			else
			{
				container.setVisible(false);
				header.setVisible(false);
				table.setVisible(false);
				lbl_caption.setVisible(false);
				btn_close.setVisible(false);
			}
		};

		Game::getInstance().getEventBus().subscribe<MenuPanelRequestEvent>(onAboutPanelRequest);
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;
};

#endif