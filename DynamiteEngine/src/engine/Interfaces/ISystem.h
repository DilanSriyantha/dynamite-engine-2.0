#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "Entity.h"

class ISystem {
public:
	virtual void update(Entity& entity, float delta_time) {};
};

#endif