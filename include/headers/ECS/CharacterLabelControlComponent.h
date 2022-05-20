#pragma once
#include <iostream>
#include <utility>
#include <fstream>
#include <string>

#include "ECS/Components.h"
#include "EnumManager.h"
#include "Vector2D.h"


class CharacterLabelControlComponent : public Component
{
private:
    TransformComponent* transform;
    CharacterComponent* character;
    SpriteComponent* sprite;

public:


    ~CharacterLabelControlComponent() = default;
    CharacterLabelControlComponent() = default;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        character = &entity->getComponent<CharacterComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void handleEvents() override
    {
        if (Game::event.type == SDL_MOUSEBUTTONDOWN)
        {
            CharacterLabel* charLab = &entity->getGroup(GroupLabels::groupLABELS).at(Labels::CHARACTERLABEL)->getComponent<CharacterLabel>();
            switch (Game::event.button.button)
            {
                case SDL_BUTTON_RIGHT:
                    if(pointOnSprite())
                    {
                        charLab->activate(character);
                    }
                    else
                    {
                        // Need to improve the idea of CharacterLabel, so when clicking on an empty space, label is not active;
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