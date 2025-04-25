#ifndef SOUNDMANAGER_H
#define SOUNGMANAGER_H

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>

class SoundManager
{
public:
    static SoundManager& getInstance();

    bool loadSound(const std::string& id, const std::string& path);
    void playSound(const std::string& id, int loops = 0);
    void cleanUp();

private:
    SoundManager() = default;
    ~SoundManager() = default;

    std::unordered_map<std::string, Mix_Chunk*> soundMap;

};

#endif // SOUNDMANAGER_H
