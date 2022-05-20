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


class Game {
    public:
        Game();
        ~Game();

        void run();
        
        
        static SDL_Renderer* renderer;
        static SDL_Event event;
        static Entity* getCharByIndex(GroupLabels grp, size_t i);
        static AssetManager* assets;
        static Entity* getTileByCoord(int xTile, int yTile);
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
        void startFight();
        int random(int low, int high) const;
        //void updateBoard();
        void restart();

        void initBoard(size_t boardSize);

        SDL_Window* window;

        Level currentLevel;

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