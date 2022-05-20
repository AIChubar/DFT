#include "AssetManager.h"

AssetManager::AssetManager(Manager* mng) : manager(mng)
{
    
}

AssetManager::~AssetManager()
{

}

void AssetManager::addTexture(const std::string& id, const char* path)
{
    textures.emplace(id, TextureManager::loadTexture(path));
}

SDL_Texture* AssetManager::getTexture(const std::string& id)
{
    return textures.at(id);
}