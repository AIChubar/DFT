#pragma once

#include "TextureManager.h"

#include "Game.h"

#include <string>
#include <vector>
// TBD redo this
class MoneyLabel : public Component
{
private:

    

    SDL_Rect position;
    std::string labelText;
    SDL_Color textColor;
    SDL_Texture* labelTexture;
    TTF_Font* font;

public:

    int balance;

    MoneyLabel(int bal)
    {
        textColor = {255, 255, 255, 255};
        font = TTF_OpenFont("fonts/Tillana-Bold.ttf", 48);

        balance = bal;
        
        position.x = 64 * 13;
        position.y = 64 * 12; 

        setLabelTexture();      
    }

    ~MoneyLabel() = default;


    void draw() override
    {
        SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);  
    }

    void changeBalance(int amount)
    {
        balance += amount;
        setLabelTexture(); 
    }
    bool pay (int amount)
    {
        if (amount <= balance)
        {
            changeBalance(-amount);
            return true;
        }
        return false;
    }
    void setLabelTexture()
    {
        labelText = "Gold: " + std::to_string(balance);
        SDL_Surface* surf = TTF_RenderText_Blended(font, labelText.c_str(), textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surf); // Might put it in TextureManager
        SDL_FreeSurface(surf);
        labelTexture = texture;
        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h); // Getting height and width of texture
    }
};