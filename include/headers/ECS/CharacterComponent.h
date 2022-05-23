#pragma once
#include <iostream>
#include <utility>
#include <fstream>

#include "ECS/CharacterFrameComponent.h"
#include "ECS/TileComponent.h"
#include "Game.h"

#include "EnumManager.h"
#include "Vector2D.h"

using GameUnit = size_t;


class CharacterComponent : public Component
{
private:
    CharacterFrameComponent* characterFrame;
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
public:


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

    void init() override
    {
        characterFrame = &entity->addComponent<CharacterFrameComponent>("fightFrame");
    }

    void update() override
    {
        if (currentHealth <= 0)
        {
            SDL_Event event;
            SDL_memset(&event, 0, sizeof(event)); 
            event.type = SDL_USEREVENT;
            event.user.code = UserEvents::PLAYERDIED;
            if (side == Side::ENEMY)
            {
                triggerAddMoney();
                event.user.code = UserEvents::ENEMYDIED;
            }
            SDL_PushEvent(&event);
            Vector2D tileCoord = entity->getComponent<TransformComponent>().getPos();
            Game::getTileByCoord(tileCoord.x/64, tileCoord.y/64).getComponent<TileComponent>().bindedCharIndex = -1;
            entity->destroy();
        }
    }

        void sell()
    {
        currentHealth = 0;
        triggerAddMoney();
    }

    

    void startAttack(Entity* enemy)
    {
        characterFrame->activate(false);
        enemy->getComponent<CharacterFrameComponent>().activate(true);
    }

    void finishAttack(Entity* enemy)
    {
        characterFrame->deactivate();
        enemy->getComponent<CharacterFrameComponent>().deactivate();
        enemy->getComponent<CharacterComponent>().currentHealth -= damage;

        if (!ranged)
        {
            currentHealth -= enemy->getComponent<CharacterComponent>().damage;
        }
    }

    void powerUp()
    {
        damage *= 1.1;
        maxHealth *= 1.1;
        currentHealth *= 1.1;
    }

    GameUnit getCost()
    {
        return cost;
    }

private:
    void triggerAddMoney()
    {
        SDL_Event event;
        SDL_memset(&event, 0, sizeof(event)); 
        event.type = SDL_USEREVENT;
        event.user.code = UserEvents::ADDMONEY;
        uintptr_t costptr = (uintptr_t)&cost;
        event.user.data1 = (void*)costptr; // Passing data with the Event, this conversion might not be the best solution
        SDL_PushEvent(&event);
    }

};