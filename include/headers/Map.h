#pragma once
#include "EnumManager.h"
#include "Vector2D.h"
#include <string>
#include <vector>
#include <utility>

class Map{
private:

    void addTile(int srcX, int srcY, int xPos, int yPos, GroupLabels grp, TileStatus tileStat);

    
    std::string terrainTileSetID;
    std::string overlayTileSetID; 
    int tileSize;

public:

    Map(const std::string& tID, const std::string& oID, int ts);
    ~Map();
    


    void loadMap(const std::string& pathTerrain, const std::string& pathTileOverlay, std::vector<Vector2D>& playerBoardCoords, std::vector<Vector2D>& shopTileCoords, std::vector<Vector2D>& enemiesTileCoords, int sizeX, int sizeY);



};