#ifndef IPOSITION_CONTROLLER_H
#define IPOSITION_CONTROLLER_H

#include "Transform.h"
#include "Entity.h"
#include "SDL_rect.h"

template<typename T, typename = void>
struct has_entity_member : std::false_type {};

template<typename T>
struct has_entity_member<T, std::void_t<decltype(std::declval<T>().entity)>> : std::true_type {};

template<typename T>
constexpr bool has_entity_member_value = has_entity_member<T>::value;

template<typename T, typename R>
class IPositionController {
public:
	virtual R& centerHorizontal(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return centerHorizontal(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& centerVertical(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return centerVertical(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& center(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return center(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& dockOnTop(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return dockOnTop(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& dockOnTopCenter(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return dockOnTopCenter(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& dockOnTopLeft(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return dockOnTopLeft(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& dockOnTopRight(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return dockOnTopRight(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& dockOnBottomCenter(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return dockOnBottomCenter(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& dockOnBottomLeft(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return dockOnBottomLeft(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& dockOnBottomRight(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return dockOnBottomRight(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& dockBelow(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return dockBelow(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& dockAbove(T& type)
	{
		if constexpr (has_entity_member_value<T>)
		{
			Entity* e = type.entity;
			if (e != nullptr)
				return dockAbove(e);
		}
		return static_cast<R&>(*this);
	}

	virtual R& centerHorizontal(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return centerHorizontal(entity->getComponent<Transform>());
	}

	virtual R& centerHorizontal(Transform& parent_transform)
	{
		int center_x = parent_transform.getPositionX() + (parent_transform.getWidth() * parent_transform.getScale() - getLocalTransform().getWidth() * getLocalTransform().getScale()) / 2;
		getLocalTransform().setPositionX(center_x);

		return static_cast<R&>(*this);
	}

	virtual R& centerHorizontal(SDL_Rect* rect)
	{
		int center_x = getLocalTransform().getPositionX() + (getLocalTransform().getWidth() * getLocalTransform().getScale() - rect->w) / 2;
		rect->w = center_x;

		return static_cast<R&>(*this);
	}

	virtual R& centerVertical(Entity* entity)
	{
		if (!entity->hasComponent<Transform>())  return static_cast<R&>(*this);;
		return centerVertical(entity->getComponent<Transform>());
	}

	virtual R& centerVertical(Transform& parent_transform)
	{
		int center_y = parent_transform.getPositionY() + (parent_transform.getHeight() * parent_transform.getScale() - getLocalTransform().getHeight() * getLocalTransform().getScale()) / 2;
		getLocalTransform().setPositionY(center_y);

		return static_cast<R&>(*this);
	}

	virtual R& centerVertical(SDL_Rect* rect)
	{
		int center_y = getLocalTransform().getPositionY() * getLocalTransform().getScale() + (getLocalTransform().getHeight() * getLocalTransform().getScale() - rect->h) / 2;
		rect->y = center_y;

		return static_cast<R&>(*this);
	}

	virtual R& center(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return center(entity->getComponent<Transform>());
	}

	virtual R& center(Transform& parent_transform)
	{
		int center_x = parent_transform.getPositionX() + (parent_transform.getWidth() * parent_transform.getScale() - getLocalTransform().getWidth() * getLocalTransform().getScale()) / 2;
		int center_y = parent_transform.getPositionY() + (parent_transform.getHeight() * parent_transform.getScale() - getLocalTransform().getHeight() * getLocalTransform().getScale()) / 2;

		getLocalTransform().setPosition(center_x, center_y);

		return static_cast<R&>(*this);
	}

	virtual R& center(SDL_Rect* rect)
	{
		int center_x = getLocalTransform().getPositionX() + (getLocalTransform().getWidth() * getLocalTransform().getScale() - rect->w) / 2;
		int center_y = getLocalTransform().getPositionY() + (getLocalTransform().getHeight() * getLocalTransform().getScale() - rect->h) / 2;
		rect->x = center_x;
		rect->y = center_y;

		return static_cast<R&>(*this);
	}

	virtual R& center(const SDL_Rect& parent_rect)
	{
		int center_x = parent_rect.x + (parent_rect.w - getLocalTransform().getWidth() * getLocalTransform().getScale()) / 2;
		int center_y = parent_rect.y + (parent_rect.h - getLocalTransform().getHeight() * getLocalTransform().getScale()) / 2;
		
		std::cout << "c_x: " << center_x << ", c_y: " << center_y << "\n";

		getLocalTransform().setPosition(center_x, center_y);

		return static_cast<R&>(*this);
	}

	virtual R& dockOnTop(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return dockOnTop(entity->getComponent<Transform>());
	}

	virtual R& dockOnTop(Transform& parent_transform)
	{
		getLocalTransform().setPositionY(parent_transform.getPositionY());

		return static_cast<R&>(*this);
	}

	virtual R& dockOnTop(SDL_Rect* rect)
	{
		rect->y = getLocalTransform().getPositionY();

		return static_cast<R&>(*this);
	}

	virtual R& dockOnTopCenter(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return dockOnTopCenter(entity->getComponent<Transform>());
	}

	virtual R& dockOnTopCenter(Transform& parent_transform)
	{
		getLocalTransform().setPositionY(parent_transform.getPositionY());
		return centerHorizontal(parent_transform);
	}

	virtual R& dockOnTopCenter(SDL_Rect* rect)
	{
		rect->y = getLocalTransform().getPositionY();
		return centerHorizontal(rect);
	}

	virtual R& dockOnTopLeft(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return dockOnTopLeft(entity->getComponent<Transform>());
	}

	virtual R& dockOnTopLeft(Transform& parent_transform)
	{
		getLocalTransform().setPositionY(parent_transform.getPositionY());
		getLocalTransform().setPositionX(parent_transform.getPositionX());

		return static_cast<R&>(*this);
	}

	virtual R& dockOnTopLeft(SDL_Rect* rect)
	{
		rect->y = getLocalTransform().getPositionY();
		rect->x = getLocalTransform().getPositionX();

		return static_cast<R&>(*this);
	}

	virtual R& dockOnTopRight(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return dockOnTopRight(entity->getComponent<Transform>());
	}

	virtual R& dockOnTopRight(Transform& parent_transform)
	{
		getLocalTransform().setPositionY(parent_transform.getPositionY());
		getLocalTransform().setPositionX(parent_transform.getWidth() * parent_transform.getScale() - getLocalTransform().getWidth() * getLocalTransform().getScale() + parent_transform.getPositionX());

		return static_cast<R&>(*this);
	}

	virtual R& dockOnTopRight(SDL_Rect* rect)
	{
		rect->y = getLocalTransform().getPositionY();
		rect->x = getLocalTransform().getWidth() - rect->w + getLocalTransform().getPositionX();

		return static_cast<R&>(*this);
	}

	virtual R& dockOnBottomCenter(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return dockOnBottomCenter(entity->getComponent<Transform>());
	}

	virtual R& dockOnBottomCenter(Transform& parent_transform)
	{
		getLocalTransform().setPositionY(parent_transform.getHeight() * parent_transform.getScale() - getLocalTransform().getHeight() * getLocalTransform().getScale() + parent_transform.getPositionY());
		return centerHorizontal(parent_transform);
	}

	virtual R& dockOnBottomCenter(SDL_Rect* rect)
	{
		rect->y = getLocalTransform().getHeight() * getLocalTransform().getScale() - rect->h + getLocalTransform().getPositionY();
		return centerHorizontal(rect);
	}

	virtual R& dockOnBottomLeft(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return dockOnBottomLeft(entity->getComponent<Transform>());
	}

	virtual R& dockOnBottomLeft(Transform& parent_transform)
	{
		getLocalTransform().setPositionY(parent_transform.getHeight() * parent_transform.getScale() - getLocalTransform().getHeight() * getLocalTransform().getScale() + parent_transform.getPositionY());
		getLocalTransform().setPositionX(parent_transform.getPositionX());

		return static_cast<R&>(*this);
	}

	virtual R& dockOnBottomLeft(SDL_Rect* rect)
	{
		rect->y = getLocalTransform().getHeight() * getLocalTransform().getScale() - rect->h + getLocalTransform().getPositionY();
		rect->x = getLocalTransform().getPositionX();

		return static_cast<R&>(*this);
	}

	virtual R& dockOnBottomRight(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return dockOnBottomRight(entity->getComponent<Transform>());
	}

	virtual R& dockOnBottomRight(Transform& parent_transform)
	{
		getLocalTransform().setPositionY(parent_transform.getHeight() * parent_transform.getScale() - getLocalTransform().getHeight() * getLocalTransform().getScale() + parent_transform.getPositionY());
		getLocalTransform().setPositionX(parent_transform.getPositionX() + parent_transform.getWidth() * parent_transform.getScale() - getLocalTransform().getWidth() * getLocalTransform().getScale());

		return static_cast<R&>(*this);
	}

	virtual R& dockOnBottomRight(SDL_Rect* rect)
	{
		rect->y = getLocalTransform().getHeight() * getLocalTransform().getScale() - rect->h + getLocalTransform().getPositionY();
		rect->x = getLocalTransform().getPositionX() + getLocalTransform().getWidth() *getLocalTransform().getScale() - rect->w;

		return static_cast<R&>(*this);
	}

	virtual R& dockBelow(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return dockBelow(entity->getComponent<Transform>());
	}

	virtual R& dockBelow(Transform& parent_transform)
	{
		getLocalTransform().setPositionY(parent_transform.getPositionY() + parent_transform.getHeight() * parent_transform.getScale());

		return static_cast<R&>(*this);
	}

	virtual R& dockBelow(SDL_Rect* rect)
	{
		rect->y = getLocalTransform().getPositionY() + getLocalTransform().getHeight() * getLocalTransform().getScale();
		
		return static_cast<R&>(*this);
	}

	virtual R& dockAbove(Entity* entity)
	{
		if (!entity->hasComponent<Transform>()) return static_cast<R&>(*this);;
		return dockAbove(entity->getComponent<Transform>());
	}

	virtual R& dockAbove(Transform& parent_transform)
	{
		getLocalTransform().setPositionY(parent_transform.getPositionY() - getLocalTransform().getHeight() * getLocalTransform().getScale());

		return static_cast<R&>(*this);
	}

	virtual R& dockAbove(SDL_Rect* rect)
	{
		rect->y = getLocalTransform().getPositionY() - rect->h;

		return static_cast<R&>(*this);
	}

	virtual R& paddingTop(const int& val)
	{
		getLocalTransform().setPositionY(getLocalTransform().getPositionY() + val);
		return static_cast<R&>(*this);
	}

	virtual R& paddingLeft(const int& val)
	{
		getLocalTransform().setPositionX(getLocalTransform().getPositionX() + val);
		return static_cast<R&>(*this);
	}

	protected:
		virtual Transform& getLocalTransform() const = 0;
};

#endif