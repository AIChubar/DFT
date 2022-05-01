#pragma once

#include <headers/ECS/Components.h>

class SpriteComponent : public Component
{
private:

    TransformComponent* transform;
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
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
        destRect.w = destRect.h = 64;
    } 

    void update() override
    {
        destRect.x = static_cast<int>(transform->x());
        destRect.y = static_cast<int>(transform->y());
        destRect.w = transform->width * transform->scale;
        destRect.h = transform->height * transform->scale;
    }

    void draw() override
    {
        TextureManager::draw(texture, srcRect, destRect);
    }

    void setPose(float x, float y)
    {
        transform->setPose(x, y);
    }

    SDL_Rect getRect()
    {
        return destRect;
    }
};