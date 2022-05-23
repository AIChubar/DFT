#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include "SDL_ttf.h"


class AssetManager {

public:
    static std::unique_ptr<AssetManager> assets;

    AssetManager(Manager* manager);
    ~AssetManager();

    void addTexture(const std::string& id, const char* path);
    SDL_Texture* getTexture(const std::string& id);

    void addFont(const std::string& id, const char* path, int fSize);
    TTF_Font* getFont(const std::string& id);

private:

    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, TTF_Font*> fonts;
    Manager* manager;
};
