#pragma once
#include <iostream>
#include <utility>
#include <fstream>

#include "EnumManager.h"
#include "Vector2D.h"

using GameUnit = size_t;

// TBD add user events to control characters

class CharacterComponent : public Component
{
public:

    bool textureAdded;

    GameUnit maxHealth;
    int currentHealth;
    GameUnit damage;
    GameUnit cost;
    Side side;
    Characters character;
    std::string name;

    size_t inGroupIndex;

    bool ranged;
    bool cleave;

    ~CharacterComponent() = default;
    CharacterComponent() = default;

    CharacterComponent(const nlohmann::json& jCharacter, GameUnit powerCoef, size_t chr, Side sd)
    {
        textureAdded = false;
        maxHealth = currentHealth = jCharacter["health"];
        maxHealth = currentHealth = maxHealth * (1 + 0.3 * powerCoef);
        damage = jCharacter["damage"];
        damage *= (1 + 0.3 * powerCoef);
        ranged = jCharacter["ranged"];
        cleave = jCharacter["cleave"];
        name = jCharacter["name"];
        cost = jCharacter["cost"];
        character = static_cast<Characters>(chr);
        side = sd;
    }

    void update() override
    {
        if (currentHealth <= 0)
        {
            if (side == Side::ENEMY)
            {
                SDL_Event event;
                SDL_memset(&event, 0, sizeof(event)); 
                event.type = SDL_USEREVENT;
                event.user.code = UserEvents::ADDMONEY;
                uintptr_t costptr = (uintptr_t)&cost;
                event.user.data1 = (void*)costptr; // Passing data with the Event, this conversion might not be the best solution
                SDL_PushEvent(&event);
            }
            Vector2D tileCoord = entity->getComponent<TransformComponent>().position;
            Game::getTileByCoord(tileCoord.x/64, tileCoord.y/64)->getComponent<TileComponent>().bindedCharIndex = -1;
            entity->destroy();
        }
    }

    void sell() // Making a character player wants to sell an enemy and setting its hp to 0, so it triggers condition in the update function
    {
        side = Side::ENEMY;
        currentHealth = 0;
    }

    void attack(CharacterComponent* enemy)
    {
        
        enemy->currentHealth -= damage;
        if (!ranged)
        {
            currentHealth -= enemy->damage;
        }
        //if(cleave) // attacks neighbour enemies TBD
        if (enemy->currentHealth <= 0)
        {
            return;
        }
    }

    void powerUp()
    {
        damage *= 1.1;
        maxHealth *= 1.1;
        currentHealth *= 1.1;
    }

};