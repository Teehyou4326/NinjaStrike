#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <set>
#include "json.hpp"

#include "Texture.h"
#include "Collision.h"

struct TileSet
{
    Texture texture;
    int firstGid;
    int tileWidth;
    int tileHeight;
    int columns;
    int tileCount;
};

class Map
{
public:
    Map();
    ~Map();

    bool loadMap(SDL_Renderer* renderer, const std::string& path);
    void draw(SDL_Renderer* renderer);
    bool checkCollision(const SDL_Rect& rect);

    int cameraX = 0;
    void updateCamera() { cameraX += 2 ; };

    void drawCollisionTiles(SDL_Renderer* renderer);

    std::vector<SDL_Point> getEnemySpawnPoints() const;
    std::vector<SDL_Point> getPotionSpawnPoints() const;

private:
    std::vector<TileSet> tileSets;
    std::vector<std::vector<int>> tileData;
    std::vector<nlohmann::json> objectLayers;

    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    TileSet* findTileSetByGid(int gid);

    std::vector<std::vector<int>> collisionLayer;
    std::set<int> collidableTileIDs;

};

#endif // MAP_H
