#pragma once
#include <cstddef>

#include "EnumManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include "nlohmann/json.hpp"




class LevelManager{
public:

    LevelManager(Manager* mng, const std::string& jChar, const std::vector<Vector2D>& shopTileCoords, const std::vector<Vector2D>& enemiesTileCoords);
    ~LevelManager();

    
    const std::array<const char*, 10> characterTextures = {"assets/char0.png", "assets/char1.png", "assets/char2.png", "assets/char3.png", "assets/char4.png", "assets/char5.png",
"assets/char6.png", "assets/char7.png", "assets/char8.png", "assets/char9.png"};

    void loadCharacters(Level level, GameMode gmd);


    
private:

    void addChars(Side side, size_t chr, size_t powerCoef, size_t charsNum);
    void loadShop();
    void loadEnemies();
    std::vector<Vector2D> shopTileCoords; // This is a copy because an original vector is destroyed after init() function is done
    std::vector<Vector2D> enemiesTileCoords;
    nlohmann::json jCharacters;
    Manager* manager;
    
};