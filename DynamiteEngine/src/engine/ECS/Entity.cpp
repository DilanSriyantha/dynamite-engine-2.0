#include "Entity.h"
#include "EntityManager.h"

Entity::~Entity()
{
	std::cout << "Entity destroyed\n";
}

void Entity::init()
{
	for (auto& c : components)
		c->init();
}

void Entity::handleEvents(const SDL_Event& event)
{
	for (auto& c : components)
		c->handleEvents(event);
}

void Entity::update(float delta_time)
{
	for (auto& c : components)
		c->update(delta_time);
}

void Entity::updateWhilePause(float delta_time)
{
	for (auto& c : components)
		c->updateWhilePause(delta_time);
}

void Entity::draw(SDL_Rect* viewport)
{
	for (auto& c : components)
		c->draw(viewport);
}

bool Entity::isActive()
{
	return active;
}

void Entity::destroy()
{
	active = false;
}

bool Entity::hasGroup(Group group)
{
	return group_bitset[group];
}

void Entity::addGroup(Group group)
{
	group_bitset[group] = true;
	entity_manager.addToGroup(this, group);
}

void Entity::delGroup(Group group)
{
	group_bitset[group] = false;
}