#ifndef LABEL_H
#define LABEL_H

#include <string>
#include "Vector2D.h"
#include "IComponent.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Transform.h"
#include "IPostionController.h"
#include "Panel.h"

class Label : public ICanvas {
public:
	Label() = default;

	~Label()
	{
		SDL_DestroyTexture(font_tex);
		TTF_CloseFont(font);
	}

	void setText(const std::string& txt, Uint32 wrap_length = 0U, bool auto_adjust_transform_size = true)
	{
		if (font_tex != nullptr)
			SDL_DestroyTexture(font_tex);

		this->text = txt;

		SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), f_color, wrap_length);
		font_tex = SDL_CreateTextureFromSurface(Game::getInstance().getRendererPtr(), surface);
		SDL_FreeSurface(surface);
		SDL_QueryTexture(font_tex, nullptr, nullptr, &dest_rect.w, &dest_rect.h); // since we do not know the width of the text;

		if(auto_adjust_transform_size)
			getLocalTransform().setSize(dest_rect.w, dest_rect.h);
	}

	void setFont(TTF_Font* font)
	{
		this->font = font;
	}

	void setFontColor(const SDL_Color& font_color)
	{
		f_color = font_color;
	}

	void setFontColor(SDL_Color& font_color)
	{
		f_color = font_color;
	}

	void setPosition(int x, int y)
	{
		dest_rect.x = x;
		dest_rect.y = y;
	}

	void setPosition(Vector2D position)
	{
		setPosition(position.x, position.y);
	}

	void setVisible(bool visible)
	{
		this->visible = visible;
	}

	bool getVisible()
	{
		return visible;
	}

	void moveTo(Vector2D position)
	{
		setPosition(position);
	}

	void moveBy(Vector2D by)
	{
		setPosition(
			dest_rect.x + by.x,
			dest_rect.y + by.y
		);
	}

	SDL_Rect& getRect()
	{
		return dest_rect;
	}

	SDL_Rect* getRectPtr() 
	{
		return &dest_rect;
	}

	void init() override
	{
		if(!entity->hasComponent<Transform>())
			entity->addComponent<Transform>(0, 0, 10, 15, 1);

		dest_rect.x = getLocalTransform().getPositionX();
		dest_rect.y = getLocalTransform().getPositionY();

		entity->addGroup(GRP_UI_ELEMENTS_LAYER_1);
	}

	void update(float delta_time) override
	{
		if (!entity->hasComponent<Transform>()) return;

		dest_rect.x = getLocalTransform().getPositionX() + (getLocalTransform().getWidth() * getLocalTransform().getScale() - dest_rect.w) / 2;
		dest_rect.y = getLocalTransform().getPositionY() + (getLocalTransform().getHeight() * getLocalTransform().getScale() - dest_rect.h) / 2;
	}

	void updateWhilePause(float delta_time) override
	{
		if (!entity->hasComponent<Transform>()) return;

		dest_rect.x = getLocalTransform().getPositionX() + (getLocalTransform().getWidth() * getLocalTransform().getScale() - dest_rect.w) / 2;
		dest_rect.y = getLocalTransform().getPositionY() + (getLocalTransform().getHeight() * getLocalTransform().getScale() - dest_rect.h) / 2;
	}

	void draw(SDL_Rect* viewport) override
	{
		if (!visible) return;

		SDL_RenderCopy(Game::getInstance().getRendererPtr(), font_tex, nullptr, &dest_rect);
	}

private:
	std::string text;
	TTF_Font* font = nullptr;
	SDL_Color f_color { 0x00, 0x00, 0x00, 0x00 };
	SDL_Rect dest_rect;
	SDL_Texture* font_tex = nullptr;

	bool visible = true;

	Transform& getLocalTransform() const override
	{
		return entity->getComponent<Transform>();
	}
};

#endif