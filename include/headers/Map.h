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
    
    //using TileStatusIndexPair = std::pair<TileStatus, size_t>;
    //using TileStatusVector = std::vector<std::vector<TileStatusIndexPair>>; // For ENEMYBOARD and PLAYERBOARD size_t is an index of character in the according indices arrays
    //TileStatusVector tileStatuses; 
    //std::vector<int> playerBoardCharIndices; // reflect an index of the character at the tile in the groupPlayerBoard, -1 if tile is empty
    //std::vector<Vector2D> shopTransforms;
    //std::vector<Vector2D> enemyTilesTransforms;

    void loadMap(const std::string& pathTerrain, const std::string& pathTileOverlay, std::vector<Vector2D>& shopTileCoords, std::vector<Vector2D>& enemiesTileCoords, int sizeX, int sizeY);



};