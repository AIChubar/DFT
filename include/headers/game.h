#pragma once

#include <bitset>
#include <iostream>

#include "sdl2/SDL.h"
#include "sdl2/SDL_image.h"
#include "sdl2/SDL_ttf.h"
#include "nlohmann/json.hpp"

#include "EnumManager.h"
#include "ECS/ECS.h"
#include "AssetManager.h"
#include "LevelManager.h"

#include "Map.h"


class Game {
    public:
        Game();
        ~Game();

        void run();
        
        static std::unique_ptr<AssetManager> assets;
        static SDL_Renderer* renderer;
        static SDL_Event event;
        static Entity& getCharByIndex(GroupLabels grp, size_t i);
        static Entity& getTileByCoord(int xTile, int yTile);
        static GameStage gameStage;
        static GameMode gameMode;

    private:
        void init(const char* title, int x, int y, int w, int h, Uint32 flags);
        void gameLoop();
        void handleEvents();
        void render();
        void clean();
        void update();
        void initLevel();
        void startDuel();
        int random(int low, int high) const;
        void restart();

        void initBoard(size_t boardSize);

        std::unique_ptr<Map> map;
        std::unique_ptr<LevelManager> levelManager;

        size_t currentDuelIndex;
        size_t currentOpponentIndex; // random opponent index
        int currentDuelTimer; // in ms
        Side attackingSide;
        bool duelIsActive;
        size_t playerBoardSize;
        size_t enemyBoardSize;

        Entity* currentOpponent;
        Entity* currentAttacker;

        SDL_Window* window;

        Level currentLevel;
        int startingMoney;

        int screenWidth;
        int screenHeight;
        int numTilesX;
        int numTilesY;

        const int FPS = 60;
        const int frameDelay = 1000 / FPS;  

        Uint32 frameStart;
        int frameTime;

        GameState gameState;
};