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
    ~SpriteComponent()
    {
        //SDL_DestroyTexture(texture);
    }
    // void setTex(const char* path)
    // {
    //     //texture = TextureManager::loadTexture(path);
    // }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
        destRect.w = transform->width*2;
        destRect.h = transform->height*2;
    } 

    void update() override
    {
        destRect.x = transform->x();
        destRect.y = transform->y();
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
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