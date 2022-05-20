#pragma once

#include "ECS/ECS.h"
#include "TextureManager.h"
#include "AssetManager.h"
#include "Game.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include <string>
#include <array>

// TBD support adding text by passing position of center of the label

class UILAbelComponent : public Component
{
private:
    SDL_Rect position;
    std::string labelText;
    SDL_Color textColor;
    SDL_Texture* labelTexture;
    TTF_Font* font;

public:

    UILAbelComponent(int xPos, int yPos, const std::string& text, SDL_Color& color, int textSize) : labelText(text), textColor(color)
    {
        position.x = xPos;
        position.y = yPos;
        font = TTF_OpenFont("fonts/Tillana-Bold.ttf", textSize);
        setLabelTexture();
    }

    ~UILAbelComponent()
    {
    }

    void setText(const std::string& text)
    {
        labelText = text;
        setLabelTexture();
    }

    void addText(const std::string& text)
    {
        setText(labelText + text);
    }

    void setLabelTexture()
    {
        SDL_Surface* surf = TTF_RenderText_Blended(font, labelText.c_str(), textColor);
        labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf); // Might put it in TextureManager
        SDL_FreeSurface(surf);

        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h); // Getting height and width of texture
    }

    void draw() override
    {
        SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
    }


};