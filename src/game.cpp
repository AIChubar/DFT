#include <headers/Game.h>
#include <headers/TextureManager.h>
#include <headers/GameObject.h>
#include <headers/Map.h>

#include <headers/ECS/Components.h>
#include <headers/ECS/ECS.h>

Map* map;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;

auto& player(manager.addEntity());

Game::Game() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 800;
    screenHeight = 640;
    gameState = GameState::PLAY;
}
Game::~Game() {}

void Game::run() 
{
    init("DFT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    gameLoop();
}

void Game::init(const char* title, int x, int y, int w, int h, Uint32 flags)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(title, x, y, w, h, flags);
    renderer = SDL_CreateRenderer(window, -1, 0);

    if(renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }


    map = new Map();
    
    player.addComponent<PositionComponent>();
    player.addComponent<SpriteComponent>("assets/tex.png");
}

void Game::handleEvents() 
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
    }
}

void Game::update()
{
    manager.refresh();
    manager.update();

    if(player.getComponent<PositionComponent>().x() > 100)
    {
        player.getComponent<SpriteComponent>().setTex("yellow.png");
    }
}


void Game::render() 
{
    SDL_RenderClear(renderer);
    map->drawMap();
    manager.draw();
    SDL_RenderPresent(renderer);
    SDL_Delay(0);
}

void Game::gameLoop()
{
    while (gameState != GameState::EXIT)
    {
        frameStart = SDL_GetTicks();


        handleEvents();
        update();
        render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
        
    }
    clean();
}

void Game::clean() 
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}