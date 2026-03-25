#ifndef TRANSFORM_DATA_H
#define TRANSFORM_DATA_H

#include "Vector2D.h"

struct TransformData {
	Vector2D position;
	int width = 0, height = 0;
	float scale = 1;
};

#endif // !TRANSFORM_DATA_H