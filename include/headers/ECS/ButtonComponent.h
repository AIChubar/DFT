#pragma once
#include <iostream>
#include <utility>
#include <fstream>
#include <string>

#include "ECS/Components.h"
#include "EnumManager.h"
#include "Vector2D.h"
#include "Game.h"


class ButtonComponent : public Component
{
private:
    GameMode gameMode;
    SpriteComponent* sprite;
    UserEvents eventToCall;

public:

    ~ButtonComponent() = default;
    ButtonComponent(UserEvents evnt)
    {
        eventToCall = evnt;
    }

    void init() override
    {
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void handleEvents() override
    {
        if (Game::event.type == SDL_MOUSEBUTTONDOWN)
        {
            switch (Game::event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    if(pointOnSprite())
                    {                    
                        SDL_Event event;
                        SDL_memset(&event, 0, sizeof(event)); 
                        event.type = SDL_USEREVENT;
                        event.user.code = eventToCall;
                        SDL_PushEvent(&event);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    
private:

    bool pointOnSprite() const
    {
        Vector2D mousePos = getMousePos();
        const SDL_Point point = {mousePos.x, mousePos.y};
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