#ifndef ICOMPOSITE_SCENE_OBJECT
#define ICOMPOSITE_SCENE_OBJECT

#include "IScene.h"

/// <summary>
/// ICompositeSceneObejct is an interface that I use to create complex objects in a scene such as 
/// Player, Enemy or HUD. Each object takes IScene& into the constructor and will use the 
/// reference while modeling the desired object. This engine treats every object as an Entity 
/// which is having multiple IComponents to operate. Thus I am using this interface to model
/// complex objects which requires more than one entity to operate or compromise overall 
/// readability and clarity of the code in an scene.
/// </summary>
class ICompositeSceneObject {
public:
	ICompositeSceneObject(IScene& scene)
		: scene(scene) {}

	virtual ~ICompositeSceneObject() = default;

	virtual void init() {}
	virtual void update(float delta_time) {}
	virtual void draw() {}

protected:
	IScene& scene;
};

#endif