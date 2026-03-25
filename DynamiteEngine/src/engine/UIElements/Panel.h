#ifndef CANVAS_H
#define CANVAS_H

#include "IComponent.h"
#include "IPostionController.h"
#include "Entity.h"
#include "Vector2D.h"
#include "SDL.h"
#include <vector>
#include "Transform.h"
#include "Game.h"
#include "ICanvas.h"

class Panel : public ICanvas {
public:
	Panel() {}

	Panel& setVisible(bool flag)
	{
		this->visible = flag;

		return *this;
	}

	Panel& setTransform(int x, int y, int w, int h, int scale)
	{
		entity->getComponent<Transform>().setTransform(x, y, w, h, scale);

		return *this;
	}

	Panel& setTransform(Vector2D position, Vector2D size, int scale)
	{
		return setTransform(position.x, position.y, size.x, size.y, scale);
	}

	SDL_Rect getTransformRect()
	{
		return entity->getComponent<Transform>().getTransformRect();
	}

	Panel& setPosition(int x, int y)
	{
		entity->getComponent<Transform>().setPosition(x, y);

		return *this;
	}

	Panel& setPosition(Vector2D position)
	{
		return setPosition(position.x, position.y);
	}

	Panel& setSize(int w, int h)
	{
		entity->getComponent<Transform>().setSize(w, h);

		return *this;
	}

	Panel& setSize(Vector2D size)
	{
		return setSize(size.x, size.y);
	}

	Vector2D getSize()
	{
		return entity->getComponent<Transform>().getSize();
	}

	Panel& setScale(float scale)
	{
		entity->getComponent<Transform>().setScale(scale);

		return *this;
	}

	Panel& setColor(SDL_Color color)
	{
		this->background_color = color;

		return *this;
	}

	Panel& setGroup(Group group)
	{
		entity->addGroup(group);

		return *this;
	}

	void init() override
	{
		auto& local_transform = entity->addComponent<Transform>(0, 0, 10, 15, 1);
		bg_rect.x = local_transform.getPositionX();
		bg_rect.y = local_transform.getPositionY();
		bg_rect.w = local_transform.getWidth();
		bg_rect.h = local_transform.getHeight();

		entity->addGroup(GRP_UI_ELEMENTS_LAYER_3);
	}

	void update(float delta_time) override
	{
		bg_rect.x = getLocalTransform().getPositionX();
		bg_rect.y = getLocalTransform().getPositionY();
		bg_rect.w = getLocalTransform().getWidth();
		bg_rect.h = getLocalTransform().getHeight();
	}

	void draw(SDL_Rect* viewport) override
	{
		if (!visible) return;
		
		SDL_SetRenderDrawBlendMode(Game::getInstance().getRendererPtr(), SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(Game::getInstance().getRendererPtr(), background_color.r, background_color.g, background_color.b, background_color.a);
		SDL_RenderFillRect(Game::getInstance().getRendererPtr(), &bg_rect);
		SDL_SetRenderDrawColor(Game::getInstance().getRendererPtr(), 0xff, 0xff, 0xff, 0xff);
		SDL_SetRenderDrawBlendMode(Game::getInstance().getRendererPtr(), SDL_BLENDMODE_NONE);
	}

private:
	bool visible = false;
	SDL_Rect bg_rect{ 0, 0, 0, 0 };
	SDL_Color background_color { 0x00, 0x00, 0x00, 0x90 };

	Transform& getLocalTransform() const override
	{
		return entity->getComponent<Transform>();
	}
};

#endif