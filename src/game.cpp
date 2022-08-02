#include "Game.h"
#include "TextureManager.h"

#include <random>
#include <chrono>
#include <inttypes.h>

#include "ECS/Components.h"

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

std::default_random_engine gen(seed);

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager; //This is destroyed

GameStage Game::gameStage;
GameMode Game::gameMode;

const char* mapFile = "assets/TileSet.png";
const char* characterTilesFile = "assets/CharacterTile.png";

std::unique_ptr<AssetManager> Game::assets = std::make_unique<AssetManager>(&manager);

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
    startingMoney = 350;
    currentDuelIndex = 0;
    currentOpponentIndex = 0;
    currentDuelTimer = 0;
    attackingSide = Side::PLAYER;
    duelIsActive = false;
}
Game::~Game() 
{
}

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
    assets->addTexture("buttonRegular", "assets/buttonRegular.png");
    assets->addTexture("fightFrame", "assets/fightFrame.png");

    const std::array<const char*, 10> characterTextures = {"assets/char0.png", "assets/char1.png", "assets/char2.png", "assets/char3.png", "assets/char4.png", "assets/char5.png",
"assets/char6.png", "assets/char7.png", "assets/char8.png", "assets/char9.png"};

    for (size_t i = 0; i < characterTextures.size(); i++)
    {
        Game::assets->addTexture("char" + std::to_string(i) + ".png", characterTextures[i]);
    }


    assets->addFont("textSmall", "fonts/Tillana-Bold.ttf", 24);
     assets->addFont("textCharacters", "fonts/Tillana-Bold.ttf", 32);
    assets->addFont("textRegular", "fonts/Tillana-Bold.ttf", 48);
    assets->addFont("textLarge", "fonts/Tillana-Bold.ttf", 72);

    map = std::make_unique<Map>("terrain", "charOverlay", 64);
    std::vector<Vector2D> shopTileCoords;
    std::vector<Vector2D> enemiesTileCoords;
    std::vector<Vector2D> playerBoardCoords;
    map->loadMap("assets/MainMap.txt", "assets/charOverlay.txt", playerBoardCoords, shopTileCoords, enemiesTileCoords, numTilesX, numTilesY);
    levelManager = std::make_unique<LevelManager>("assets/characters.json", playerBoardCoords, shopTileCoords, enemiesTileCoords);


    
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {135, 0, 0, 255};
    
    characterLabel.addComponent<CharacterLabel>("textCharacters", white);
    characterLabel.addGroup(GroupLabels::groupLABELS);

    moneyLabel.addComponent<MoneyLabel>(350, 64*13, 64*12, "Gold: ", "textRegular", white);
    moneyLabel.addGroup(GroupLabels::groupLABELS);

    buttonEvil.addComponent<ButtonComponent>(Vector2D(64*11 - 96, 64*7 - 48), 96, 192, UserEvents::EVILMODE, "buttonEvil");
    buttonEvil.addComponent<UILAbelComponent>("Evil", "textRegular", white);
    buttonEvil.addGroup(GroupLabels::groupBUTTONS);

    buttonGood.addComponent<ButtonComponent>(Vector2D(64*12 + 96, 64*7 - 48), 96, 192, UserEvents::GOODMODE, "buttonGood");
    buttonGood.addComponent<UILAbelComponent>("Good", "textRegular", white);
    buttonGood.addGroup(GroupLabels::groupBUTTONS);

    buttonFight.addComponent<ButtonComponent>(Vector2D(64*18, 64*10), 96, 192, UserEvents::STARTFIGHT, "buttonRegular");
    buttonFight.addComponent<UILAbelComponent>("Start Fight", "textSmall", white);
    buttonFight.addGroup(GroupLabels::groupBUTTONS);

    buttonPowerUp.addComponent<ButtonComponent>(Vector2D(64*21 + 32, 64*10), 96, 192, UserEvents::POWERUP, "buttonRegular");
    buttonPowerUp.addComponent<UILAbelComponent>("Power Up", "textSmall", white);
    buttonPowerUp.addGroup(GroupLabels::groupBUTTONS);

    shopText.addComponent<UILAbelComponent>(64*20 - 32, 64*1 - 32, "Shop", "textRegular", white);
    shopText.addGroup(GroupLabels::groupLABELS);

    enemyBoardText.addComponent<UILAbelComponent>(64*6, 64*3 - 32, "Enemy Board", "textRegular", white);
    enemyBoardText.addGroup(GroupLabels::groupLABELS);

    playerBoardText.addComponent<UILAbelComponent>(64*6, 64*9 - 32, "Your Board", "textRegular", white);
    playerBoardText.addGroup(GroupLabels::groupLABELS);

    levelText.addComponent<UILAbelComponent>(10, 10, "Level: 0", "textRegular", white);
    levelText.addGroup(GroupLabels::groupLABELS);
    
    gameOverText.addComponent<UILAbelComponent>(64*4, 64*6 - 48, "Game Over. Reached Level: ", "textLarge", red);
}

void Game::initLevel()
{
    levelManager->loadCharacters(currentLevel, gameMode);
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
        for (auto& pb : playerBoard)
        {
            pb->handleEvents();
        }
        for (auto& en : enemyBoard)
        {
            en->handleEvents();
        }
    }

    for (auto& l : labels)
    {
        l->handleEvents();
    }

    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
        case SDL_USEREVENT:
            if(event.user.code == UserEvents::GOODMODE)
            {
                gameMode = GameMode::GOOD;
                gameStage = GameStage::BOARDMANAGEMENT;
                buttonEvil.destroy();
                buttonGood.destroy();
                initLevel();
            }
            else if(event.user.code == UserEvents::EVILMODE)
            {
                gameMode = GameMode::EVIL;
                gameStage = GameStage::BOARDMANAGEMENT;
                buttonEvil.destroy();
                buttonGood.destroy();
                initLevel();
            }
            else if(event.user.code == UserEvents::STARTFIGHT && !(playerBoard.empty() || enemyBoard.empty()))
            {
                gameStage = GameStage::FIGHTING;
                playerBoardSize =  playerBoard.size();
                enemyBoardSize =  enemyBoard.size();
            }
            else if(event.user.code == UserEvents::POWERUP && !playerBoard.empty())
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
            else if(event.user.code == UserEvents::ENEMYDIED)
            {
            }
            else if(event.user.code == UserEvents::PLAYERDIED)
            {
            }
            break;
    }
    
}


void Game::update()
{
    manager.refresh();
    manager.update();
    
    if (gameStage == GameStage::FIGHTING)
    {
        startDuel();
        
    }
    
}

void Game::startDuel()
{
    if (!(playerBoard.empty() || enemyBoard.empty()))
    { 
        if (attackingSide == Side::PLAYER)
        {
            if (!duelIsActive)
            {
                if (playerBoardSize > playerBoard.size()) //size is changed, therefore previous character is dead
                {
                    playerBoardSize = playerBoard.size();
                    currentDuelIndex--;
                }
                if (currentDuelIndex < playerBoard.size())
                {
                    currentOpponentIndex = random(0, enemyBoard.size() - 1);
                   
                    currentOpponent = enemyBoard.at(currentOpponentIndex);
                    currentAttacker = playerBoard.at(currentDuelIndex);
                    currentAttacker->getComponent<CharacterComponent>().startAttack(currentOpponent);
                }
                else
                {
                    currentDuelIndex = 0;
                    enemyBoardSize = enemyBoard.size();
                    attackingSide = Side::ENEMY;
                    return;
                }
                currentDuelTimer = 0;
                duelIsActive = true;
            }
            else if (duelIsActive && currentDuelTimer > 1000)
            {
                currentAttacker->getComponent<CharacterComponent>().finishAttack(currentOpponent);
                currentDuelIndex++;
                duelIsActive = false;
            }
            
        }
        else
        {
            if (!duelIsActive)
            {
                if (enemyBoardSize > enemyBoard.size())
                {
                    currentDuelIndex--;
                    enemyBoardSize = enemyBoard.size();
                }
                if (currentDuelIndex < enemyBoard.size())
                {
                    currentOpponentIndex = random(0, playerBoard.size() - 1);
                    currentOpponent = playerBoard.at(currentOpponentIndex);
                    currentAttacker = enemyBoard.at(currentDuelIndex);
                    currentAttacker->getComponent<CharacterComponent>().startAttack(currentOpponent);
                }
                else
                {
                    currentDuelIndex = 0;
                    playerBoardSize = playerBoard.size();
                    attackingSide = Side::PLAYER;
                    return;
                }
                currentDuelTimer = 0;
                duelIsActive = true;
            }
            else if (duelIsActive && currentDuelTimer > 1000)
            {
                currentAttacker->getComponent<CharacterComponent>().finishAttack(currentOpponent);
                currentDuelIndex++;
                duelIsActive = false;
            }
        }
    }    
    else
    {
        if (playerBoard.empty() && !enemyBoard.empty())
        {
            gameOverText.getComponent<UILAbelComponent>().addText(std::to_string(currentLevel));
            gameStage = GameStage::GAMEOVER;
        }
        else
        {
            currentDuelIndex = 0;
            currentDuelTimer = 0;
            attackingSide = Side::PLAYER;
            currentLevel++;
            levelText.getComponent<UILAbelComponent>().setText("Level: " + std::to_string(currentLevel));
            gameStage = GameStage::BOARDMANAGEMENT;
            initLevel();
        }
    }
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
        if (gameStage == GameStage::FIGHTING)
        {
            currentDuelTimer += (frameTime < frameDelay ? frameDelay : frameTime);
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

Entity& Game::getCharByIndex(GroupLabels grp, size_t i)
{
    return  manager.getCharByIndex(grp, i);
}

Entity& Game::getTileByCoord(int xTile, int yTile) // Treat 1d group array as 2d
{ //TBD decide on input (tile index or coords)
    return *tiles.at(25*yTile + xTile);
}