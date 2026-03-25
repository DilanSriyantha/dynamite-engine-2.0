#ifndef PARALLAX_H
#define PARALLAX_H

#include "IComponent.h"

class Parallax : public IComponent {
public:
	float scroll_factor = 1.0f;

	Parallax(float factor = 1.0f)
		: scroll_factor(factor)
	{}
};

#endif