#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "SDL_image.h"

class TextureManager {
public:
	static SDL_Texture* loadTexture(const char* file_name, SDL_Renderer* renderer)
	{
		SDL_Surface* tmp_surface = IMG_Load(file_name);
		if (!tmp_surface)
		{
			std::cerr << "error: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
		if (!texture)
		{
			std::cerr << "error: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		SDL_FreeSurface(tmp_surface);

		return texture;
	}

	static SDL_Texture* loadTextureFromBuffer(void* mem, size_t size, SDL_Renderer* renderer)
	{
		SDL_RWops* rwops = SDL_RWFromMem(mem, size);
		if (!rwops)
		{
			std::cerr << "SDL_RWFromMem error: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		SDL_Surface* surface = IMG_Load_RW(rwops, 1); // 1 closes the RWops after loading - according to the SDL2 documentation.
		if (!surface)
		{
			std::cerr << "IMG_Load_RW error: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (!texture)
		{
			std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
			return nullptr;
		}

		SDL_FreeSurface(surface);

		return texture;
	}

	static void draw(SDL_Texture* texture, SDL_Rect* src_rect, SDL_Rect* dest_rect, SDL_RendererFlip flip, SDL_Renderer* renderer)
	{
		SDL_RenderCopyEx(renderer, texture, src_rect, dest_rect, NULL, NULL, flip);
	}
};

#endif