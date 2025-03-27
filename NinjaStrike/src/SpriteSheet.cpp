#include "SpriteSheet.h"

#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;


SpriteSheet::SpriteSheet(){}

SpriteSheet::~SpriteSheet()
{
    clean();
}

bool SpriteSheet::load(SDL_Renderer* renderer, const char* imagePath, const char* jsonPath)
{
    if(!texture.load(renderer, imagePath))
    {
        std::cout << "Failed to load texture " << imagePath << std::endl;
        return false;
    }

    std::ifstream file(jsonPath);
    if(!file.is_open())
    {
        std::cout << "Failed to open " << jsonPath << std::endl;
        return false;
    }

    json root;
    file >> root;
    file.close();

    for(const auto& frameName : root["frames"].items())
    {
        const auto& frameData = frameName.value()["frame"];

        if (frameData.contains("x") && frameData.contains("y") &&
            frameData.contains("w") && frameData.contains("h"))
        {
            Frame frame;
            frame.x = frameData["x"].get<int>();
            frame.y = frameData["y"].get<int>();
            frame.w = frameData["w"].get<int>();
            frame.h = frameData["h"].get<int>();
            frames.push_back(frame);
        }
        else
        {
            std::cout << "Missing frame data in JSON for " << frameName.key() << std::endl;
        }
    }

    return true;
}

void SpriteSheet::update(float dt)
{
    frameTimer += dt;
    if(frameTimer >= frameSpeed)
    {
        frameTimer = 0.0f;
        frameIndex = (frameIndex + 1) % frames.size();
    }
}

void SpriteSheet::draw(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip)
{
    if(frameIndex < 0 || static_cast<size_t>(frameIndex) >= frames.size()) return;

    SDL_Rect srcRect = {frames[frameIndex].x, frames[frameIndex].y, frames[frameIndex].w, frames[frameIndex].h};
    SDL_Rect dstRect = {x, y, frames[frameIndex].w, frames[frameIndex].h };

    texture.draw(renderer, &srcRect, &dstRect, flip);
}

void SpriteSheet::setFrame(int index)
    {
        if(index >= 0 && index < static_cast<int>(frames.size()) )
        {
            frameIndex = index;
            frameTimer = 0;
        }
    }

void SpriteSheet::clean()
{
    texture.clean();
    frames.clear();
}

int SpriteSheet::getFrameCount() const
{
    return frames.size();
}
