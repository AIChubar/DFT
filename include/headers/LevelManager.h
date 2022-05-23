#pragma once
#include <cstddef>

#include "EnumManager.h"
#include "Vector2D.h"
#include "nlohmann/json.hpp"




class LevelManager{
public:

    LevelManager(const std::string& jChar, const std::vector<Vector2D>& playerBoard, const std::vector<Vector2D>& shopTileCoords, const std::vector<Vector2D>& enemiesTileCoords);
    ~LevelManager();

    void loadCharacters(Level level, GameMode gmd);
    
private:

    void addChars(Side side, size_t chr, size_t powerCoef, size_t charsNum);
    void loadShop();
    void loadEnemies();
    std::vector<Vector2D> playerBoardCoords;
    std::vector<Vector2D> shopTileCoords;
    std::vector<Vector2D> enemiesTileCoords;
    nlohmann::json jCharacters;
    
};