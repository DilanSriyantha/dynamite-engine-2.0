#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <bitset>
#include <memory>
#include <array>
#include "IComponent.h"

class EntityManager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0;

	return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();

	return typeID;
}

constexpr std::size_t max_components = 32;
constexpr std::size_t max_groups = 32;

using ComponentBitSet = std::bitset<max_components>;
using GroupBitSet = std::bitset<max_groups>;
using ComponentArray = std::array<IComponent*, max_components>;

class Entity {
public:
	Entity(EntityManager& entity_manager)
		: entity_manager(entity_manager) {}

	~Entity();

	virtual void init();
	virtual void handleEvents(const SDL_Event& event);
	virtual void update(float delta_time);
	virtual void updateWhilePause(float delta_time);
	virtual void draw(SDL_Rect* viewport);
	void draw();

	bool isActive();
	void destroy();

	bool hasGroup(Group group);

	void addGroup(Group group);
	void delGroup(Group group);

	template<typename T>
	bool hasComponent() const
	{
		return components_bitset[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... args)
	{
		T* c(new T(std::forward<TArgs>(args)...));
		c->entity = this;
		std::unique_ptr<IComponent> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		components_array[getComponentTypeID<T>()] = c;
		components_bitset[getComponentTypeID<T>()] = true;

		c->init();

		return *c;
	}

	template <typename T> T&
	getComponent() const
	{
		auto ptr(components_array[getComponentTypeID<T>()]);

		if (!ptr) return *static_cast<T*>(nullptr);
		return *static_cast<T*>(ptr);
	}
private:
	EntityManager& entity_manager;

	bool active = true;

	std::vector<std::unique_ptr<IComponent>> components;

	ComponentArray components_array;
	ComponentBitSet components_bitset;
	GroupBitSet group_bitset;
};

#endif