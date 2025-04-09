#include "Map.h"
#include <fstream>
#include <iostream>
#include "tinyxml2.h"

Map::Map()
{
    mapWidth = 0;
    mapHeight = 0;
    tileSize = 0;
}

Map::~Map(){}

bool Map::loadMap(const std::string& jsonFile, SDL_Renderer* renderer)
{
    std::ifstream file(jsonFile);
    if(!file.is_open())
    {
        std::cout << "K mo dc: " << jsonFile << std::endl;
        return false;
    }

    nlohmann::json mapData;
    file >> mapData;
    file.close();

    mapWidth = mapData["width"];
    mapHeight = mapData["height"];
    tileSize = mapData["tilewidth"];
    tileColumns = 0;

    if(!loadTileSet(tsxFile, renderer)) return false;

    tiles.resize(mapHeight, std::vector<int>(mapWidth,0));
    auto tileData = mapData["layers"][0]["data"];
    for(int y=0; y<mapHeight; y++)
    {
        for(int x=0; x<mapWidth; x++)
        {
            tiles[y][x] = tileData[y*mapWidth+x] - 1;
        }
    }
    return true;
}

bool Map::loadTileSet(const std::string& tsxFile, SDL_Renderer* renderer)
{
    tinyxml2::XMLDocument doc;
    if(doc.LoadFile(tsxFile.c_str()) != tinyxml2::XML_SUCCESS)
    {
        std::cout << "k mo dc tileset: " << tsxFile << std::endl;
        return false;
    }

    tinyxml2::XMLElement* tilesetElement = doc.FirstChildElement("tileset");
    if(!tilesetElement)
    {
        std::cout << "ko thay <tileset> trong file" << std::endl;
        return false;
    }

    tileSize = tilesetElement->IntAttribute("tilewidth");
    tileColumns = tilesetElement->IntAttribute("columns");

    std::string tilesetImage = tilesetElement->FirstChildElement("image")->Attribute("source");
    if(!tileSet.loadFromFile(tilesetImage, renderer))
    {
        std::cout << "ko mo dc: " << tilesetImage << std::endl;
        return false;
    }
    return true;
}

void Map::render(SDL_Renderer* renderer)
{
    for(int y=0; y<mapHeight; y++)
    {
        for(int x=0; x<mapWidth; x++)
        {
            int tileID = tiles[y][x];
            if(tileID < 0) continue;

            int tileX = tileID % tileColumns;
            int tileY = tileID / tileColumns;

            SDL_Rect srcRect = {tileX*tileSize, tileY*tileSize, tileSize, tileSize};
            SDL_Rect dstRect = {x*tileSize, y*tileSize, tileSize, tileSize};

            tileSet.draw(renderer, srcRect, dstRect, flip);
        }
    }
}

bool Map::isSolidAt(float worldX, float worldY) const
{
    if(tileSize == 0) return false;

    int tileX = static_cast<int>(worldX) / tileSize;
    int tileY = static_cast<int>(worldY) / tileSize;

    if(tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight) return false;

    int tileID = tiles[tileY][tileX];
    return tileID != -1 && tileID != 0;
}
