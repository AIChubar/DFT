#include "headers/Map.h"
#include "headers/TextureManager.h"
#include "headers/Game.h"
#include <fstream>
#include <iostream>
Map::Map()
{
}

Map::~Map()
{
    //SDL_DestroyTexture(...);
}

void Map::loadMap(std::string path, int sizeX, int sizeY)
{
    char tile;
    std::fstream mapFile;
    mapFile.open(path);

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(tile);
            std::cout << tile << ' ';
            Game::addTile(atoi(&tile), x*64, y*64); // Loading tile index from a map and converting it to int
            mapFile.ignore();
        }
        std::cout << '\n';
    }

    mapFile.close();
}
