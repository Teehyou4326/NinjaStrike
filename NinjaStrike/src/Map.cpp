#include "Map.h"

#include <fstream>
#include <iostream>
#include "tinyxml2.h"

using json = nlohmann::json;

Map::Map(){}
Map::~Map(){}

bool Map::loadMap(SDL_Renderer* renderer, const std::string& path)
{
    std::ifstream inFile(path);
    if(!inFile.is_open())
    {
        std::cout << "K the mo file map: " << path << std::endl;
        return false;
    }

    json mapData;
    inFile >> mapData;

    mapWidth = mapData["width"];
    mapHeight = mapData["height"];
    tileWidth = mapData["tilewidth"];
    tileHeight = mapData["tileheight"];

    //load tileset
    for(auto& ts : mapData["tilesets"])
    {
        if(ts.contains("source"))
        {
            std::string tsxPath = "res/map/" + std::string(ts["source"]);

            tinyxml2::XMLDocument doc;
            if(doc.LoadFile(tsxPath.c_str()) != tinyxml2::XML_SUCCESS)
            {
                std::cout << "K the mo tileset: " << tsxPath << std::endl;
                continue;
            }

            tinyxml2::XMLElement* root = doc.RootElement();
            tinyxml2::XMLElement* imageElement = root->FirstChildElement("image");

            if(imageElement == nullptr)
            {
                std::cout << "Tileset thieu phan image trong file: " << tsxPath << std::endl;
                continue;
            }

            std::string imagePath = imageElement->Attribute("source");
            int tsxTileWidth = root->IntAttribute("tilewidth");
            int tsxTileHeight = root->IntAttribute("tileheight");
            int columns = imageElement->IntAttribute("width") / tsxTileWidth;

            int imageHeight = imageElement->IntAttribute("height");
            int rows = imageHeight / tsxTileHeight;

            TileSet tileSet;
            tileSet.texture.load(renderer, ("res/map/" + imagePath).c_str());

            tileSet.firstGid = ts["firstgid"];
            tileSet.tileWidth = tsxTileWidth;
            tileSet.tileHeight = tsxTileHeight;
            tileSet.columns = columns;
            tileSet.tileCount = columns * rows;

            tileSets.push_back(tileSet);

        }
    }

    //load layers
    for(auto& layer : mapData["layers"])
    {
        std::string type = layer["type"];
        if(type == "tilelayer")
        {
            std::vector<int> layerData = layer["data"];
            tileData.push_back(layerData);
        }
        else if(type == "objectgroup")
        {
            objectLayers.push_back(layer);
        }
    }

    for (const auto& layer : tileData)
    {
        for (int gid : layer)
        {
            if (gid > 0)
                collidableTileIDs.insert(gid);
        }
    }

    std::cout << "=== Tile Data ===" << std::endl;
    for (size_t layerIndex = 0; layerIndex < tileData.size(); ++layerIndex)
    {
        std::cout << "Layer " << layerIndex << ":" << std::endl;
        const auto& layer = tileData[layerIndex];
        for (size_t i = 0; i < layer.size(); ++i)
        {
            std::cout << layer[i] << " ";
            if ((i + 1) % mapWidth == 0)
                std::cout << std::endl;
        }
    }

//    std::cout << "=== Object Layers ===" << std::endl;
//    for (size_t i = 0; i < objectLayers.size(); ++i)
//    {
//        std::cout << "Object Layer " << i << ": " << std::endl;
//        auto& objects = objectLayers[i]["objects"];
//        for (const auto& obj : objects)
//        {
//            std::cout << "  ID: " << obj["id"];
//            std::cout << ", Name: " << obj["name"];
//            std::cout << ", Type: " << obj["type"];
//            std::cout << ", X: " << obj["x"];
//        C:\Code\NinjaStrike\NinjaStrike\src\Player.cpp    std::cout << ", Y: " << obj["y"] << std::endl;
//        }
//    }


    return true;

}

void Map::draw(SDL_Renderer* renderer)
{
    // draw tile layer;
    for(size_t layerIndex = 0; layerIndex < tileData.size(); layerIndex++)
    {
        const auto& data = tileData[layerIndex];

        for(size_t i=0; i < data.size(); i++)
        {
            int gid = data[i];
            if(gid == 0) continue;

            TileSet* ts = findTileSetByGid(gid);
            if(!ts)
            {
                std::cout << "k thay tileset cho gid: " << gid << std::endl;
                continue;
            }

            int localID = gid - ts->firstGid;
            int col = localID % ts->columns;
            int row = localID / ts->columns;

            SDL_Rect srcRect = {
              col * ts->tileWidth,
              row * ts->tileHeight,
              ts->tileWidth,
              ts->tileHeight
            };

            SDL_Rect dstRect;
            dstRect.x = (i % mapWidth) * tileWidth - cameraX ;
            dstRect.y = (i / mapWidth) * tileHeight;

            if(!ts->texture.isValid())
            {
                std::cout << "[Map::draw] Texture null cho GID: " << gid << std::endl;
            }

            ts->texture.draw(renderer, dstRect.x, dstRect.y, &srcRect);
        }
    }

    //draw object layer
    for(auto& layer : objectLayers)
    {
        for(auto& obj : layer["objects"])
        {
            if(obj.contains("gid"))
            {
                int gid = obj["gid"];
                TileSet* ts = findTileSetByGid(gid);
                if(!ts) continue;

                int localID = gid - ts->firstGid;
                int col = localID % ts->columns;
                int row = localID / ts->columns;

                SDL_Rect srcRect ={
                    col * ts->tileWidth,
                    row * ts->tileHeight,
                    ts->tileWidth,
                    ts->tileHeight
                };

                float x = obj["x"];
                float y = obj["y"];
                y -= ts->tileHeight;

                if(!ts->texture.isValid())
                {
                    std::cout << "[Map::draw] Texture null cho GID: " << gid << std::endl;
                }

                //ts->texture.draw(renderer, int(x) - cameraX, int(y) - tileHeight, &srcRect);
            }
        }
    }
}

TileSet* Map::findTileSetByGid(int gid)
{
    TileSet* result = nullptr;
    for(auto& ts : tileSets)
    {
        if(gid >= ts.firstGid && gid < ts.firstGid + ts.tileCount)
        {
            result = &ts;
            break;
        }
    }
    return result;
}

bool Map::checkCollision(const SDL_Rect& rect)
{
    int startX = (rect.x + cameraX) / tileWidth;
    int endX = (rect.x + rect.w + cameraX) / tileWidth ;
    int startY = rect.y / tileHeight;
    int endY = (rect.y + rect.h ) / tileHeight ;

    for(size_t layerIndex = 0; layerIndex < tileData.size(); layerIndex++)
    {
        const auto& layer = tileData[layerIndex];
        for(int y = startY; y <= endY; y++)
        {
            for(int x = startX; x <= endX; x++)
            {
                int index = y * mapWidth + x;

                if(x >= 0 && x < mapWidth && y >= 0 && y < mapHeight && index >= 0 && index < static_cast<int>(layer.size()))
                {
                    int tileID = layer[index];
                    if(collidableTileIDs.count(tileID))
                    {
                        SDL_Rect tileRect = {x*tileWidth - cameraX, y*tileHeight, tileWidth, tileHeight };
                        if(Collision::checkCollision(rect, tileRect))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

std::vector<SDL_Point> Map::getEnemySpawnPoints() const
{
    std::vector<SDL_Point> enemySpawns;

    for(const auto& layer : objectLayers)
    {
        for(const auto& obj : layer["objects"])
        {
            if(obj.contains("type") && obj["type"] == "enemy")
            {
                int x = static_cast<int>(obj["x"]);
                int y = static_cast<int>(obj["y"]);
                y -= tileHeight*3;
                x -= 22;
                enemySpawns.push_back({x,y});
            }
        }
    }
    return enemySpawns;
}

std::vector<SDL_Point> Map::getPotionSpawnPoints() const
{
    std::vector<SDL_Point> potionSpawns;

    for(const auto& layer : objectLayers)
    {
        for(const auto& obj : layer["objects"])
        {
            if(obj.contains("type") && obj["type"] == "potion")
            {
                int x = static_cast<int>(obj["x"]);
                int y = static_cast<int>(obj["y"]);
                y -= tileHeight*3;
                x += 7;
                y += 6;
                potionSpawns.push_back({x,y});
            }
        }
    }
    return potionSpawns;
}

bool Map::isWallAt(int x, int y)
{
    int tileX = (x + cameraX) / tileWidth;
    int tileY = y / tileHeight;

    if(tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight)
        return true;

    for(const auto& layer : tileData)
    {
        int index = tileY * mapWidth + tileX;
        if(index >= 0 && index < static_cast<int>(layer.size()))
        {
            if(collidableTileIDs.count(layer[index])) return true;
        }
    }
    return false;
}

bool Map::isGroundBelow(int x, int y)
{
    int tileX = (x + cameraX) / tileWidth;
    int tileY = y / tileHeight;

    if(tileX < 0 || tileX >= mapWidth || tileY < 0 || tileY >= mapHeight)
        return false;

    for(const auto& layer : tileData)
    {
        int index = tileY * mapWidth + tileX;
        if(index >= 0 && index < static_cast<int>(layer.size()))
        {
            if(collidableTileIDs.count(layer[index])) return true;
        }
    }
    return false;
}

void Map::clear()
{
    tileData.clear();
    tileSets.clear();
    collidableTileIDs.clear();
}


///vẽ vùng va chạm
//void Map::drawCollisionTiles(SDL_Renderer* renderer)
//{
//    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//    for (size_t layerIndex = 0; layerIndex < tileData.size(); ++layerIndex)
//    {
//        const auto& layer = tileData[layerIndex];
//        for (size_t i = 0; i < layer.size(); ++i)
//        {
//            int tileID = layer[i];
//            if (collidableTileIDs.count(tileID))
//            {
//                int x = (i % mapWidth) * tileWidth;
//                int y = (i / mapWidth) * tileHeight;
//                SDL_Rect tileRect = {x - cameraX, y, tileWidth, tileHeight};
//                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
//                SDL_RenderFillRect(renderer, &tileRect);
//            }
//        }
//    }
//    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
//}
