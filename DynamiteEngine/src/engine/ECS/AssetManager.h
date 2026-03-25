#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <map>
#include <string>
#include <memory>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "EntityManager.h"
#include "Vector2D.h"

class AssetManager {
public:
	AssetManager() = default;
	~AssetManager();

	// game object
	void createProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string tid);

	// texture management
	void addTexture(std::string id, const char* path);
	SDL_Texture* getTexture(const std::string& id);

	// font management
	void addFont(std::string id, const char* f_path, int f_size);
	TTF_Font* getFont(std::string id);

	void clear();

private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};

#endif // !ASSET_MANAGER_H