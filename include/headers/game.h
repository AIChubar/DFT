#pragma once

#include <iostream>
#include <sdl2/SDL.h>
#include <sdl2/SDL_image.h>

enum class GameState {PLAY, EXIT};

class Game {
    public:
        Game();
        ~Game();

        void run();
        
        static SDL_Renderer* renderer;
        static SDL_Event event;
        static void addTile(int id, int x, int y);
    private:
        void init(const char* title, int x, int y, int w, int h, Uint32 flags);
        void gameLoop();
        void handleEvents();
        void render();
        void clean();
        void update();

        SDL_Window* window;

        
        int screenWidth;
        int screenHeight;

        const int FPS = 60;
        const int frameDelay = 1000 / FPS;  

        Uint32 frameStart;
        int frameTime;

        GameState gameState;
};