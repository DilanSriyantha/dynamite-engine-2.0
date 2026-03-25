#ifndef PARALLAX_BACKGROUND_H
#define PARALLAX_BACKGROUND_H

#include "ICompositeSceneObject.h"
#include "Components.h"

class ParallaxBackground : public IScript {
public:
	ParallaxBackground(Camera& active_camera, AssetManager& asset_manager, EntityManager& entity_manager)
		: active_camera(active_camera), asset_manager(asset_manager), entity_manager(entity_manager) {}

	void init() override
	{
		// load assets
		asset_manager.addTexture("sky", "Assets/game/PNG/Cartoon_Forest_BG_01/Layers/Sky.png");
		asset_manager.addTexture("bg_decor", "Assets/game/PNG/Cartoon_Forest_BG_01/Layers/BG_Decor.png");
		asset_manager.addTexture("middle_decor", "Assets/game/PNG/Cartoon_Forest_BG_01/Layers/Middle_Decor.png");
		asset_manager.addTexture("foreground", "Assets/game/PNG/Cartoon_Forest_BG_01/Layers/Foreground.png");
		asset_manager.addTexture("ground", "Assets/game/PNG/Cartoon_Forest_BG_01/Layers/Ground.png");

		const int LAYER_WIDTH = 1920;
		const int LAYER_HEIGHT = 1080;

		// helper lambda to create parallax layers
		auto createParallaxLayer = [&](const std::string& texture_name, float parallax_factor, std::vector<Entity*>& layers_list, Group group)
		{
			auto& layer_a = entity_manager.addEntity();
			auto& layer_b = entity_manager.addEntity();

			layer_a.addComponent<Transform>(0, 0, LAYER_WIDTH, LAYER_HEIGHT, 1.f);
			layer_a.addComponent<Sprite>(asset_manager.getTexture(texture_name)).setParallaxFactor(parallax_factor);
			layer_a.addGroup(group);

			layer_b.addComponent<Transform>(LAYER_WIDTH, 0, LAYER_WIDTH, LAYER_HEIGHT, 1.f);
			layer_b.addComponent<Sprite>(asset_manager.getTexture(texture_name)).setParallaxFactor(parallax_factor);
			layer_b.addGroup(group);

			layers_list.push_back(&layer_a);
			layers_list.push_back(&layer_b);
		};

		auto& sky = entity_manager.addEntity();
		sky.addComponent<Transform>(0, 0, 1920, 1080, 1.f);
		sky.addComponent<Sprite>(asset_manager.getTexture("sky"));
		sky.addGroup(GRP_SKYBOX);

		// create parallax background tiles
		createParallaxLayer("bg_decor", 0.2f, bg_layers_1, GRP_MAP_LAYER_4);
		createParallaxLayer("middle_decor", 0.5f, bg_layers_2, GRP_MAP_LAYER_3);
		createParallaxLayer("foreground", 0.8f, bg_layers_3, GRP_MAP_LAYER_2);
		createParallaxLayer("ground", 1.f, bg_layers_4, GRP_MAP_LAYER_1);
	}

	void update(float delta_time) override
	{
		updateParallaxLayers();
	}

	void updateParallaxLayers()
	{
		const float camera_x = active_camera.getPosition().x;
		const float viewport_x = active_camera.getViewport().x;
		const int LAYER_WIDTH = 1920;

		auto scrollLayer = [&](std::vector<Entity*>& layers)
		{
			if (layers.size() < 2) return;

			auto& a = layers[0]->getComponent<Transform>();
			auto& b = layers[1]->getComponent<Transform>();
			auto& a_sprite = layers[0]->getComponent<Sprite>();
			auto& b_sprite = layers[1]->getComponent<Sprite>();

			float a_offset = a.getPositionX() - viewport_x * a_sprite.getParallaxFactor();
			float b_offset = b.getPositionX() - viewport_x * b_sprite.getParallaxFactor();

			if (a_offset + LAYER_WIDTH < 0)
			{
				a.setPositionX(b.getPositionX() + LAYER_WIDTH);
			}
			else if (b_offset + LAYER_WIDTH < 0)
			{
				b.setPositionX(a.getPositionX() + LAYER_WIDTH);
			}

			if (a_offset > LAYER_WIDTH)
			{
				a.setPositionX(b.getPositionX() - LAYER_WIDTH);
			}
			else if (b_offset > LAYER_WIDTH)
			{
				b.setPositionX(a.getPositionX() - LAYER_WIDTH);
			}
		};

		scrollLayer(bg_layers_1);
		scrollLayer(bg_layers_2);
		scrollLayer(bg_layers_3);
		scrollLayer(bg_layers_4);
	}

private:
	Camera& active_camera;
	AssetManager& asset_manager;
	EntityManager& entity_manager;

	std::vector<Entity*> bg_layers_1;
	std::vector<Entity*> bg_layers_2;
	std::vector<Entity*> bg_layers_3;
	std::vector<Entity*> bg_layers_4;
};

#endif