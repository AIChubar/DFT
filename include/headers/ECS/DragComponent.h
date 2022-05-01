#pragma once
#include <iostream>

#include "headers/ECS/Components.h"
#include "headers/Vector2D.h"

class DragComponent : public Component
{
public:

    SpriteComponent *sprite;
    TransformComponent *transform;
    Vector2D relativeMousePos;
    bool spriteMoving = false;
    
    DragComponent() = default;

    void init() override
    {
        sprite = &entity->getComponent<SpriteComponent>();
        transform = &entity->getComponent<TransformComponent>();
    }

    void update() override
    {
        if (Game::event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (Game::event.button.button)
            {
                case SDL_BUTTON_LEFT:
                if(pointOnSprite())
                {
                    spriteMoving = true;
                    Vector2D mousePos = getMousePos();
                    relativeMousePos = mousePos - transform->position;
                }
                default:
                    break;
            }
            
        }
        if (Game::event.type == SDL_MOUSEBUTTONUP)
        {
            switch (Game::event.button.button)
            {
                case SDL_BUTTON_LEFT:
                spriteMoving = false; 
                default:
                break;
            }
        }

        if(spriteMoving)
        {
            Vector2D mousePos = getMousePos();
            transform->position = mousePos - relativeMousePos;
        }
    }

private:
    bool pointOnSprite() const
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        const SDL_Point point = {x, y};
        const SDL_Rect spriteRect = sprite->getRect();
        return SDL_PointInRect(&point, &spriteRect);
    }

    const Vector2D getMousePos() const
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return Vector2D(x, y);
    }

};