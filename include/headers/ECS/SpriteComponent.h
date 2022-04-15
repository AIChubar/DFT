#pragma once

#include <headers/ECS/Components.h>

class SpriteComponent : public Component
{
private:

    PositionComponent* position;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

public:

    SpriteComponent() = default;
    SpriteComponent(const char* path)
    {
        setTex(path);
    }
    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }
    void setTex(const char* path)
    {
        texture = TextureManager::loadTexture(path);
    }

    void init() override
    {
        position = &entity->getComponent<PositionComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = srcRect.h = 32;
        destRect.w = destRect.h = 64;
    } 

    void update() override
    {
        destRect.x = position->x();
        destRect.y = position->y();
    }

    void draw() override
    {
        TextureManager::draw(texture, srcRect, destRect);
    }
};