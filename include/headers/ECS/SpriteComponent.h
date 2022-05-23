#pragma once

#include "headers/ECS/Components.h"
#include "headers/TextureManager.h"
#include "headers/LevelManager.h"
#include "Game.h"

#include <string>
#include <array>

class SpriteComponent : public Component
{
private:

    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

public:

    SpriteComponent() = default;
    SpriteComponent(const std::string& id)
    {
        texture = Game::assets->getTexture(id);
    }
    SpriteComponent(size_t chr) 
    {
        std::string tID = "char" + std::to_string(chr) + ".png";
        texture = Game::assets->getTexture(tID);
    }
    ~SpriteComponent() //all Textures are destroyed in AssetManager now
    {}


    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->w();
        srcRect.h = transform->h();
        destRect.w = transform->w();
        destRect.h = transform->h();
    } 

    void update() override
    {
        destRect.x = transform->x();
        destRect.y = transform->y();
        destRect.w = transform->w();
        destRect.h = transform->h();
    }

    void draw() override
    {
        TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
    }


    const SDL_Rect& getRect()
    {
        return destRect;
    }
};