#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include "json.hpp"

#include "Texture.h"

struct TileSet
{
    Texture texture;
    int firstGid;
    int tileWidth;
    int tileHeight;
    int columns;
};

class Map
{
public:
    Map();
    ~Map();

    bool loadMap(SDL_Renderer* renderer, const std::string& path);
    void draw(SDL_Renderer* renderer);

    int cameraX = 0;
    int cameraY = 0;
private:
    std::vector<TileSet> tileSets;
    std::vector<std::vector<int>> tileData;
    std::vector<nlohmann::json> objectLayers;

    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    TileSet* findTileSetByGid(int gid);

};

#endif // MAP_H
