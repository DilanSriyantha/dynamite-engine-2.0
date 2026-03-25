#ifndef INPUT_H
#define INPUT_H

#include "IComponent.h"
#include "SDL.h"
#include <unordered_map>

class Input : public IComponent {
public:
	Input() = default;
	~Input() override = default;

	void init() override;
	void handleEvents(const SDL_Event& event) override;

	bool isKeyPressed(SDL_Scancode key) const;
	bool isMouseButtonPressed(Uint8 button) const;

	SDL_Point getMousePosition() const;

private:
	std::unordered_map<SDL_Scancode, bool> key_state;
	std::unordered_map<Uint8, bool> mouse_state;
	SDL_Point mouse_position = { 0, 0 };
};

#endif
