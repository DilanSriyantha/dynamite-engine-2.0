#include "CollisionSystem.h"
#include <iostream>
#include "RigidBody.h"
#include "Transform.h"
#include "Input.h"
#include "Game.h"
#include "EventTypes.h"

bool CollisionSystem::AABB(const SDL_Rect& rect_a, const SDL_Rect& rect_b)
{
	if (
		rect_a.x < rect_b.x + rect_b.w &&
		rect_a.x + rect_a.w > rect_b.x &&
		rect_a.y < rect_b.y + rect_b.h &&
		rect_a.y + rect_a.h > rect_b.y
		)
		return true;
	return false;
}

bool CollisionSystem::AABB(const BoxCollider& col_a, const BoxCollider& col_b)
{
	if (AABB(col_a.collider, col_b.collider))
		return true;
	return false;
}

void CollisionSystem::handleEvents(Entity& target, const SDL_Event& event)
{
	if (!target.hasComponent<BoxCollider>()) return;

	auto& target_collider = target.getComponent<BoxCollider>();

	// check whether the target collider is a trigger and
	// compare mouse location to figure out whether 
	// the collider is triggered or not
	if (target.hasComponent<Input>() && target_collider.isTrigger())
	{
		auto& input = target.getComponent<Input>();
		auto mouse = input.getMousePosition();
		SDL_Rect mouse_pos = { mouse.x, mouse.y, 1, 1 };

		if (input.isMouseButtonPressed(SDL_BUTTON_LEFT))
		{
			if (AABB(target_collider.collider, mouse_pos))
			{
				if (target_collider.hit) return;

				target_collider.hit = true;
				Game::getInstance().getEventBus().emit(MouseLeftClickEvent{mouse});
			}
		}
		else
		{
			target_collider.hit = false;
		}
	}
}

void CollisionSystem::update(Entity& target, std::vector<std::unique_ptr<Entity>>& other_entities, float delta_time)
{
	if (target.hasComponent<BoxCollider>())
	{
		auto& target_collider = target.getComponent<BoxCollider>();

		for (auto& e : other_entities)
		{
			if (e.get() == &target) continue;

			if (e->hasComponent<BoxCollider>())
			{
				auto& other_collider = e->getComponent<BoxCollider>();
				
				if (AABB(target_collider, other_collider))
				{
					if (target_collider.getTag() == COL_PLAYER && other_collider.getTag() == COL_ENEMY)
					{
						target_collider.hit = true;
						other_collider.hit = true;
						//std::cout << "player and enemy colliders are hit\n";
					}

					if (target_collider.isTrigger()) continue;

					if (target.hasComponent<RigidBody>())
					{
						auto& rb = target.getComponent<RigidBody>();

						// apply collision response 
						// !!! HAVE TO ENHANCE THIS IMPLEMENTATION !!!
						if (target.hasComponent<Transform>())
						{
							auto& td = target.getComponent<Transform>().getData();

							// calculate peneration depth
							float penetration = (target_collider.collider.y + target_collider.collider.h) - other_collider.collider.y;

							// clip position by peneration amount
							td.position.y -= penetration;

							// apply bounce
							const float restitution = .6f;
							const float damping_factor = .85f;

							rb.velocity.y = -rb.velocity.y * restitution;

							if (fabs(rb.velocity.y) < .5f)
							{
								rb.velocity.Zero();
								rb.acceleration.Zero();
								rb.use_gravity = false;
							}
							else
							{
								rb.use_gravity = true;
							}

							//std::cout << "velocity: " << rb.velocity << "\n";
						}
					}
				}
			}
		}
	}
}