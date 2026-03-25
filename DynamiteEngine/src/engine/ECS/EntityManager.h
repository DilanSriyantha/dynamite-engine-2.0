#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <array>
#include <memory>
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "SDL.h"

class Entity;
class Component;

using EntityList = std::vector<std::unique_ptr<Entity>>;
using GroupedEntityList = std::array<std::vector<Entity*>, max_groups>;

enum Groups : std::size_t {
	GRP_SKYBOX,
	GRP_MAP_LAYER_1,
	GRP_MAP_LAYER_2,
	GRP_MAP_LAYER_3,
	GRP_MAP_LAYER_4,
	GRP_MAP_LAYER_5,
	GRP_MAP_LAYER_6,
	GRP_COLLIDERS,
	GRP_PROJECTILES,
	GRP_PLAYERS,
	GRP_OBJECTS,
	GRP_UI_ELEMENTS_LAYER_3,
	GRP_UI_ELEMENTS_LAYER_2,
	GRP_UI_ELEMENTS_LAYER_1,
	GRP_UI_ELEMENTS_LAYER_0,
	GRP_UI_ELEMENTS_LAYER_M_1 // -1 (minus one)
};

class EntityManager {
public:
	EntityManager() = default;

	void init();
	void handleEvents(const SDL_Event& event);
	void update(float delta_time);
	void updateWhilePause(float delta_time);
	void draw(SDL_Rect* viewport);
	void refresh();

	void addToGroup(Entity* entity, Group group);
	std::vector<Entity*>& getGroup(Group group);
	Entity& addEntity();
	void removeEntity(Entity& index);
	void clear();
		
	EntityList& getEntities();
	GroupedEntityList& getGroupedEntities();

private:
	EntityList entities;
	GroupedEntityList grouped_entities;
	PhysicsSystem physics_system;
	CollisionSystem collision_system;
};

#endif