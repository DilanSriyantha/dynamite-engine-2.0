#ifndef IMAGE_PANEL_H
#define IMAGE_PANEL_H

#include "IComponent.h"
#include "IPostionController.h"
#include "Entity.h"
#include "Vector2D.h"
#include "SDL.h"
#include <vector>
#include "Transform.h"
#include "Game.h"
#include "ICanvas.h"
#include "Sprite.h"

class ImagePanel : public ICanvas {
public:
	ImagePanel() = default;

	void init() override
	{
		if(!entity->hasComponent<Transform>())
			entity->addComponent<Transform>(0, 0, 10, 15, 1);

		if(!entity->hasComponent<Sprite>())
			entity->addComponent<Sprite>();

		entity->addGroup(GRP_UI_ELEMENTS_LAYER_2);
	}

	void setVisible(bool flag)
	{
		entity->getComponent<Sprite>().setVisible(flag);
	}

	void setTransform(int x, int y, int w, int h, float scale)
	{
		getLocalTransform().setTransform(x, y, w, h, scale);
	}

	void setTransform(Vector2D position, Vector2D size, int scale)
	{
		setTransform(position.x, position.y, size.x, size.y, scale);
	}

	SDL_Rect getTransformRect()
	{
		return entity->getComponent<Transform>().getTransformRect();
	}

	void setPosition(int x, int y)
	{
		entity->getComponent<Transform>().setPosition(x, y);
	}

	void setPosition(Vector2D position)
	{
		setPosition(position.x, position.y);
	}

	void setSize(int w, int h)
	{
		entity->getComponent<Transform>().setSize(w, h);
	}

	void setSize(Vector2D size)
	{
		setSize(size.x, size.y);
	}

	Vector2D getSize()
	{
		return entity->getComponent<Transform>().getSize();
	}

	void setScale(float scale)
	{
		entity->getComponent<Transform>().setScale(scale);
	}

	float getScale()
	{
		return entity->getComponent<Transform>().getScale();
	}

	void setTexture(SDL_Texture* texture)
	{
		entity->getComponent<Sprite>().setTransformData(getLocalTransform().getData());
		entity->getComponent<Sprite>().setTexture(texture);
	}

	void setGroup(Group group)
	{
		if (entity->hasGroup(GRP_UI_ELEMENTS_LAYER_2))
			entity->delGroup(GRP_UI_ELEMENTS_LAYER_2);

		if (entity->hasGroup(group)) return;

		entity->addGroup(group);
	}

private:
	Transform& getLocalTransform() const override
	{
		return entity->getComponent<Transform>();
	}
};

#endif