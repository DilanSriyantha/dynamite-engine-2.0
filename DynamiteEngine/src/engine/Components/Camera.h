#ifndef CAMERA_H
#define CAMERA_H

#include "Vector2D.h"
#include "Transform.h"

class Camera : public IComponent {
public:
	Camera(int screen_width, int screen_height)
		: viewport{ 0, 0, screen_width, screen_height }, world_bounds{ 0, 0, screen_width * 2, screen_height }
	{}

	void setLerpFactor(float lf)
	{
		lerp_factor = lf;
	}

	void setWorldBounds(int x, int y, int w, int h)
	{
		world_bounds = { x, y, w, h };
	}

	void setFollowOffset(float offset)
	{
		follow_offset = offset;
	}

	void setInfiniteMode(float flag)
	{
		inifinite_mode = flag;
	}

	bool getInfiniteMode() const
	{
		return inifinite_mode;
	}

	SDL_Rect& getViewport()
	{
		return viewport;
	}

	Vector2D& getPosition()
	{
		return position;
	}

	int getHorizontalDirection() const
	{
		return horizontal_direction;
	}

	int getVerticalDirection() const
	{
		return vertical_direction;
	}

	void follow(Transform* t)
	{
		target = t;
	}

	void update(float delta_time) override
	{
		if (!target) return;

		// desired center position based on the target
		float desired_x = target->getPositionX() - viewport.w / 2.f + follow_offset;
		float desired_y = target->getPositionY() - viewport.h / 2.f + follow_offset;

		//std::cout << "target x: " << target->getPositionX() << ", y: " << target->getPositionY() << "\n";

		// temporary values to determine camera moving direction
		float tmp_position_x = position.x;
		float tmp_position_y = position.y;

		// smooth follow using lerp (linear interpolation)
		position.x += (desired_x - position.x) * lerp_factor * delta_time;
		position.y += (desired_y - position.y) * lerp_factor * delta_time;

		// update camera moving direction
		if (position.x > tmp_position_x)
			horizontal_direction = 1; // horizontally positive direction
		else
			horizontal_direction = -1; // horizontally negative direction

		if (position.y > tmp_position_y)
			vertical_direction = 1;
		else
			vertical_direction = -1;

		// clamp camera inside world bounds
		if (position.x < world_bounds.x)
			position.x = world_bounds.x;

		if (position.y < world_bounds.y)
			position.y = world_bounds.y;

		if (!inifinite_mode)
			if (position.x + viewport.w > world_bounds.x + world_bounds.w)
				position.x = world_bounds.x + world_bounds.w - viewport.w;
		

		if (position.y + viewport.h > world_bounds.y + world_bounds.h)
			position.y = world_bounds.y + world_bounds.h - viewport.h;

		// update viewport 
		viewport.x = static_cast<int>(position.x);
		viewport.y = static_cast<int>(position.y);

		//std::cout << "viewport x: " << viewport.x << ", y: " << viewport.y << "\n";
		//std::cout << "camera x: " << position.x << ", y: " << position.y << "\n";
	}
	
private:
	Vector2D position{ 0,0 };
	SDL_Rect viewport;
	SDL_Rect world_bounds;
	Transform* target = nullptr;
	float lerp_factor = 5.f;
	float follow_offset = 800.f;
	int horizontal_direction = 1;
	int vertical_direction = 1;
	bool inifinite_mode = false;
};

#endif