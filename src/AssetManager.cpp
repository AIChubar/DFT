#include "AssetManager.h"

AssetManager::AssetManager(Manager* mng) : manager(mng)
{
    
}

AssetManager::~AssetManager()
{
    for (auto& tex : textures)
    {
        SDL_DestroyTexture(tex.second);
    }
    for (auto& font : fonts)
    {
        TTF_CloseFont(font.second);
    }
    delete manager;
}

void AssetManager::addTexture(const std::string& id, const char* path)
{
    textures.emplace(id, TextureManager::loadTexture(path));
}

SDL_Texture* AssetManager::getTexture(const std::string& id)
{
    return textures.at(id);
}

void AssetManager::addFont(const std::string& id, const char* path, int fSize)
{
    fonts.emplace(id, TTF_OpenFont(path, fSize));
}

TTF_Font* AssetManager::getFont(const std::string& id)
{
    return fonts.at(id);
}