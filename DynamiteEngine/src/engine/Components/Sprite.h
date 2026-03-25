#ifndef SPRITE_H
#define SPRITE_H

#include "IComponent.h"
#include "TransformData.h"
#include "SDL_image.h"
#include <map>
#include "Game.h"

/// <summary>
/// row -> which row (y index) in the sprite sheet
/// frames -> how many frames in this animation
/// frame_duration -> duration of each frame (seconds)
/// loop -> true, if your animation needs to be iterative and false, if your animation needs to play once
/// </summary>
struct Animation {
	int row;
	int frames;
	float frame_duration;
	bool loop;
};

using AnimationList = std::map<std::string, Animation>;

class Sprite : public IComponent {
public:
	Sprite() = default;
	Sprite(SDL_Texture* texture);
	Sprite(SDL_Texture* texture, bool animated);
	~Sprite();

	void setTexture(SDL_Texture* texture);
	void setTexture(SDL_Texture* texture, bool animated);
	void setTransformData(TransformData& td);
	void setFlip(bool flip);
	void setVisible(bool flag);
	bool getVisible();
	void setParallaxFactor(float factor);
	float getParallaxFactor();

	void addAnimation(const std::string& name, int row, int frame, float frame_duration, bool loop);
	void addAnimation(const std::string& name, int row, int frame, float frame_duration);
	void removeAnimation(const std::string& name);
	void play(const std::string& name);
	void replay(const std::string& name);
	
	void init() override;
	void draw(SDL_Rect* viewport) override;
	void update(float delta_time) override;

	bool isAnimationFinished() const;
	bool isAnimationPlaying(const std::string& animation) const;
	int getCurrentAnimationFrame() const;

private:
	SDL_Texture* texture = nullptr;
	TransformData* td;

	SDL_Rect srcRect{}, destRect{};
	bool animated = false;

	AnimationList animations;
	std::string current_anim;

	int current_frame = 0;
	int totel_frames = 1;
	int anim_index = 0;

	float frame_duration = .1f;
	float frame_time = .0f;
	float parallax_factor = 1.f;

	bool loop_anim = false;
	bool visible = true;

	// flip sprites horizontally, vertically
	SDL_RendererFlip sprite_flip = SDL_FLIP_NONE;
};

#endif // !Sprite