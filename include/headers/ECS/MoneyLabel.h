#pragma once

#include "UILabelComponent.h"



#include <string>
#include <vector>

class MoneyLabel : public UILAbelComponent
{
private:
    int balance;
    std::string baseText;
public:

    MoneyLabel(int bal, int xPos, int yPos, const std::string& text, const std::string& fID,  const SDL_Color& color)
    : UILAbelComponent(xPos, yPos, text + std::to_string(bal), fID, color), balance(bal), baseText(text)
    {}

    ~MoneyLabel() = default;


    void draw() override
    {
        SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);  
    }



    void changeBalance(int amount)
    {
        balance += amount;
        labelText = baseText + std::to_string(balance);
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
};