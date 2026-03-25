#include "Transform.h"
#include "Game.h"

Transform::Transform(float scale) 
{
	/* for map scrolling camera angle implementation
	I have to keep the player static in the middle of
	the screen.*/

	td.position.x = 400;
	td.position.y = 320;

	td.scale = scale;
}

Transform::Transform(float x, float y) 
{
	td.position.x = x;
	td.position.y = y;
}

Transform::Transform(float x, float y, int w, int h, float scale) 
{
	td.position.x = x;
	td.position.y = y;
	td.width = w;
	td.height = h;
	td.scale = scale;
}

void Transform::normalize(float x, float y) 
{
	float magnitude = std::sqrt(x * x + y * y);

	// avoid division by zero
	if (magnitude == 0.f) return;

	x /= magnitude;
	y /= magnitude;
}

void Transform::normalize(Vector2D tv) // param tv = transform vector
{
	float magnitude = std::sqrt(tv.x * tv.x + tv.y * tv.y);

	if (magnitude == 0.f) return;

	tv /= magnitude;
}

TransformData& Transform::getData()
{
	return td;
}

Vector2D Transform::getPosition()
{
	return td.position;
}

void Transform::setPosition(float x, float y)
{
	td.position.x = x;
	td.position.y = y;
}

void Transform::setPosition(Vector2D position)
{
	td.position = position;
}

int Transform::getPositionX()
{
	return static_cast<int>(td.position.x);
}

void Transform::setPositionX(int x)
{
	td.position.x = x;
}

int Transform::getPositionY()
{
	return td.position.y;
}

void Transform::setPositionY(int y)
{
	td.position.y = y;
}

int Transform::getHeight() 
{
	return td.height;
}

void Transform::setHeight(int height) 
{
	td.height = height;
}

int Transform::getWidth() 
{
	return td.width;
}

void Transform::setWidth(int width) 
{
	td.width = width;
}

Vector2D Transform::getSize()
{
	return Vector2D{ static_cast<float>(td.width), static_cast<float>(td.height) };
}

void Transform::setSize(int w, int h)
{
	td.width = w;
	td.height = h;
}

void Transform::setSize(Vector2D size)
{
	setSize(size.x, size.y);
}

float Transform::getScale() 
{
	return td.scale;
}

void Transform::setScale(float scale) 
{
	td.scale = scale;
}

void Transform::setVelocity(Vector2D velocity)
{
	this->velocity = velocity;
}

void Transform::setTransform(int x, int y, int w, int h, float scale)
{
	td.position.x = x;
	td.position.y = y;
	td.width = w;
	td.height = h;
	td.scale = scale;
}

void Transform::setTransform(Vector2D position, Vector2D size, float scale)
{
	setTransform(position.x, position.y, size.x, size.y, scale);
}

SDL_Rect Transform::getTransformRect() const
{
	return SDL_Rect{
		static_cast<int>(td.position.x),
		static_cast<int>(td.position.y),
		td.width,
		td.height
	};
}

SDL_Rect Transform::getScaledTransformRect() const
{
	return {
		static_cast<int>(td.position.x),
		static_cast<int>(td.position.y),
		static_cast<int>(ceil(td.width * td.scale)),
		static_cast<int>(ceil(td.height * td.scale)),
	};
}

void Transform::init() 
{
	velocity.Zero();
}

void Transform::update(float delta_time) 
{
	normalize(velocity);

	td.position.x += velocity.x * speed;
	td.position.y += velocity.y * speed;
}