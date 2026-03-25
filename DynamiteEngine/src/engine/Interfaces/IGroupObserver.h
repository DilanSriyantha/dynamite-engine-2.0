#ifndef IGROUP_OBSERVER_H
#define IGROUP_OBSERVER_H

#include "Utils.h"

class Entity;

class IGroupObserver{
public:
	virtual void onGroupAdded(Entity* entity, Group group) = 0;
	virtual void onGroupRemoved(Entity* entity, Group group) = 0;
};

#endif