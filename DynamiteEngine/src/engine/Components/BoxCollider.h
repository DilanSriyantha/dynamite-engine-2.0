#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "IComponent.h"
#include "SDL.h"
#include "ColliderTags.h"
#include "Vector2D.h"

class BoxCollider : public IComponent {
public:
	SDL_Rect collider;
	bool hit = false;

	BoxCollider() = default;
	BoxCollider(std::size_t t);
	BoxCollider(std::size_t t, bool trigger);
	BoxCollider(std::size_t t, bool trigger, bool show_gizmo);
	BoxCollider(std::size_t t, int x, int y, int w, int h);
	BoxCollider(std::size_t t, int x, int y, int w, int h, bool show_gizmo);
	BoxCollider(std::size_t t, int x, int y, int w, int h, bool show_gizmo, bool trigger);

	void setTag(std::size_t t);
	std::size_t getTag();
	void setPosition(int x, int y);
	Vector2D getPosition();
	void setWidth(int w);
	void setHeight(int h);
	void setDimensions(int w, int h);
	void setBoundaries(int x, int y, int w, int h);
	void showGizmo();
	void hideGizmo();
	bool isTrigger();

	void init() override;
	void update(float delta_time) override;
	void draw(SDL_Rect* viewport) override;

private:
	int id;
	std::size_t tag;
	SDL_Rect gizmo_src_rect{ 0,0,32,32 }, gizmo_rect;
	SDL_Texture* gizmo_tex = nullptr;
	bool gizmo_visible = false;
	bool trigger = false;
};

#endif