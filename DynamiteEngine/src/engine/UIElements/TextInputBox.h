#ifndef TEXT_INPUT_BOX_H
#define TEXT_INPUT_BOX_H

#include "ICanvas.h"
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

class TextInputBox : public ICanvas {
public:
	TextInputBox(TTF_Font* font)
	{
		this->font = font;
	}

	TextInputBox(TTF_Font* font, bool password_mode)
	{
		this->font = font;
		this->password_mode = password_mode;
	}

	void init() override
	{
		if (!entity->hasComponent<Transform>())
			entity->addComponent<Transform>(0, 0, 200, 46, 1.f);

		if (!entity->hasComponent<Input>())
			entity->addComponent<Input>();

		if (!entity->hasComponent<BoxCollider>())
			entity->addComponent<BoxCollider>(COL_OBJECT, true);

		entity->addGroup(GRP_UI_ELEMENTS_LAYER_1);
	}

	void handleEvents(const SDL_Event& event) override
	{
		if (!visible) return;

		if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			handleMouseDown(event.motion.x, event.motion.y);
		}

		if (!focused) return;

		if (event.type == SDL_KEYDOWN)
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				if (caret_index > 0)
				{
					caret_index--;
					adjustScroll();
				}
				break;

			case SDLK_RIGHT:
				if (caret_index < text.size())
				{
					caret_index++;
					adjustScroll();
				}
				break;
			case SDLK_BACKSPACE:
				if (caret_index > 0)
				{
					text.pop_back();
					caret_index--;
					adjustScroll();
				}
				break;
			}

		if (event.type == SDL_TEXTINPUT)
		{
			if (text.size() < max_chars)
			{
				text += event.text.text;
				caret_index += strlen(event.text.text);
				adjustScroll();
			}
		}
	}

	void update(float delta_time) override
	{
		if (!visible) return;

		blinkCaret(delta_time);
	}

	void draw(SDL_Rect* viewport) override
	{
		if (!visible) return;

		if (!entity->hasComponent<Transform>()) return;

		SDL_Renderer* renderer = Game::getInstance().getRendererPtr();
		auto& transform = entity->getComponent<Transform>();
		auto& pos = transform.getData().position;
		box = { (int)pos.x, (int)pos.y, (int)transform.getWidth(), (int)transform.getHeight() };

		SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, 255);
		SDL_RenderFillRect(renderer, &box);

		// Draw text using SDL_ttf
		if (!text.empty())
		{
			std::string txt_out = text;
			if (password_mode)
				for (auto& c : txt_out)
					c = '*';

			SDL_Surface* surf = TTF_RenderText_Blended(font, txt_out.c_str(), f_color);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

			SDL_Rect src = {
				scroll_offset, 
				0,
				box.w + 10,
				surf->h
			};

			int visible_width = (((surf->w - scroll_offset) < (box.w - 10)) ? (surf->w - scroll_offset) : (box.w - 10));
			if (visible_width < 0) visible_width = 0;

			SDL_Rect dst = {
				box.x + 5,
				box.y + (box.h - surf->h) / 2,
				visible_width,
				surf->h
			};

			SDL_RenderCopy(renderer, tex, &src, &dst);

			SDL_FreeSurface(surf);
			SDL_DestroyTexture(tex);
		}

		// caret
		if (focused && caret_visible)
		{
			int caret_px = measureTextWidth(text.substr(0, caret_index)) - scroll_offset;
			int caret_x = box.x + caret_px + 5;

			SDL_Rect caret = { caret_x, box.y + 5, 2, (int)transform.getHeight() - 10};
			
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
			SDL_RenderFillRect(renderer, &caret);
		}
	}

	void setSize(int w, int h)
	{
		if (!entity->hasComponent<Transform>()) return;

		entity->getComponent<Transform>().setSize(w, h);
	}

	void setSize(Vector2D size)
	{
		setSize((int)size.x, (int)size.y);
	}

	void setPosition(float x, float y)
	{
		if (!entity->hasComponent<Transform>()) return;

		entity->getComponent<Transform>().setPosition(x, y);
	}

	void setPosition(Vector2D position)
	{
		setPosition(position.x, position.y);
	}

	void setPasswordMode(bool password_mode)
	{
		this->password_mode = password_mode;
	}

	void setVisible(bool visible)
	{
		this->visible = visible;
	}

	std::string getText()
	{
		return text;
	}

	void setText(const std::string& txt)
	{
		this->text = txt;
	}

private:
	bool focused = false;
	bool caret_visible = true;
	bool was_clicked = false;
	bool password_mode = false;
	bool visible = true;

	size_t max_chars = 50;

	std::string text;

	float blink_timer = 0.f;

	int scroll_offset = 0;
	size_t caret_index = 0;

	SDL_Color f_color{ 0x00, 0x00, 0x00, 0x00 };
	SDL_Color bg_color{ 0xff, 0xff, 0xff, 0xff };
	TTF_Font* font = nullptr;

	SDL_Rect box{ 0,0,0,0 };

	int measureTextWidth(const std::string& t)
	{
		if (!font || t.empty()) return 0;

		int w = 0, h = 0;
		TTF_SizeText(font, t.c_str(), &w, &h);
		return w;
	}

	Transform& getLocalTransform() const override
	{
		return entity->getComponent<Transform>();
	}

	void adjustScroll()
	{
		if (!entity->hasComponent<Transform>()) return;

		auto& transform = entity->getComponent<Transform>();
		int caret_px = measureTextWidth(text.substr(0, caret_index));
		int box_innder_w = transform.getWidth() - 10;

		if (caret_px - scroll_offset > box.w)
		{
			scroll_offset = caret_px - box.w;
		}
		else if (caret_px - scroll_offset < 0)
		{
			scroll_offset = caret_px;
		}

		if (scroll_offset < 0)
			scroll_offset = 0;
	}

	void blinkCaret(float delta_time)
	{
		blink_timer += delta_time;
		if (blink_timer >= .5f)
		{
			caret_visible = !caret_visible;
			blink_timer = 0.f;
		}
	}

	void handleMouseDown(int x, int y)
	{
		if (!entity->hasComponent<Transform>()) return;

		auto position = entity->getComponent<Transform>().getPosition();
		auto width = entity->getComponent<Transform>().getWidth();
		auto height = entity->getComponent<Transform>().getHeight();

		focused = (x >= position.x && x <= position.x + width && y >= position.y && y <= position.y + height);
	}
};

#endif