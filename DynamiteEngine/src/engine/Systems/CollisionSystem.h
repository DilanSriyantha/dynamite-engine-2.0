#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "ISystem.h"
#include "SDL.h"
#include "BoxCollider.h"
#include <memory>

class CollisionSystem {
public:
	static bool AABB(const SDL_Rect& rect_a, const SDL_Rect& rect_b);
	static bool AABB(const BoxCollider& col_a, const BoxCollider& col_b);

	void handleEvents(Entity& target, const SDL_Event& event);
	void update(Entity& target, std::vector<std::unique_ptr<Entity>>& other_entities, float delta_time);
};

#endif