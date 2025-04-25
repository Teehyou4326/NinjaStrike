#include "SoundManager.h"
#include <iostream>

SoundManager& SoundManager::getInstance()
{
    static SoundManager instance;
    return instance;
}

bool SoundManager::loadSound(const std::string& id, const std::string& path)
{
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if(!sound)
    {
        std::cout << "Failed to load sound: " << path << " Error: " << Mix_GetError() << std::endl;
        return false;
    }
    soundMap[id] = sound;
    return true;
}

void SoundManager::playSound(const std::string& id, int loops)
{
    auto it = soundMap.find(id);
    if(it != soundMap.end())
    {
        Mix_PlayChannel(-1, it->second, loops);
    }
}

void SoundManager::cleanUp()
{
    for(auto& pair : soundMap)
    {
        Mix_FreeChunk(pair.second);
    }
    soundMap.clear();
}
