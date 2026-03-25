#ifndef SCRIPT_H
#define SCRIPT_H

#include "IComponent.h"

class Script : public IComponent {
public:
	Entity* entity = nullptr;

	Script() = default;
	virtual ~Script() = default;

	void init() override {};
	void update(float delta_time) override {}
	void draw(SDL_Rect* viewport) override {}
};

#endif