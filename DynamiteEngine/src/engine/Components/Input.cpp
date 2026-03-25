#include "Input.h"
#include <iostream>

void Input::init()
{
	key_state.clear();
	mouse_state.clear();
}

void Input::handleEvents(const SDL_Event& event)
{
	bool was_clicked = false;

	switch (event.type)
	{
	case SDL_KEYDOWN:
		key_state[event.key.keysym.scancode] = true;
		break;

	case SDL_KEYUP:
		key_state[event.key.keysym.scancode] = false;
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (was_clicked) break;
		was_clicked = true;
		mouse_state[event.button.button] = true;
		break;

	case SDL_MOUSEBUTTONUP:
		was_clicked = false;
		mouse_state[event.button.button] = false;
		break;

	case SDL_MOUSEMOTION:
		mouse_position.x = event.motion.x;
		mouse_position.y = event.motion.y;
		break;

	default:
		break;
	}
}

bool Input::isKeyPressed(SDL_Scancode key) const
{
	auto it = key_state.find(key);
	return it != key_state.end() && it->second;
}

bool Input::isMouseButtonPressed(Uint8 button) const
{
	auto it = mouse_state.find(button);
	return it != mouse_state.end() && it->second;
}

SDL_Point Input::getMousePosition() const
{
	return mouse_position;
}