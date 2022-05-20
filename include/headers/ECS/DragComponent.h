#pragma once
#include <iostream>
#include <algorithm>

#include "ECS/Components.h"
#include "Vector2D.h"
#include "ECS/TileComponent.h"
#include "ECS/MoneyLabel.h"
#include "ECS/CharacterComponent.h"
#include "Game.h"

class DragComponent : public Component
{
public:

    SpriteComponent* sprite; // Not needed
    TransformComponent* transform;
    Vector2D relativeMousePos;
    Vector2D initialPos;
    bool spriteMoving = false;

    TileComponent* initialTile;
    TileComponent* targetTile;
    
    DragComponent() = default;

    void init() override
    {
        sprite = &entity->getComponent<SpriteComponent>();
        transform = &entity->getComponent<TransformComponent>();
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
                        initialPos = transform->position;
                        initialTile = &Game::getTileByCoord(initialPos.x/64, initialPos.y/64)->getComponent<TileComponent>();
                        std::cout << initialTile->tileStatus <<'\n';
                        spriteMoving = true;
                        Vector2D mousePos = getMousePos();
                        relativeMousePos = mousePos - transform->position;
                    }
                    break;
                default:
                    break;
            }
            
        }
        if (Game::event.type == SDL_MOUSEBUTTONUP)
        {
            switch (Game::event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    if (spriteMoving)
                    {
                        spriteMoving = false;
                        targetTile = &Game::getTileByCoord((transform->position.x + 32)/64, (transform->position.y + 32)/64)->getComponent<TileComponent>();
                        if (transform->position.x > 64*17) //Selling case
                        {
                            entity->getComponent<CharacterComponent>().sell();
                            initialTile->bindedCharIndex = -1;
                        }
                        else if (!canBePlaced() || (!isEmpty() && initialTile->tileStatus == TileStatus::SHOP)) 
                        { // Dragable character can only be in the SHOP or at the PLAYERBOARD
                            transform->position = initialPos;
                        }
                        else
                        {
                            
                            if (!isEmpty()) // Swap case
                            {
                                swap();
                            }
                            else if(initialTile->tileStatus == TileStatus::SHOP) // Buying case
                            {
                                MoneyLabel* monLab = &entity->getGroup(GroupLabels::groupLABELS).at(Labels::MONEYLABEL)->getComponent<MoneyLabel>();
                                CharacterComponent* character = &entity->getComponent<CharacterComponent>();
                                if (!monLab->pay(character->cost))
                                {
                                    transform->position = initialPos;
                                    break;
                                }
                                
                                entity->delGroup(GroupLabels::groupSHOP);
                                entity->delGroup(GroupLabels::groupPLAYERCHARS);
                                entity->addGroup(GroupLabels::groupPLAYERBOARD);
                                targetTile->bindedCharIndex = entity->getGroupSize(GroupLabels::groupPLAYERBOARD) - 1;
                            }
                            else
                            {
                                int initialIndex = initialTile->bindedCharIndex;
                                initialTile->bindedCharIndex = -1;
                                targetTile->bindedCharIndex = initialIndex;
                            }
                            transform->position = targetTile->position;
                        }
                    }
                    break;
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
        Vector2D mousePos = getMousePos();
        const SDL_Point& point = {mousePos.x, mousePos.y};
        const SDL_Rect& spriteRect = sprite->getRect();
        return SDL_PointInRect(&point, &spriteRect);
    }

    bool canBePlaced() const
    {
        return targetTile->tileStatus == TileStatus::PLAYERBOARD;
    }

    void swap()
    {
        int targetIndex = targetTile->bindedCharIndex;

        int initialIndex = initialTile->bindedCharIndex;

        Game::getCharByIndex(GroupLabels::groupPLAYERBOARD, targetIndex)
            ->getComponent<TransformComponent>().setPos(initialPos);

        targetTile->bindedCharIndex = initialIndex;

        initialTile->bindedCharIndex = targetIndex;
    }

    bool isEmpty() const
    {
        return targetTile->bindedCharIndex == -1;
    }

    const Vector2D getMousePos() const
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return Vector2D(x, y);
    }

    

};