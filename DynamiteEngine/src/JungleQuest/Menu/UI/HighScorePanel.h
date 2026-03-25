#ifndef HIGH_SCORE_PANEL_H
#define HIGH_SCORE_PANEL_H

#include "IScript.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "UIElements.h"
#include "SDL_rect.h"
#include "FileSystemManager.h"
#include "ConfigTypes.h"
#include <sstream>
#include <iomanip>
#include "JungleQuest/Helpers/ScoreManager.h"
#include "FirebaseFirestoreManager.h"

class HighScorePanel : public IScript {
public:
	HighScorePanel(AssetManager& asset_manager, EntityManager& entity_manager)
		: asset_manager(asset_manager), entity_manager(entity_manager) 
	{}

	void init() override
	{
		auto& container = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& header = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& table = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& lbl_caption = entity_manager.addEntity().addComponent<Label>();
		lbl_score = &entity_manager.addEntity().addComponent<Label>();
		auto& btn_close = entity_manager.addEntity().addComponent<Button>();

		container.setSize(948, 881);
		container.setScale(.5f);
		container.setTexture(asset_manager.getTexture("high-score-panel-bg"));
		container.center({ 0, 0, 1920, 1080 });
		container.setVisible(false);

		header.setSize(1153, 455);
		header.setScale(.5f);
		header.setTexture(asset_manager.getTexture("header-high-score"));
		header.dockOnTopCenter(container);
		header.paddingTop(-100);
		header.setVisible(false);

		table.setSize(824, 554);
		table.setScale(.5f);
		table.setTexture(asset_manager.getTexture("table"));
		table.dockOnTopCenter(container);
		table.setVisible(false);

		lbl_caption.setFont(asset_manager.getFont("def-font-24"));
		lbl_caption.setText("Your Best Score");
		lbl_caption.center(table);
		lbl_caption.setVisible(false);

		lbl_score->setFont(asset_manager.getFont("def-font-46"));
		lbl_score->setFontColor({ 0x04, 0x7f, 0x9c });
		lbl_score->setText("0000000000");
		lbl_score->center(table);
		lbl_score->dockBelow(lbl_caption);
		lbl_score->setVisible(false);

		header.setGroup(GRP_UI_ELEMENTS_LAYER_1);

		auto onClose = []()
		{
			Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_LEADERBOARD, false });
		};

		btn_close.setSize(214, 215);
		btn_close.setScale(.5);
		btn_close.addTexture("idle", asset_manager.getTexture("btn-close"), true);
		btn_close.center(container);
		btn_close.paddingTop(120);
		btn_close.setOnClickListener(onClose);
		btn_close.setVisible(false);

		auto onLeaderBoardRequest = [&](const MenuPanelRequestEvent& e)
		{
			if (e.pnl != PNL_LEADERBOARD) return;

			if (e.open)
			{
				std::cout << "leaderboard requested\n";
				container.setVisible(true);
				header.setVisible(true);
				table.setVisible(true);
				lbl_caption.setVisible(true);
				lbl_score->setVisible(true);
				btn_close.setVisible(true);

				loadHighScore();
			}
			else
			{
				container.setVisible(false);
				header.setVisible(false);
				table.setVisible(false);
				lbl_caption.setVisible(false);
				lbl_score->setVisible(false);
				btn_close.setVisible(false);
			}
		};

		Game::getInstance().getEventBus().subscribe<MenuPanelRequestEvent>(onLeaderBoardRequest);
	}

	void update(float delta_time) override
	{
		//lbl_score->setText(std::to_string(high_score));
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;

	Label* lbl_score = nullptr;

	void loadHighScore()
	{
		Game::getInstance().getEventBus().emit<LoadingEvent>({ true });

		auto& fsm = Game::getInstance().getFileSystemManager();
		fsm.read<AuthResult>(AUTH_CACHE_FILE, [this](AuthResult ac) {
			if (ac.success)
			{
				FirebaseFirestoreManager ffm;
				ffm.readEntry(std::string(ac.local_id), std::string(ac.id_token), [this](LeaderboardResult lbr) {
					Game::getInstance().getEventBus().emit<LoadingEvent>({ false });

					if (lbr.success)
					{
						updateHighScoreLabel(lbr.score);
						return;
					}

					updateHighScoreLabel(0);
					std::cout << "error occurred while reading high score record: " << lbr.error_message << "\n";
				});
			}
		});
	}

	void updateHighScoreLabel(size_t hs)
	{
		if (!lbl_score) return;

		int desired_width = 10;
		
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(desired_width) << hs;
		std::string str = ss.str();

		lbl_score->setText(str);
	}
};

#endif