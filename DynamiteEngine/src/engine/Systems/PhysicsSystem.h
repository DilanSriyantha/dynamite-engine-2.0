#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "ISystem.h"

class PhysicsSystem : public ISystem {
public:
	void update(Entity& entity, float delta_time) override;
};

#endif