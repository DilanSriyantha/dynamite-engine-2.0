#include "EntityManager.h"
#include "Entity.h"
#include "IComponent.h"
#include "Game.h"
#include "Transform.h"
#include "Input.h"

void EntityManager::init()
{
	for (auto& e : entities)
		e->init();
}

void EntityManager::handleEvents(const SDL_Event& event)
{
	for (auto& e : entities)
	{
		collision_system.handleEvents(*e, event);
		e->handleEvents(event);
	}
}

void EntityManager::update(float delta_time)
{
	for (auto& e : entities)
	{
		physics_system.update(*e, delta_time);
		collision_system.update(*e, entities, delta_time);
		e->update(delta_time);
	}
}

void EntityManager::updateWhilePause(float delta_time)
{
	for (auto& e : entities)
	{
		collision_system.update(*e, entities, delta_time);
		e->updateWhilePause(delta_time);
	}
}

void EntityManager::draw(SDL_Rect* viewport)
{
	for (auto& e : grouped_entities[GRP_SKYBOX])
		e->draw(nullptr);

	for (auto& e : grouped_entities[GRP_MAP_LAYER_6])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_MAP_LAYER_5])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_MAP_LAYER_4])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_MAP_LAYER_3])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_MAP_LAYER_2])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_COLLIDERS])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_PROJECTILES])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_PLAYERS])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_OBJECTS])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_MAP_LAYER_1])
		e->draw(viewport);

	for (auto& e : grouped_entities[GRP_UI_ELEMENTS_LAYER_3])
		e->draw(nullptr);

	for (auto& e : grouped_entities[GRP_UI_ELEMENTS_LAYER_2])
		e->draw(nullptr);

	for (auto& e : grouped_entities[GRP_UI_ELEMENTS_LAYER_1])
		e->draw(nullptr);

	for (auto& e : grouped_entities[GRP_UI_ELEMENTS_LAYER_0])
		e->draw(nullptr);

	for(auto& e : grouped_entities[GRP_UI_ELEMENTS_LAYER_M_1]) // -1
		e->draw(nullptr);
}

void EntityManager::refresh()
{
	for (auto i(0u); i < max_groups; i++)
	{
		auto& v(grouped_entities[i]);
		v.erase(
			std::remove_if(
				std::begin(v), 
				std::end(v),			
				[i](Entity* entity)
				{
					return !entity->isActive() || !entity->hasGroup(i);
				}), 
				std::end(v)
		);
	}

	entities.erase(
		std::remove_if(
			std::begin(entities), 
			std::end(entities), 
			[](const std::unique_ptr<Entity>& entity) 
			{
				return !entity->isActive();
			}), 
			std::end(entities)
	);
}

void EntityManager::addToGroup(Entity* entity, Group group)
{
	grouped_entities[group].emplace_back(entity);
}

std::vector<Entity*>& EntityManager::getGroup(Group group)
{
	return grouped_entities[group];
}

Entity& EntityManager::addEntity()
{
	Entity* e = new Entity(*this);
	std::unique_ptr<Entity> uPtr{ e };
	entities.emplace_back(std::move(uPtr));

	return *e;
}

void EntityManager::removeEntity(Entity& entity)
{
	entity.destroy();
}

EntityList& EntityManager::getEntities()
{
	return entities;
}

GroupedEntityList& EntityManager::getGroupedEntities()
{
	return grouped_entities;
}

void EntityManager::clear()
{
	for (auto& e : entities)
		e->destroy();
}