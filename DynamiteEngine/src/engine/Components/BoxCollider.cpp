#include "BoxCollider.h"
#include "Entity.h"
#include "Transform.h"
#include "TextureManager.h"
#include "Game.h"
#include "Sprite.h"

BoxCollider::BoxCollider(std::size_t t) : 
	tag(t), id(rand())
{
	collider = { 0, 0,0,0 };
	gizmo_rect = { 0,0,0,0 };
}

BoxCollider::BoxCollider(std::size_t t, bool trigger) :
	tag(t), id(rand()), trigger(trigger)
{
	collider = { 0, 0,0,0 };
	gizmo_rect = { 0,0,0,0 };
}

BoxCollider::BoxCollider(std::size_t t, bool trigger, bool show_gizmo) :
	tag(t), id(rand()), trigger(trigger), gizmo_visible(show_gizmo)
{
	collider = { 0, 0,0,0 };
	gizmo_rect = { 0,0,0,0 };
}

BoxCollider::BoxCollider(std::size_t t, int x, int y, int w, int h) :
	tag(t), id(rand())
{
	collider = { x, y, w, h };
	gizmo_rect = { x, y, w, h };
}

BoxCollider::BoxCollider(std::size_t t, int x, int y, int w, int h, bool show_gizmo) :
	tag(t), id(rand()), gizmo_visible(show_gizmo)
{
	collider = { x, y, w, h };
	gizmo_rect = { x, y, w, h };
}

BoxCollider::BoxCollider(std::size_t t, int x, int y, int w, int h, bool show_gizmo, bool trigger) :
	tag(t), id(rand()), gizmo_visible(show_gizmo), trigger(trigger)
{
	collider = { x, y, w, h };
	gizmo_rect = { x, y, w, h };
}

void BoxCollider::setTag(std::size_t t)
{
	tag = t;
}

std::size_t BoxCollider::getTag()
{
	return tag;
}

void BoxCollider::setPosition(int x, int y)
{
	collider.x = x;
	collider.y = y;

	gizmo_rect.x = x;
	gizmo_rect.y = y;
}

Vector2D BoxCollider::getPosition()
{
	return { static_cast<float>(collider.x), static_cast<float>(collider.y) };
}

void BoxCollider::setWidth(int w)
{
	collider.w = w;

	gizmo_rect.w = w;
}

void BoxCollider::setHeight(int h)
{
	collider.h = h;

	gizmo_rect.h = h;
}

void BoxCollider::setDimensions(int w, int h)
{
	collider.w = w;
	collider.h = h;

	gizmo_rect.w = w;
	gizmo_rect.h = h;
}

void BoxCollider::setBoundaries(int x, int y, int w, int h)
{
	collider.x = x;
	collider.y = y;
	collider.w = w;
	collider.h = h;

	gizmo_rect.x = x;
	gizmo_rect.y = y;
	gizmo_rect.w = w;
	gizmo_rect.h = h;
}

void BoxCollider::showGizmo()
{
	gizmo_visible = true;
}

void BoxCollider::hideGizmo()
{
	gizmo_visible = false;
}

bool BoxCollider::isTrigger()
{
	return trigger;
}

void BoxCollider::init()
{
	if (!gizmo_visible) return;

	gizmo_tex = TextureManager::loadTexture("Assets/tile_collider.png", Game::getInstance().getRendererPtr());

	auto& sprite = entity->addComponent<Sprite>();
	sprite.setTexture(gizmo_tex);
}

void BoxCollider::update(float delta_time)
{
	if (tag != COL_TERRAIN)
	{
		if (!entity->hasComponent<Transform>()) return;
		auto& td = entity->getComponent<Transform>().getData();

		collider.x = static_cast<int>(td.position.x);
		collider.y = static_cast<int>(td.position.y);
		collider.w = td.width * td.scale;
		collider.h = td.height * td.scale;
	}

	gizmo_rect.x = collider.x;
	gizmo_rect.y = collider.y;
	gizmo_rect.w = collider.w;
	gizmo_rect.h = collider.h;
}

void BoxCollider::draw(SDL_Rect* viewport)
{
	//if (!gizmo_visible) return;

	//TextureManager::draw(gizmo_tex, &gizmo_src_rect, &gizmo_rect, SDL_FLIP_NONE, Game::getInstance().getRendererPtr());
}