#ifndef LOADING_OVERLAY_H
#define LOADING_OVERLAY_H

#include "IScript.h"
#include "AssetManager.h"
#include "EntityManager.h"
#include "UIElements.h"
#include "EventTypes.h"
#include "Game.h"

class LoadingOverlay : public IScript {
public:
	LoadingOverlay(AssetManager& asset_manager, EntityManager& entity_manager)
		: asset_manager(asset_manager), entity_manager(entity_manager)
	{}

	void init() override
	{
		auto& panel(entity_manager.addEntity().addComponent<Panel>());
		auto& loading(entity_manager.addEntity().addComponent<ImagePanel>());

		panel.setSize(1920, 1080);
		panel.setGroup(GRP_UI_ELEMENTS_LAYER_0);
		panel.setVisible(false);

		loading.setSize(220, 40);
		loading.setScale(1.f);
		loading.setTexture(asset_manager.getTexture("loading"));
		loading.center(panel);
		loading.setGroup(GRP_UI_ELEMENTS_LAYER_M_1);
		loading.setVisible(false);

		auto onLoading = [&](const LoadingEvent& e)
		{
			if (e.is_loading)
			{
				panel.setVisible(true);
				loading.setVisible(true);
			}
			else
			{
				panel.setVisible(false);
				loading.setVisible(false);
			}
		};

		Game::getInstance().getEventBus().subscribe<LoadingEvent>(onLoading);
	}

private:
	AssetManager& asset_manager;
	EntityManager& entity_manager;
};

#endif