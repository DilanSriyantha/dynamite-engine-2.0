#include "Sprite.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Parallax.h"

Sprite::Sprite(SDL_Texture* texture)
{
	setTexture(texture);
}

Sprite::~Sprite()
{
}

Sprite::Sprite(SDL_Texture* texture, bool animated)
{
	setTexture(texture);
	this->animated = animated;
}

void Sprite::setTexture(SDL_Texture* texture)
{
	this->texture = texture;
}

void Sprite::setTexture(SDL_Texture* texture, bool animated)
{
	this->texture = texture;
	this->animated = animated;
}

void Sprite::setTransformData(TransformData& td) 
{
	this->td = &td;
}

void Sprite::setFlip(bool flip)
{
	sprite_flip = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
}

void Sprite::setVisible(bool flag)
{
	visible = flag;
}

bool Sprite::getVisible()
{
	return visible;
}

void Sprite::setParallaxFactor(float factor)
{
	parallax_factor = factor;
}

float Sprite::getParallaxFactor()
{
	return parallax_factor;
}

void Sprite::addAnimation(const std::string& name, int row, int frames, float frame_duration, bool loop)
{
	animations[name] = { row, frames, frame_duration, loop };
}

void Sprite::addAnimation(const std::string& name, int row, int frames, float frame_duration)
{
	animations[name] = { row, frames, frame_duration, true };
}

void Sprite::removeAnimation(const std::string& name)
{
	animations.erase(name);
}

void Sprite::play(const std::string& name)
{
	if (current_anim == name) return;

	current_anim = name;
	const auto& anim = animations[name];
	anim_index = anim.row;
	totel_frames = anim.frames;
	frame_duration = anim.frame_duration;
	current_frame = 0;
	frame_time = .0f;
	loop_anim = anim.loop;
}

void Sprite::replay(const std::string& name)
{
	if (isAnimationPlaying(name)) return;

	current_anim = name;
	const auto& anim = animations[name];
	anim_index = anim.row;
	totel_frames = anim.frames;
	frame_duration = anim.frame_duration;
	current_frame = 0;
	frame_time = .0f;
	loop_anim = anim.loop;
}

void Sprite::init()
{
	setTransformData(entity->getComponent<Transform>().getData());

	srcRect.x = srcRect.y = 0;
	srcRect.w = td->width;
	srcRect.h = td->height;

	destRect.w = td->width * td->scale;
	destRect.h = td->height * td->scale;
}

void Sprite::update(float delta_time)
{
	if (animated)
	{
		if (!loop_anim && isAnimationFinished()) return;

		frame_time += delta_time;
		if (frame_time >= frame_duration)
		{
			frame_time = .0f;
			current_frame = (current_frame + 1) % totel_frames;
		}

		const auto& anim = animations[current_anim];
		srcRect.x = current_frame * td->width;
		srcRect.y = anim.row * td->height;
		srcRect.w = td->width;
		srcRect.h = td->height;

		destRect.x = static_cast<int>(td->position.x);
		destRect.y = static_cast<int>(td->position.y);
		destRect.w = td->width * td->scale;
		destRect.h = td->height * td->scale;
	}

	srcRect.y = anim_index * td->height;
	
	// destRect.x = static_cast<int>(td->position.x) - camera->x;
	// destRect.y = static_cast<int>(td->position.y) - camera->y;

	srcRect.w = td->width;
	srcRect.h = td->height;

	destRect.x = static_cast<int>(td->position.x);
	destRect.y = static_cast<int>(td->position.y);

	destRect.w = td->width * td->scale;
	destRect.h = td->height * td->scale;
}

void Sprite::draw(SDL_Rect* viewport)
{
	if (!visible) return;

	if (viewport)
	{
		if (!entity->hasComponent<Transform>()) return;

		destRect.x = static_cast<int>(entity->getComponent<Transform>().getPositionX() - viewport->x * parallax_factor);
		destRect.y = static_cast<int>(entity->getComponent<Transform>().getPositionY() - viewport->y * parallax_factor);
	}

	TextureManager::draw(texture, &srcRect, &destRect, sprite_flip, Game::getInstance().getRendererPtr());
}

bool Sprite::isAnimationFinished() const
{
	return !loop_anim && current_frame >= totel_frames - 1;
}

bool Sprite::isAnimationPlaying(const std::string& animation) const
{
	return current_anim._Equal(animation) && !isAnimationFinished();
}

int Sprite::getCurrentAnimationFrame() const
{
	return current_frame;
}