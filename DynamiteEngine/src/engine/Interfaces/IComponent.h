#ifndef COMPONENT_H
#define COMPONENT_H

#include "SDL.h"

class Entity;

class IComponent {
public:
	Entity* entity = nullptr;

	virtual void init() {}
	virtual void handleEvents(const SDL_Event& event) {}
	virtual void update(float delta_time) {}
	virtual void updateWhilePause(float delta_time) {}
	virtual void draw(SDL_Rect* viewport) {}

	virtual ~IComponent() = default;
};

#endif