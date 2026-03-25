#ifndef COLLIDER_TAGS_H
#define COLLIDER_TAGS_H

#include <string>

enum ColliderTag : std::size_t {
	COL_TERRAIN,
	COL_OBJECT,
	COL_PLAYER,
	COL_ENEMY
};

#endif