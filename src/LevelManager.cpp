#include "LevelManager.h"
#include "ECS/Components.h"
#include "Game.h"
#include <fstream>


extern Manager manager;

LevelManager::LevelManager(const std::string& jCharFile, const std::vector<Vector2D>& playerBoard, const std::vector<Vector2D>& shopTiles, const std::vector<Vector2D>& enemiesTiles)
{
    std::ifstream f(jCharFile);
    f >> jCharacters;
    playerBoardCoords = playerBoard;
    shopTileCoords = shopTiles;
    enemiesTileCoords = enemiesTiles; 
}

LevelManager::~LevelManager(){}

void LevelManager::loadCharacters(Level level, GameMode gmd)
{
    size_t enemiesStartingIndex = 5;
    size_t playerStartingIndex = 0;
    if (gmd == GameMode::EVIL)
    {
        enemiesStartingIndex = 0;
        playerStartingIndex = 5;
    }
    
    if (level == 0)
    {
        addChars(Side::PLAYER, PowerLevels::PAWN + playerStartingIndex, 0, 4);
        addChars(Side::PLAYER, PowerLevels::ROOK + playerStartingIndex, 0, 3);
        addChars(Side::PLAYER, PowerLevels::KNIGHT + playerStartingIndex, 0, 1);
        addChars(Side::ENEMY, PowerLevels::PAWN + enemiesStartingIndex, 0, 6);
        addChars(Side::ENEMY, PowerLevels::ROOK + enemiesStartingIndex, 0, 2);
    }
    else if (level == 1)
    {
        addChars(Side::PLAYER, PowerLevels::ROOK + playerStartingIndex, 2, 1);
        addChars(Side::PLAYER, PowerLevels::KNIGHT + playerStartingIndex, 1, 2);
        addChars(Side::PLAYER, PowerLevels::BISHOP + playerStartingIndex, 0, 3);
        addChars(Side::ENEMY, PowerLevels::PAWN + enemiesStartingIndex, 2, 4);
        addChars(Side::ENEMY, PowerLevels::ROOK + enemiesStartingIndex, 2, 2);
        addChars(Side::ENEMY, PowerLevels::KNIGHT + enemiesStartingIndex, 1, 2);
        
    }
    else if (level == 2)
    {
        addChars(Side::PLAYER, PowerLevels::ROOK + playerStartingIndex, 2, 1);
        addChars(Side::PLAYER, PowerLevels::KNIGHT + playerStartingIndex, 1, 2);
        addChars(Side::PLAYER, PowerLevels::BISHOP + playerStartingIndex, 0, 3);
        addChars(Side::ENEMY, PowerLevels::PAWN + enemiesStartingIndex, 4, 3);
        addChars(Side::ENEMY, PowerLevels::ROOK + enemiesStartingIndex, 3, 2);
        addChars(Side::ENEMY, PowerLevels::KNIGHT + enemiesStartingIndex, 2, 2);
        
    }
    else if (level == 3)
    {
        addChars(Side::PLAYER, PowerLevels::KNIGHT + playerStartingIndex, 2, 2);
        addChars(Side::PLAYER, PowerLevels::BISHOP + playerStartingIndex, 1, 2);
        addChars(Side::ENEMY, PowerLevels::ROOK + enemiesStartingIndex, 4, 2);
        addChars(Side::ENEMY, PowerLevels::KNIGHT + enemiesStartingIndex, 4, 3);
        addChars(Side::ENEMY, PowerLevels::BISHOP + enemiesStartingIndex, 2, 3);
        addChars(Side::ENEMY, PowerLevels::KING + enemiesStartingIndex, 0, 2);
    }
    else if (level == 4)
    {
        addChars(Side::PLAYER, PowerLevels::KNIGHT + playerStartingIndex, 3, 2);
        addChars(Side::PLAYER, PowerLevels::BISHOP + playerStartingIndex, 2, 2);
        addChars(Side::PLAYER, PowerLevels::KING + playerStartingIndex, 0, 1);
        addChars(Side::ENEMY, PowerLevels::KNIGHT + enemiesStartingIndex, 4, 4);
        addChars(Side::ENEMY, PowerLevels::BISHOP + enemiesStartingIndex, 2, 3);
        addChars(Side::ENEMY, PowerLevels::KING + enemiesStartingIndex, 1, 2);
    }
    else if (level == 5)
    {
        addChars(Side::PLAYER, PowerLevels::BISHOP + playerStartingIndex, 3, 3);
        addChars(Side::PLAYER, PowerLevels::KING + playerStartingIndex, 1, 2);
        addChars(Side::ENEMY, PowerLevels::BISHOP + enemiesStartingIndex, 3, 4);
        addChars(Side::ENEMY, PowerLevels::KING + enemiesStartingIndex, 3, 2);
    }
    else if (level == 6)
    {
        addChars(Side::PLAYER, PowerLevels::KING + playerStartingIndex, 2, 4);
        addChars(Side::ENEMY, PowerLevels::KING + enemiesStartingIndex, 5, 5);
    }
    else if (level > 6)
    {
        addChars(Side::ENEMY, PowerLevels::KING + enemiesStartingIndex, level*level, 7);
    }
    loadShop();
    loadEnemies();
}

void LevelManager::addChars(Side side, size_t chr, size_t powerCoef, size_t charsNum) // I wanted to keep adding new entities only in the Game class functions
{
    for (size_t i = 0; i < charsNum; i++)
    {
        auto& character(manager.addEntity());
        character.addComponent<CharacterComponent>(jCharacters.at(chr), powerCoef, chr, side);
        if (side == Side::PLAYER)
        {
            character.addGroup(GroupLabels::groupPLAYERCHARS);
        }
        else
        {
            character.addGroup(GroupLabels::groupENEMYCHARS);
        }
        
    }
}

void LevelManager::loadShop()
{
    manager.clearGroup(groupSHOP);
    manager.shuffleGroup(groupPLAYERCHARS);
    auto& plChars(manager.getGroup(GroupLabels::groupPLAYERCHARS));
    for (size_t i = 0; i < shopTileCoords.size(); i++)
    {
        if (i >= plChars.size())
        {
            break;
        }
        Vector2D tileCoord = shopTileCoords.at(i);
        if (!plChars.at(i)->getComponent<CharacterComponent>().textureAdded)
        {
            plChars.at(i)->addComponent<TransformComponent>(tileCoord * 64);
            plChars.at(i)->addComponent<SpriteComponent>(plChars.at(i)->getComponent<CharacterComponent>().character);
            plChars.at(i)->addComponent<CharacterLabelControlComponent>();
            plChars.at(i)->addComponent<DragComponent>();
            plChars.at(i)->getComponent<CharacterComponent>().textureAdded = true;
        }
        else
        {
            plChars.at(i)->getComponent<TransformComponent>().setPos(tileCoord * 64);
        }
        plChars.at(i)->addGroup(groupSHOP);
    }
}

void LevelManager::loadEnemies()
{
    manager.clearGroup(groupENEMYBOARD);
    manager.shuffleGroup(groupENEMYCHARS);
    auto& enChars(manager.getGroup(GroupLabels::groupENEMYCHARS));
    for (size_t i = 0; i < enemiesTileCoords.size(); i++)
    {
        if (i >= enChars.size())
        {
            break;
        }
        Vector2D tileCoord = enemiesTileCoords.at(i);
        if (!enChars.at(i)->getComponent<CharacterComponent>().textureAdded) // this is not currently needed for enemies
        {
            enChars.at(i)->addComponent<TransformComponent>(tileCoord * 64);
            enChars.at(i)->addComponent<SpriteComponent>(enChars.at(i)->getComponent<CharacterComponent>().character);
            enChars.at(i)->addComponent<CharacterLabelControlComponent>();
            enChars.at(i)->getComponent<CharacterComponent>().textureAdded = true;
        }
        else
        {
            enChars.at(i)->getComponent<TransformComponent>().setPos(tileCoord * 64);
        }
        enChars.at(i)->addGroup(groupENEMYBOARD);
        
    }
}
