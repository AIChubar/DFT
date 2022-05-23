#pragma once
#include <iostream>
#include <utility>
#include <fstream>

#include "ECS/TransformComponent.h"

#include "EnumManager.h"
#include "Vector2D.h"
#include "Game.h"
using GameUnit = size_t;

// TBD add user events to control characters

class CharacterFrameComponent : public Component
{
private:

    bool isActive;
    SDL_Rect srcRect, destRect;
    SDL_Texture* texture;

    TransformComponent* transform;
public:
    ~CharacterFrameComponent() = default;
    CharacterFrameComponent() = default;

    CharacterFrameComponent(const std::string id)
    {
        texture = Game::assets->getTexture(id);
        isActive = false;
    }

    void activate(bool isAttacked)
    {
        transform = &entity->getComponent<TransformComponent>();
        srcRect.x = srcRect.y = 0;
        srcRect.w = transform->w();
        srcRect.h = transform->h();
        destRect.w = transform->w();
        destRect.h = transform->h();
        destRect.x = transform->x();
        destRect.y = transform->y();

        isActive = true;
        if (isAttacked)
        {
            srcRect.x = 64;
        }
        else
        {
            srcRect.x = 0;
        }
    }

    void deactivate()
    {
        isActive = false;
    }

    void draw() override
    {
        if (isActive)
        {
            TextureManager::draw(texture, srcRect, destRect, SDL_FLIP_NONE);
        }
    }

};