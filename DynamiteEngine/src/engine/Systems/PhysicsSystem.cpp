#include "PhysicsSystem.h"
#include "RigidBody.h"
#include "Transform.h"

void PhysicsSystem::update(Entity& entity, float delta_time)
{
	if (!entity.hasComponent<RigidBody>()) return;
	auto rb = &entity.getComponent<RigidBody>();

	if (!entity.hasComponent<Transform>()) return;
	auto t = &entity.getComponent<Transform>();

	// apply gravity
	t->velocity = rb->velocity;
}