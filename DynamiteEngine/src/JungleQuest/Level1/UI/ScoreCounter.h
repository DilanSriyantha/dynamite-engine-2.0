#ifndef SCORE_COUNTER_H
#define SCORE_COUNTER_H

#include "IScript.h"
#include "SDL_ttf.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "UIElements.h"
#include <sstream>
#include <iomanip>
#include "ConfigTypes.h"
#include <cstring>
#include <string>
#include "FirebaseFirestoreManager.h"

class ScoreCounter : public IScript {
public:
	ScoreCounter(AssetManager& asset_manager, EntityManager& entity_manager) :
		asset_manager(asset_manager), entity_manager(entity_manager) {}

	void init() override
	{
		auto& container(entity_manager.addEntity().addComponent<Panel>());
		container.setTransform(10, 80, 960, 50, 1.f);

		label = &entity_manager.addEntity().addComponent<Label>();
		label->dockOnTopLeft(container);
		label->setFont(asset_manager.getFont("def-font-46"));
		label->setFontColor({ 0xff, 0xff, 0xff, 0xff });
		label->setText("0000000000");

		container.entity->addGroup(GRP_UI_ELEMENTS_LAYER_2);
		label->entity->addGroup(GRP_UI_ELEMENTS_LAYER_1);

		auto onEnemyTakeDamage = [&](const EnemyTakeDamageEvent& e)
		{
			score += static_cast<int>(e.damage_percentage);
		};

		auto onGameQuitRequest = [&](const GameQuitRequestEvent& e)
		{
			updateHighScoreRecord();
		};

		Game::getInstance().getEventBus().subscribe<EnemyTakeDamageEvent>(onEnemyTakeDamage);
		Game::getInstance().getEventBus().subscribe<GameQuitRequestEvent>(onGameQuitRequest);
	}

	void update(float delta_time) override
	{
		updateScore();
	}

	void updateScore()
	{
		int desired_width = 10;

		std::stringstream ss;
		ss << std::setfill('0') << std::setw(desired_width) << score;
		std::string str = ss.str();
		
		label->setText(str);
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;

	Label* label = nullptr;
	int score = 0;

	void updateHighScoreRecord()
	{
		auto& fsm = Game::getInstance().getFileSystemManager();
		auto& event_bus = Game::getInstance().getEventBus();

		event_bus.emit<LoadingEvent>({ true });

		fsm.read<AuthResult>(AUTH_CACHE_FILE, [this, &event_bus](AuthResult ac) {
			FirebaseFirestoreManager ffm;

			ffm.readEntry(ac.local_id, ac.id_token, [this, &ac, &ffm, &event_bus](LeaderboardResult lbr) {
				if (lbr.success && lbr.score > score)
				{
					event_bus.emit<LoadingEvent>({ false });
					event_bus.emit<GameQuitEvent>({});
					return;
				}

				ffm.createEntry(ac.local_id, ac.id_token, ac.email, score, [&event_bus](bool is_successfull) {
					if (is_successfull)
					{
						std::cout << "high score recorded successfully.\n";
					}
					else
					{
						std::cout << "error occurred while updating high score\n";
					}

					event_bus.emit<LoadingEvent>({ false });
					event_bus.emit<GameQuitEvent>({});
				});
			});
		});
	}
};

#endif