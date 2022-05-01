#pragma once
#include <headers/Game.h>
#include <string>

class Map{
public:

    Map();
    ~Map();

    static void loadMap(std::string path, int sizeX, int sizeY);

private:

};