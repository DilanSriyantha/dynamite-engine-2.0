#ifndef HEART_CHALLENGE_H
#define HEART_CHALLENGE_H

#include "IScript.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "UIElements.h"
#include "TextureManager.h"
#include <fstream>
#include <random>
#include <algorithm>
#include <set>

class HeartChallenge : public IScript {
public:
	HeartChallenge(AssetManager& asset_manager, EntityManager& entity_manager) :
		asset_manager(asset_manager), entity_manager(entity_manager) {}

	void init() override
	{
		auto& panel = entity_manager.addEntity().addComponent<Panel>();
		auto& loading = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& bg = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& table = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& question_caption = entity_manager.addEntity().addComponent<Label>();
		auto& question = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& header = entity_manager.addEntity().addComponent<ImagePanel>();
		auto& btn_1 = entity_manager.addEntity().addComponent<Button>();
		auto& btn_2 = entity_manager.addEntity().addComponent<Button>();
		auto& btn_3 = entity_manager.addEntity().addComponent<Button>();

		panel.setSize(1920, 1080);
		panel.setVisible(false);

		loading.setSize(220, 40);
		loading.setScale(1.f);
		loading.setTexture(asset_manager.getTexture("loading"));
		loading.center(panel);
		loading.setVisible(false);

		bg.setSize(948, 881);
		bg.setScale(.5f);
		bg.setTexture(asset_manager.getTexture("pause-bg"));
		bg.center(panel);
		bg.setVisible(false);

		table.setSize(824, 554);
		table.setScale(.5f);
		table.setTexture(asset_manager.getTexture("pause-table"));
		table.dockOnTopCenter(bg);
		table.setGroup(GRP_UI_ELEMENTS_LAYER_2);
		table.setVisible(false);

		question_caption.setFont(asset_manager.getFont("def-font-24"));
		question_caption.setText("How many hearts are there?");
		question_caption.dockOnTopCenter(table);
		question_caption.paddingTop(80);
		question_caption.setVisible(false);

		question.setSize(660, 360);
		question.setScale(.4f);
		question.centerHorizontal(table);
		question.dockBelow(question_caption);
		question.setGroup(GRP_UI_ELEMENTS_LAYER_1);
		question.setVisible(false);

		header.setSize(1153, 455);
		header.setScale(.5f);
		header.setTexture(asset_manager.getTexture("challenge-header"));
		header.dockOnTopCenter(bg);
		header.paddingTop(-120);
		header.setGroup(GRP_UI_ELEMENTS_LAYER_1);
		header.setVisible(false);

		btn_1.setSize(214, 215);
		btn_1.setScale(.5f);
		btn_1.addTexture("idle", asset_manager.getTexture("btn-empty"), true);
		btn_1.setFont(asset_manager.getFont("def-font-46"));
		btn_1.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		btn_1.setText("01");
		btn_1.dockOnBottomLeft(bg);
		btn_1.paddingTop(-50);
		btn_1.paddingLeft(60);
		btn_1.setVisible(false);

		btn_2.setSize(214, 215);
		btn_2.setScale(.5f);
		btn_2.addTexture("idle", asset_manager.getTexture("btn-empty"), true);
		btn_2.setFont(asset_manager.getFont("def-font-46"));
		btn_2.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		btn_2.setText("02");
		btn_2.dockOnBottomCenter(bg);
		btn_2.paddingTop(-50);
		btn_2.setVisible(false);

		btn_3.setSize(214, 215);
		btn_3.setScale(.5f);
		btn_3.addTexture("idle", asset_manager.getTexture("btn-empty"), true);
		btn_3.setFont(asset_manager.getFont("def-font-46"));
		btn_3.setFontColor({ 0xff, 0xff, 0xff, 0xff });
		btn_3.setText("03");
		btn_3.dockOnBottomRight(bg);
		btn_3.paddingTop(-50);
		btn_3.paddingLeft(-60);
		btn_3.setVisible(false);

		btn_ptrs[0] = &btn_1;
		btn_ptrs[1] = &btn_2;
		btn_ptrs[2] = &btn_3;
		question_ptr = &question;
	
		Game::getInstance().getEventBus().subscribe<MenuPanelRequestEvent>([&](const MenuPanelRequestEvent& e) {
			if (e.pnl != PNL_CHALLENGE) return;

			if (e.open)
			{
				panel.setVisible(true);
				loading.setVisible(true);

				Game::getInstance().setPaused(true);

				loadQuestion();
			}
			else
			{
				Game::getInstance().setPaused(false);

				panel.setVisible(false);
				bg.setVisible(false);
				table.setVisible(false);
				question_caption.setVisible(false);
				question.setVisible(false);
				header.setVisible(false);
				btn_1.setVisible(false);
				btn_2.setVisible(false);
				btn_3.setVisible(false);
			}
		});

		Game::getInstance().getEventBus().subscribe<ChallengeLoadedEvent>([&](const ChallengeLoadedEvent& e) {
			loading.setVisible(false);

			panel.setVisible(true);
			bg.setVisible(true);
			table.setVisible(true);
			question_caption.setVisible(true);
			question.setVisible(true);
			header.setVisible(true);
			btn_1.setVisible(true);
			btn_2.setVisible(true);
			btn_3.setVisible(true);
		});
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;

	std::array<Button*, 3> btn_ptrs = {};
	ImagePanel* question_ptr = nullptr;

	void loadQuestion()
	{
		auto& client = Game::getInstance().getHttpRequestManager();

		client.get("https://marcconrad.com/uob/heart/api.php", [&](json response) {
			std::string question_url = response["question"];
			int solution = response["solution"];
			int carrots = response["carrots"];

			client.downloadToMemory(question_url, [this, solution, carrots](MemoryStruct mem_struct) {
				question_ptr->setTexture(TextureManager::loadTextureFromBuffer((void*)mem_struct.memory, mem_struct.size, Game::getInstance().getRendererPtr()));
				free(mem_struct.memory);

				std::set<int> excluded = { solution, carrots };
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dist(0, 10);

				int random_num;
				bool is_excluded;
				
				do {
					random_num = dist(gen);
					is_excluded = excluded.count(random_num) > 0;
				} while (is_excluded);

				std::shuffle(btn_ptrs.begin(), btn_ptrs.end(), gen);
				btn_ptrs[0]->setText(std::to_string(solution));
				btn_ptrs[0]->setOnClickListener([]() {
					Game::getInstance().getEventBus().emit(PlayerHealthResetEvent{});
					Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_CHALLENGE, false });
				});

				btn_ptrs[1]->setText(std::to_string(carrots));
				btn_ptrs[1]->setOnClickListener([]() {
					Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_CHALLENGE, false });
				});

				btn_ptrs[2]->setText(std::to_string(random_num));
				btn_ptrs[2]->setOnClickListener([]() {
					Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_CHALLENGE, false });
				});

				Game::getInstance().getEventBus().emit(ChallengeLoadedEvent{});
			});
		});
	}
};

#endif