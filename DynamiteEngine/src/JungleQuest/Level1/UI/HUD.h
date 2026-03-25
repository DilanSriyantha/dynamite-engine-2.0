#ifndef HUD_H
#define HUD_H

#include "IScene.h"
#include "UIElements.h"
#include "EventTypes.h"
#include "ICompositeSceneObject.h"
#include "HealthBar.h"
#include "ScoreCounter.h"
#include "LifelineIndicator.h"

class HUD : public IScript {
public:
	HUD(AssetManager& asset_manager, AudioManager& audio_manager, EntityManager& entity_manager)
		: asset_manager(asset_manager), audio_manager(audio_manager), entity_manager(entity_manager) {}

	void init() override
	{
		asset_manager.addTexture("btn-pause", "Assets/game/ui/btn/pause.png");

		auto& hud_panel = entity_manager.addEntity().addComponent<Panel>();
		hud_panel.setSize(1920, 1080);
		
		auto& btn_pause = entity_manager.addEntity().addComponent<Button>();
		btn_pause.setTransform(0, 0, 214, 215, .5f);
		btn_pause.addTexture("idle", asset_manager.getTexture("btn-pause"), true);
		btn_pause.dockOnTopRight(hud_panel);
		btn_pause.paddingLeft(-10);
		btn_pause.paddingTop(10);
		btn_pause.setOnClickListener([]() {
			Game::getInstance().getEventBus().emit(MenuPanelRequestEvent{ PNL_PAUSE, true });
			Game::getInstance().getEventBus().emit(GamePauseEvent{true});
		});

		auto& health_bar = hud_panel.entity->addComponent<HealthBar>(asset_manager, entity_manager);
		auto& score_counter = hud_panel.entity->addComponent<ScoreCounter>(asset_manager, entity_manager);
		auto& life_line_indicator = hud_panel.entity->addComponent<LifelineIndicator>(entity_manager, asset_manager);

		auto onGamePaused = [&](const GamePauseEvent& e)
		{
			if (e.state)
			{
				Game::getInstance().setPaused(true);
				btn_pause.setVisible(false);

			}
			else
			{
				Game::getInstance().setPaused(false);
				btn_pause.setVisible(true);
			}
		};
		
		Game::getInstance().getEventBus().subscribe<GamePauseEvent>(onGamePaused);
	}

private:
	AssetManager& asset_manager;
	AudioManager& audio_manager;
	EntityManager& entity_manager;
};

#endif