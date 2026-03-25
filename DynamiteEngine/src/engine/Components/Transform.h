#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "IComponent.h"
#include "Vector2D.h"
#include "TransformData.h"
#include "SDL.h"

class Transform : public IComponent {
public:
	Vector2D velocity;

	float speed = 1.f;

	Transform() = default;
	Transform(float scale);
	Transform(float x, float y);
	Transform(float x, float y, int w, int h, float scale);

	void normalize(float x, float y);
	void normalize(Vector2D transform_vector);
	
	TransformData& getData();

	Vector2D getPosition();
	void setPosition(float x, float y);
	void setPosition(Vector2D position);
	
	int getPositionX();
	void setPositionX(int x);

	int getPositionY();
	void setPositionY(int y);

	int getHeight();
	void setHeight(int height);

	int getWidth();
	void setWidth(int width);

	Vector2D getSize();
	void setSize(int w, int h);
	void setSize(Vector2D size);

	float getScale();
	void setScale(float scale);

	void setVelocity(Vector2D velocity);

	SDL_Rect getTransformRect() const;
	SDL_Rect getScaledTransformRect() const;
	void setTransform(int x, int y, int w, int h, float scale);
	void setTransform(Vector2D position, Vector2D size, float scale);

	void init() override;
	void update(float delta_time) override;

private:
	TransformData td;
};

#endif