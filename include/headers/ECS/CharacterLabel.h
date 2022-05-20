#pragma once

#include "ECS/Components.h"
#include "TextureManager.h"
#include "Game.h"

#include "SDL.h"
#include "SDL_ttf.h"


#include <string>
#include <vector>

class CharacterLabel : public Component
{
private:
    CharacterComponent* currentCharacter;

    std::vector<SDL_Rect> positions;
    std::vector<std::string> labelTexts;
    std::vector<SDL_Texture*> labelTextures;

    
    SDL_Color textColor;
    TTF_Font* font;

public:

    bool active;

    CharacterLabel()
    {
        active = false;
        textColor = {255, 255, 255, 255};
        font = TTF_OpenFont("fonts/Tillana-Bold.ttf", 30);
        
        for (size_t i = 0; i < 2; i++) //Creating 6 rectangles for characterLabel in the bottom left part of the screen
        {
            for (size_t j = 0; j < 3; j++)
            {
                SDL_Rect position;
                position.x = 64 * i * 7 + 30;
                position.y = 64 * (11 + j) + 10;
                positions.emplace_back(position);
            }  
        }
        labelTexts.resize(6);
        labelTextures.resize(6);
        
    }

    ~CharacterLabel()
    {}

    void draw() override
    {
        for (size_t i = 0; i < labelTextures.size(); i++)
        {
            SDL_RenderCopy(Game::renderer, labelTextures.at(i), nullptr, &positions.at(i));
        }
        
    }

    void update() override
    {}
    

    void activate(CharacterComponent* character)
    {
        active = true;
        currentCharacter = character;
        setLabelTexts();
    }

    void setLabelTexts()
    {
        std::string yes = "Yes";
        std::string no = "No";
        labelTexts.at(0) = ("Name: " + currentCharacter->name);
        labelTexts.at(1) = ("Health: " + std::to_string(currentCharacter->currentHealth) + " / " + std::to_string(currentCharacter->maxHealth));
        labelTexts.at(2) = ("Damage: " + std::to_string(currentCharacter->damage));
        labelTexts.at(3) = ("Cleave: " + (currentCharacter->cleave ? yes : no));
        labelTexts.at(4) = ("Range: " + (currentCharacter->ranged ? yes : no));
        labelTexts.at(5) = ("Cost: " + std::to_string(currentCharacter->cost));

        for (size_t i = 0; i < labelTexts.size(); i++)
        {
            SDL_Surface* surf = TTF_RenderText_Blended(font, labelTexts.at(i).c_str(), textColor);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surf); // Might put it in TextureManager
            SDL_FreeSurface(surf);
            labelTextures.at(i) = texture;
            SDL_QueryTexture(labelTextures.at(i), nullptr, nullptr, &positions.at(i).w, &positions.at(i).h); // Getting height and width of texture
        }
    }
};