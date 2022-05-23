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
protected:
    SDL_Rect position;
    std::string labelText;
    SDL_Color textColor;
    SDL_Texture* labelTexture;
    TTF_Font* font;

public:

    UILAbelComponent(int xPos, int yPos, const std::string& text, const std::string& fID,  const SDL_Color& color) : labelText(text), textColor(color)
    {
        position.x = xPos;
        position.y = yPos;
        font = Game::assets->getFont(fID);
    }

    UILAbelComponent(const std::string& text, const std::string& fID, const SDL_Color& color) : labelText(text), textColor(color)
    {
        font = Game::assets->getFont(fID);      
    }

    ~UILAbelComponent()
    {
        SDL_DestroyTexture(labelTexture);
    }

    void init() override
    {
        setLabelTexture();
        
        if (entity->hasComponent<TransformComponent>())
        {
            
            TransformComponent* transform = &entity->getComponent<TransformComponent>();
            int xCenter = transform->x() + transform->w() / 2;
            int yCenter = transform->y() + transform->h() / 2;
            position.x = xCenter - position.w / 2;
            position.y = yCenter - position.h / 2;
        }

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


    void draw() override
    {
        SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
    }
protected:
    void setLabelTexture()
    {
        SDL_Surface* surf = TTF_RenderText_Blended(font, labelText.c_str(), textColor);
        labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
        SDL_FreeSurface(surf);
        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h); // Getting height and width of texture
    }


};