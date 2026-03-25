#ifndef BUTTON_H
#define BUTTON_H

#include "IScript.h"
#include <string>
#include "SDL_ttf.h"
#include <map>
#include <functional>
#include "Game.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Sprite.h"
#include "Label.h"
#include "Input.h"
#include "BoxCollider.h"
#include "Game.h"
#include "EventBus.h"
#include "EventTypes.h"

using TextureMap = std::map<std::string, SDL_Texture*>;

class Button : public ICanvas {
public:
	Button() = default;

	void init() override
	{
		if (!entity->hasComponent<Transform>())
			entity->addComponent<Transform>(0, 0, 10, 5, 1);

		if (!entity->hasComponent<Sprite>())
			entity->addComponent<Sprite>();

		if (!entity->hasComponent<Input>())
			entity->addComponent<Input>();

		if (!entity->hasComponent<BoxCollider>())
			entity->addComponent<BoxCollider>(COL_OBJECT, true);

		if (!entity->hasComponent<Label>())
			entity->addComponent<Label>();

		entity->addGroup(GRP_UI_ELEMENTS_LAYER_1);
	}

	void update(float delta_time) override
	{
		checkButtonClick();
	}

	void updateWhilePause(float delta_time) override
	{
		checkButtonClick();
	}

	void checkButtonClick()
	{
		if (!entity->hasComponent<BoxCollider>()) return;

		if (!getVisible()) return;

		auto& collider = entity->getComponent<BoxCollider>();
		if (collider.hit)
		{
			if (!was_clicked)
			{
				was_clicked = true;
				if (on_click) on_click();
			}
		}
		else
		{
			was_clicked = false;
		}
	}

	void setPosition(int x, int y)
	{
		getLocalTransform().setPosition(x, y);
		entity->getComponent<Label>().center(*this);
	}

	void setSize(int w, int h)
	{
		getLocalTransform().setSize(w, h);
	}

	void setScale(float scale)
	{
		getLocalTransform().setScale(scale);
	}

	void addTexture(const std::string& id, SDL_Texture* tex)
	{
		tex_map.emplace(id, std::move(tex));
	}

	void addTexture(const std::string& id, SDL_Texture* tex, bool activate)
	{
		tex_map.emplace(id, std::move(tex));
		if(activate) setTexture(id);
	}

	void setTexture(const std::string& id)
	{
		entity->getComponent<Sprite>().setTransformData(entity->getComponent<Transform>().getData());
		entity->getComponent<Sprite>().setTexture(tex_map[id]);
	}

	void setDimensions(int w, int h, int scale)
	{
		getLocalTransform().setWidth(w);
		getLocalTransform().setHeight(h);
		getLocalTransform().setScale(scale);
		entity->getComponent<Label>().center(*this);
	}

	void setTransform(int x, int y, int w, int h, float scale)
	{
		getLocalTransform().setTransform(x, y, w, h, scale);
	}

	void setFont(TTF_Font* font)
	{
		entity->getComponent<Label>().setFont(font);
	}

	void setFontColor(SDL_Color f_color)
	{
		entity->getComponent<Label>().setFontColor(f_color);
	}

	void setText(std::string text)
	{
		entity->getComponent<Label>().setText(text, 0U, false);
		entity->getComponent<Label>().center(getLocalTransform());
	}

	void setText(std::string text, bool auto_adjust_transform_size)
	{

		entity->getComponent<Label>().setText(text, 0U, auto_adjust_transform_size);
		entity->getComponent<Label>().center(getLocalTransform());
	}

	void setVisible(bool flag)
	{
		entity->getComponent<Sprite>().setVisible(flag);
		entity->getComponent<Label>().setVisible(flag);
	}

	bool getVisible()
	{
		return entity->getComponent<Sprite>().getVisible();
	}

	void setGroup(Group group)
	{
		entity->addGroup(group);
	}

	void setOnClickListener(std::function<void()> on_click)
	{
		this->on_click = on_click;
	}
private:
	TextureMap tex_map;
	std::function<void()> on_click;
	bool was_clicked = false;

	Transform& getLocalTransform() const
	{
		return entity->getComponent<Transform>();
	}
};

#endif