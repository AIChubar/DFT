#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "LevelManager.h"
#include <random>
#include <chrono>
#include <inttypes.h>

#include "ECS/Components.h"

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

std::default_random_engine gen(seed);

Map* map; //initialized here to avoid circular dependency
LevelManager* levelManager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager;

GameStage Game::gameStage;
GameMode Game::gameMode;

const char* mapFile = "assets/TileSet.png";
const char* characterTilesFile = "assets/CharacterTile.png";

AssetManager* Game::assets = new AssetManager(&manager);

// Declaring groups
auto& tiles(manager.getGroup(GroupLabels::groupMAP)); 
auto& characterTiles(manager.getGroup(GroupLabels::groupCHARACTERTILES));
auto& enemyChars(manager.getGroup(GroupLabels::groupENEMYCHARS));
auto& playerChars(manager.getGroup(GroupLabels::groupPLAYERCHARS));
auto& shop(manager.getGroup(GroupLabels::groupSHOP));
auto& playerBoard(manager.getGroup(GroupLabels::groupPLAYERBOARD));
auto& enemyBoard(manager.getGroup(GroupLabels::groupENEMYBOARD));
auto& labels(manager.getGroup(GroupLabels::groupLABELS));
auto& buttons(manager.getGroup(GroupLabels::groupBUTTONS));


auto& characterLabel(manager.addEntity());
auto& moneyLabel(manager.addEntity());

auto& buttonEvil(manager.addEntity());
auto& buttonGood(manager.addEntity());
auto& buttonFight(manager.addEntity());
auto& buttonPowerUp(manager.addEntity());

auto& gameOverText(manager.addEntity());
auto& shopText(manager.addEntity());
auto& enemyBoardText(manager.addEntity());
auto& playerBoardText(manager.addEntity());
auto& levelText(manager.addEntity());
    
Game::Game() {
    window = nullptr;
    renderer = nullptr;
    screenWidth = 1600;
    screenHeight = 896;
    numTilesX = 25;
    numTilesY = 14;
    gameState = GameState::PLAY;
    Game::gameStage = GameStage::MODECHOOSING;
    currentLevel = 0;
    
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

    if(TTF_Init() == -1)
    {
        std::cout << "Error : TTF" << '\n';
    }

    assets->addTexture("terrain", "assets/terrain.png");
    assets->addTexture("charOverlay", "assets/charOverlay.png");
    assets->addTexture("buttonEvil", "assets/buttonEvil.png");
    assets->addTexture("buttonGood", "assets/buttonGood.png");
    assets->addTexture("regularButton", "assets/regularButton.png");

    map = new Map("terrain", "charOverlay", 64);
    std::vector<Vector2D> shopTileCoords;
    std::vector<Vector2D> enemiesTileCoords;
    map->loadMap("assets/MainMap.txt", "assets/charOverlay.txt", shopTileCoords, enemiesTileCoords, numTilesX, numTilesY);
    levelManager = new LevelManager(&manager, "assets/characters.json", shopTileCoords, enemiesTileCoords);

    
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {135, 0, 0, 255};
    characterLabel.addComponent<CharacterLabel>();
    characterLabel.addGroup(GroupLabels::groupLABELS);

    moneyLabel.addComponent<MoneyLabel>(250);
    moneyLabel.addGroup(GroupLabels::groupLABELS);

    buttonEvil.addComponent<TransformComponent>(Vector2D(64*11 - 96, 64*7 - 48), 48, 96);
    buttonEvil.addComponent<SpriteComponent>("buttonEvil");
    buttonEvil.addComponent<GameModeButtonComponent>(GameMode::EVIL);
    buttonEvil.addComponent<UILAbelComponent>(64*11 - 96, 64*7 - 48, "Evil", white, 48); // Need to add constructor that consider transformComponent to center text
    buttonEvil.addGroup(GroupLabels::groupBUTTONS);

    buttonGood.addComponent<TransformComponent>(Vector2D(64*12 + 96, 64*7 - 48), 48, 96);
    buttonGood.addComponent<SpriteComponent>("buttonGood");
    buttonGood.addComponent<GameModeButtonComponent>(GameMode::GOOD);
    buttonGood.addComponent<UILAbelComponent>(64*12 + 96, 64*7 - 48, "Good", white, 48);
    buttonGood.addGroup(GroupLabels::groupBUTTONS);
    
    buttonFight.addComponent<TransformComponent>(Vector2D(64*17 + 32, 64*10), 48, 96);
    buttonFight.addComponent<SpriteComponent>("buttonGood"); //needs to have its button
    buttonFight.addComponent<ButtonComponent>(UserEvents::STARTFIGHT);
    buttonFight.addComponent<UILAbelComponent>(64*17 + 32, 64*10, "Start Fight", white, 24);
    buttonFight.addGroup(GroupLabels::groupBUTTONS);

    buttonPowerUp.addComponent<TransformComponent>(Vector2D(64*21, 64*10), 48, 96);
    buttonPowerUp.addComponent<SpriteComponent>("buttonGood"); //needs to have its button
    buttonPowerUp.addComponent<ButtonComponent>(UserEvents::POWERUP);
    buttonPowerUp.addComponent<UILAbelComponent>(64*21, 64*10, "Power Up", white, 24);
    buttonPowerUp.addGroup(GroupLabels::groupBUTTONS);

    shopText.addComponent<UILAbelComponent>(64*20 -32, 64*1 - 32, "Shop", white, 48);
    shopText.addGroup(GroupLabels::groupLABELS);

    enemyBoardText.addComponent<UILAbelComponent>(64*6, 64*3 - 32, "Enemy Board", white, 48);
    enemyBoardText.addGroup(GroupLabels::groupLABELS);

    playerBoardText.addComponent<UILAbelComponent>(64*6, 64*9 - 32, "Your Board", white, 48);
    playerBoardText.addGroup(GroupLabels::groupLABELS);

    levelText.addComponent<UILAbelComponent>(10, 10, "Level: 0", white, 48);
    levelText.addGroup(GroupLabels::groupLABELS);
    
    gameOverText.addComponent<UILAbelComponent>(64*4, 64*6 - 48, "Game Over. Reached Level: ", red, 72);

}

void Game::initLevel()
{
    levelManager->loadCharacters(currentLevel, gameMode);
}

void Game::startFight() // need to add fighting visualization and improve structure
{
    while (!(playerBoard.empty() || enemyBoard.empty()))
    {
        for (size_t i = 0; i < playerBoard.size(); i++)
        {   
            size_t randomIndex = random(0, enemyBoard.size() - 1);
            CharacterComponent* opponent = &enemyBoard.at(randomIndex)->getComponent<CharacterComponent>();
            playerBoard.at(i)->getComponent<CharacterComponent>().attack(opponent);
            update();
            if (playerBoard.empty() || enemyBoard.empty())
            {
                break;
            }
        }
        if (playerBoard.empty())
        {
            break;
        }
        for (size_t i = 0; i < enemyBoard.size(); i++)
        {
            size_t randomIndex = random(0, playerBoard.size() - 1);
            CharacterComponent* opponent = &playerBoard.at(randomIndex)->getComponent<CharacterComponent>();
            enemyBoard.at(i)->getComponent<CharacterComponent>().attack(opponent);
            update();
            if (playerBoard.empty() || enemyBoard.empty() )
            {
                break;
            }
        }
    }
    if (playerBoard.empty() && !enemyBoard.empty())
    {
        gameOverText.getComponent<UILAbelComponent>().addText(std::to_string(currentLevel));
        gameStage = GameStage::GAMEOVER;
    }
}

int Game::random(int low, int high) const
{
    if (high <= 0)
        return 0;
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

void Game::handleEvents() 
{
    if (Game::gameStage != GameStage::FIGHTING && Game::gameStage != GameStage::GAMEOVER)
    {
        for (auto& b : buttons)
        {
            b->handleEvents();
        }
        for (auto& s : shop)
        {
            s->handleEvents();
        }
        for (auto& l : labels)
        {
            l->handleEvents();
        }
        for (auto& pb : playerBoard)
        {
            pb->handleEvents();
        }
        for (auto& en : enemyBoard)
        {
            en->handleEvents();
        }
    }

    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
        case SDL_USEREVENT:
            if(event.user.code == UserEvents::MODEBUTTON)
            {
                buttonEvil.destroy();
                buttonGood.destroy();
                initLevel();
            }
            else if(event.user.code == UserEvents::STARTFIGHT && !(playerBoard.empty() || enemyBoard.empty()))
            {
                //gameStage = GameStage::FIGHTING;
                startFight();
                
                currentLevel++;
                levelText.getComponent<UILAbelComponent>().setText("Level: " + std::to_string(currentLevel));
                initLevel();
            }
            else if(event.user.code == UserEvents::POWERUP)
            {
                if (!moneyLabel.getComponent<MoneyLabel>().pay(30))
                    break;
                for (auto& pl : playerBoard)
                {
                    pl->getComponent<CharacterComponent>().powerUp();
                }
            }
            else if(event.user.code == UserEvents::ADDMONEY)
            {
                int cost = *(int*)event.user.data1;
                moneyLabel.getComponent<MoneyLabel>().changeBalance(cost*0.7);
            }
            break;
    }
    
}

void Game::update()
{
    manager.refresh();
    manager.update();
}


void Game::render() 
{
    SDL_RenderClear(renderer);

    for (auto& t : tiles)
    {
        t->draw();
    }
    if(Game::gameStage == GameStage::MODECHOOSING)
    {
        buttonEvil.draw();
        buttonGood.draw();
        SDL_RenderPresent(renderer);
        SDL_Delay(0);
        return;
    }
    for (auto& ct : characterTiles)
    {
        ct->draw();
    }
    for (auto& p : playerBoard)
    {
        p->draw();
    }
    for (auto& s : shop)
    {
        s->draw();
    }
    for (auto& e : enemyBoard)
    {
        e->draw();
    }
    for (auto& b : buttons)
    {
        b->draw();
    }
    for (auto& l : labels)
    {
        l->draw();
    }
    if (characterLabel.getComponent<CharacterLabel>().active)
    {
        characterLabel.draw();
    }
    moneyLabel.draw();
    if (gameStage == GameStage::GAMEOVER)
    {
        gameOverText.draw();
    }
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

void Game::restart() // TBD
{
    // manager.destroyAll();
    // run();
}



//Static functions

Entity* Game::getCharByIndex(GroupLabels grp, size_t i)
{
    Entity* Character = manager.getCharByIndex(grp, i);
    return Character;
}

Entity* Game::getTileByCoord(int xTile, int yTile) // Treat 1d group array as 2d
{ //TBD decide on input (tile index or coords)
    return tiles.at(25*yTile + xTile);
}