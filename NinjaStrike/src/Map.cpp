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

            TileSet tileSet;
            tileSet.texture.load(renderer, ("res/map/" + imagePath).c_str());
            tileSet.firstGid = ts["firstgid"];
            tileSet.tileWidth = tsxTileWidth;
            tileSet.tileHeight = tsxTileHeight;
            tileSet.columns = columns;

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

    std::cout << "=== Object Layers ===" << std::endl;
    for (size_t i = 0; i < objectLayers.size(); ++i)
    {
        std::cout << "Object Layer " << i << ": " << std::endl;
        auto& objects = objectLayers[i]["objects"];
        for (const auto& obj : objects)
        {
            std::cout << "  ID: " << obj["id"];
            std::cout << ", Name: " << obj["name"];
            std::cout << ", Type: " << obj["type"];
            std::cout << ", X: " << obj["x"];
            std::cout << ", Y: " << obj["y"] << std::endl;
        }
    }


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
            dstRect.x = (i % mapWidth) * tileWidth - cameraX *0;
            dstRect.y = (i / mapWidth) * tileHeight - cameraY *0;

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

                ts->texture.draw(renderer, int(x) - cameraX, int(y) - cameraY, &srcRect);
            }
        }
    }
}

TileSet* Map::findTileSetByGid(int gid)
{
    TileSet* result = nullptr;
    for(auto& ts : tileSets)
    {
        if(gid >= ts.firstGid && gid < ts.firstGid + (ts.columns * ts.tileWidth))
        {
            result = &ts;
            break;
        }
    }
    return result;
}
