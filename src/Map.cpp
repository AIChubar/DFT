#include "Map.h"
#include "TextureManager.h"
#include "ECS/ECS.h"
#include "ECS/TileComponent.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

extern Manager manager;

Map::Map(const std::string& tID, const std::string& oID, int ts) : terrainTileSetID(tID), overlayTileSetID(oID), tileSize(ts){}

Map::~Map(){}

void Map::loadMap(const std::string& pathTerrain, const std::string& pathTileOverlay, std::vector<Vector2D>& playerBoardCoords, std::vector<Vector2D>& shopTileCoords, std::vector<Vector2D>& enemiesTileCoords, int sizeX, int sizeY)
{
    char c;
    std::fstream terrainFile;
    std::fstream tileOverlayFile;
    terrainFile.open(pathTerrain);

    tileOverlayFile.open(pathTileOverlay);

    int srcX, srcY;
    size_t ts;
    TileStatus tileStat;
    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            terrainFile.get(c);
            srcY = std::stoi(std::string(1, c)) * 64; //for some reason atoi or strtol was not working for me
            terrainFile.get(c);
            srcX = std::stoi(std::string(1, c)) * 64;
            tileOverlayFile.get(c);
            ts = std::stoi(std::string(1, c));
            
            tileStat = static_cast<TileStatus>(ts);
            if (tileStat == TileStatus::SHOP)
            {
                shopTileCoords.emplace_back(Vector2D(x, y));
            }
            else if (tileStat == TileStatus::ENEMYBOARD)
            {
                enemiesTileCoords.emplace_back(Vector2D(x, y));
            }
            else if (tileStat == TileStatus::PLAYERBOARD)
            {
                playerBoardCoords.emplace_back(Vector2D(x, y));
            }
            addTile(srcX, srcY, x*tileSize, y*tileSize, GroupLabels::groupMAP, tileStat);
            terrainFile.ignore();
            tileOverlayFile.ignore();
        }
    }

    terrainFile.close();
    tileOverlayFile.close();
}

void Map::addTile(int srcX, int srcY, int xPos, int yPos, GroupLabels grp, TileStatus tileStat)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, terrainTileSetID, overlayTileSetID, tileStat);
    tile.addGroup(grp);
}
