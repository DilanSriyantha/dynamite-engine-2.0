#include "RigidBody.h"
#include "Transform.h"
#include "Entity.h"

void RigidBody::addForce(const Vector2D& force)
{
	acceleration += force;
}

void RigidBody::update(float delta_time)
{
	if (use_gravity)
		acceleration.y += 9.18f * gravity_scale;

	// update velocity
	velocity.y += acceleration.y * delta_time;

	// apply damping
	velocity.y *= (1.f - linear_damping);

	// reset acceleration
	acceleration.Zero();
}