#ifndef LIFELINE_INDICATOR_H
#define LIFELINE_INDICATOR_H

#include "IScript.h"
#include "AssetManager.h"
#include "UIElements.h"
#include "Components.h"
#include "Entity.h"
#include <array>
#include <stack>

#define MAX_LIFE_LINES 3

struct LifeLine {
	Entity* entity = nullptr;
	bool active = true;

	LifeLine() = default;

	LifeLine(EntityManager& entity_manager, AssetManager& asset_manager)
	{
		entity = &entity_manager.addEntity();

		auto& img = entity->addComponent<ImagePanel>();
		img.setSize(100, 100);
		img.setScale(.3f);
		img.setPosition(0.f, 150.f);
		img.setTexture(asset_manager.getTexture("heart"));
		
		entity->addGroup(GRP_UI_ELEMENTS_LAYER_1);
	}

	void deactivate()
	{
		if (!entity) return;

		active = false;
		entity->destroy();
	}

	ImagePanel& getImg()
	{
		return entity->getComponent<ImagePanel>();
	}
};

class LifelineIndicator : public IScript {
public:
	LifelineIndicator(EntityManager& entity_manager, AssetManager& asset_manager) : entity_manager(entity_manager), asset_manager(asset_manager)
	{
		float start_x = 10.f;
		for (int i = 0; i < MAX_LIFE_LINES; i++)
		{
			float pos_x = start_x + (35.f * i);
			LifeLine life_line(entity_manager, asset_manager);
			life_line.getImg().setPosition(pos_x, 125.f);
			life_lines.push(std::move(life_line));
		}
	}

	void init() override
	{
		auto onPlayerUseLifeLine = [&](const PlayerUseLifeLineEvent& e)
		{
			if (active_life_lines > 0)
			{
				life_lines.top().deactivate();
				life_lines.pop();
				active_life_lines -= 1;

				Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_CHALLENGE, true });
			}
			else
			{
				Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_CHALLENGE, false });

				if(!Game::getInstance().isPaused())
					Game::getInstance().getEventBus().emit(GamePauseEvent{ true });

				Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_GAME_OVER, true });
			}

			std::cout << "life lines left: " << active_life_lines << "\n";
		};

		Game::getInstance().getEventBus().subscribe<PlayerUseLifeLineEvent>(onPlayerUseLifeLine);
	}

	void update(float delta_time) override
	{

	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;

	std::stack<LifeLine> life_lines;
	size_t active_life_lines = MAX_LIFE_LINES;
};

#endif