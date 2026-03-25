#ifndef ASSETS_H
#define ASSETS_H

#include <map>
#include <string>
#include "SDL_image.h"
#include "SDL_ttf.h"

struct Assets {
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
};

#endif