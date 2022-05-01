#pragma once

#include "headers/ECS/Components.h"
#include "headers/ECS/ECS.h"
//#include "headers/"

class TileComponent : public Component
{
    public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    SDL_Rect tileRect;

    int tileID;
    const char* path;
    TileComponent() = default;

    TileComponent(int x, int y, int w, int h, int id)
    {
        tileRect.x = x;
        tileRect.y = y;
        tileRect.w = w;
        tileRect.h = h;
        tileID = id;

        switch (tileID)
        {
            case 0:
                path = "assets/PlayerBoardTile_0.png";
                break;
            case 1:
                path = "assets/EnvironmentTile_1.png";
                break;
            case 2:
                path = "assets/CharacterTile_2.png";
                break;
            default:
                path = "assets/EnvironmentTile_1.png";
                break;
        }
    }

    void init() override
    {
        entity->addComponent<TransformComponent>(static_cast<float>(tileRect.x), static_cast<float>(tileRect.y), tileRect.w, tileRect.h, 2.0);
        transform = &entity->getComponent<TransformComponent>();

        entity->addComponent<SpriteComponent>(path);
        sprite = &entity->getComponent<SpriteComponent>();
    }
};
