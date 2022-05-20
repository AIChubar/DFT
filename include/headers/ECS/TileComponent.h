#pragma once

#include "Game.h"
#include "Vector2D.h"

class TileComponent : public Component
{
    public:
    SDL_Texture* texture;
    SDL_Rect srcRectTile, srcRectTileOverlay, destRect;
    TileStatus tileStatus;
    SDL_Texture* tileOverlay;
    int bindedCharIndex; // -1 if there is no character on the tile
    Vector2D position;
    TileComponent() = default;

    ~TileComponent()
    {
        SDL_DestroyTexture(texture);
    }

    TileComponent(int srcX, int srcY, int xPos, int yPos, const std::string& idTile, const std::string& idTileOverlay, TileStatus tileStat)
    {
        texture = Game::assets->getTexture(idTile);
        tileStatus = tileStat;

        bindedCharIndex = -1;
        if (tileStatus != TileStatus::BACKGROUND)
        {
            tileOverlay = Game::assets->getTexture(idTileOverlay);
        }

        srcRectTile.x = srcX;
        srcRectTile.y = srcY;
        srcRectTile.w = srcRectTile.h = srcRectTileOverlay.w = srcRectTileOverlay.h = 32;

        srcRectTileOverlay.y = 0;
        srcRectTileOverlay.x = 32 * static_cast<size_t>(tileStatus);

        destRect.x = position.x = xPos;
        destRect.y = position.y = yPos;
        destRect.w = destRect.h = 64;
    } 

    void draw() override
    {
        TextureManager::draw(texture, srcRectTile, destRect, SDL_FLIP_NONE);
        
        if (tileStatus == TileStatus::BACKGROUND)
        {
            return;
        }
        TextureManager::draw(tileOverlay, srcRectTileOverlay, destRect, SDL_FLIP_NONE);
    }
};
