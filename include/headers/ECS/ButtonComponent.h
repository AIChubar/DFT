#pragma once
#include <iostream>
#include <utility>
#include <fstream>
#include <string>

#include "ECS/Components.h"
#include "EnumManager.h"
#include "Vector2D.h"
#include "Game.h"

//
class ButtonComponent : public Component
{
private:
    GameMode gameMode;
    TransformComponent* transform;
    SpriteComponent* sprite;

    UserEvents eventToCall;

    Vector2D position;
    int height;
    int width;
    std::string texID;

public:

    ~ButtonComponent() = default;
    ButtonComponent(Vector2D pos, int h, int w, UserEvents evnt, const std::string& tID)
    {
        eventToCall = evnt;
        position = pos;
        height = h;
        width = w;
        texID = tID;
    }

    void init() override
    {
        transform = &entity->addComponent<TransformComponent>(position, height, width);
        sprite = &entity->addComponent<SpriteComponent>(texID);
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