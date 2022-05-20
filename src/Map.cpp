#include "Map.h"
#include "TextureManager.h"
#include "headers/ECS/Components.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

extern Manager manager;

Map::Map(const std::string& tID, const std::string& oID, int ts) : terrainTileSetID(tID), overlayTileSetID(oID), tileSize(ts){}

Map::~Map(){}

void Map::loadMap(const std::string& pathTerrain, const std::string& pathTileOverlay, std::vector<Vector2D>& shopTileCoords, std::vector<Vector2D>& enemiesTileCoords, int sizeX, int sizeY)
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
            srcY = strtol(&c, nullptr, 10) * 32;
            terrainFile.get(c);
            srcX = strtol(&c, nullptr, 10) * 32;
            tileOverlayFile.get(c);
            ts = strtol(&c, nullptr, 10);
            tileStat = static_cast<TileStatus>(ts);
            if (tileStat == TileStatus::SHOP)
            {
                shopTileCoords.emplace_back(Vector2D(x, y));
            }
            else if (tileStat == TileStatus::ENEMYBOARD)
            {
                enemiesTileCoords.emplace_back(Vector2D(x, y));
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
