#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include "SDL_ttf.h"

//TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);

//SDL_Color White = {255, 255, 255};

class AssetManager {

public:
    AssetManager(Manager* manager);
    ~AssetManager();

    void addTexture(const std::string& id, const char* path);
    SDL_Texture* getTexture(const std::string& id);

private:

    std::map<std::string, SDL_Texture*> textures;
    Manager* manager;
};
