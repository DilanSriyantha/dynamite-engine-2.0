#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "IComponent.h"
#include "Vector2D.h"
#include "TransformData.h"

class RigidBody : public IComponent {
public:
	Vector2D velocity{ 0.f, 0.f }, acceleration{ 0.f, 0.f };
	float mass = 1.f, gravity_scale = 1.f, linear_damping = 0.001f;
	bool use_gravity = true, is_grounded = false;

	void addForce(const Vector2D& force);

	void update(float delta_time) override;
};

#endif // !RIGID_BODY_H