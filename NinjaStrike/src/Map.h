#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <string>
#include <vector>
#include "Texture.h"
#include "json.hpp"
#include "tinyxml2.h"

class Map
{
public:
    Map();
    ~Map();

    bool loadMap(const std::string& jsonFile, const std::string& tsxFile, SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);

    bool isSolidAt(float worldX, float worldY) const;

private:
    int mapWidth, mapHeight, tileSize;
    std::vector<std::vector<int>> tiles;
    Texture tileSet;

    bool loadTileSet(const std::string& tsxFile, SDL_Renderer* renderer);
};
#endif // MAP_H
