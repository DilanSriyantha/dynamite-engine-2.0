#include "AssetManager.h"
#include "Entity.h"
#include "Transform.h"
#include "TextureManager.h"
#include "Game.h"

AssetManager::~AssetManager()
{
	clear();
}

// game object
void AssetManager::createProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string tid)
{
	//auto& projectile(manager->addEntity());
	//projectile.addComponent<Transform>(position.x, position.y, 32, 32, 1);
}

// texture management
void AssetManager::addTexture(std::string id, const char* path)
{
	auto it = textures.find(id);
	if (it != textures.end() && it->second)
		return;

	SDL_Texture* tex = TextureManager::loadTexture(path, Game::getInstance().getRendererPtr());
	if (!tex)
	{
		std::cout << SDL_GetError() << "\n";
		return;
	}
	textures.emplace(id, std::move(tex));
}

SDL_Texture* AssetManager::getTexture(const std::string& id)
{
	SDL_Texture* tex = textures[id];
	if (!tex)
	{
		std::cout << "texture id not found, id: " << id << "\n";
		return nullptr;
	}
	return tex;
}

// font management
void AssetManager::addFont(std::string id, const char* f_path, int f_size)
{
	auto it = fonts.find(id);
	if (it != fonts.end() && it->second)
		return;

	TTF_Font* font = TTF_OpenFont(f_path, f_size);
	fonts.emplace(id, std::move(font));
}

TTF_Font* AssetManager::getFont(std::string id)
{
	TTF_Font* font = fonts[id];
	if (!font)
	{
		std::cout << "font id not found, id: " << id << "\n";
		return nullptr;
	}
	return font;
}

void AssetManager::clear()
{
	// clean textures
	for (auto& t : textures)
	{
		SDL_DestroyTexture(t.second);
		t.second = nullptr;
	}
	textures.clear();

	// clean fonts
	for (auto& f : fonts)
	{
		TTF_CloseFont(f.second);
		f.second = nullptr;
	}
	fonts.clear();
}