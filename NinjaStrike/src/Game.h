#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "Player.h"
#include "Config.h"
#include "Map.h"
#include "Enemy.h"

class Game
{
public:
    Game();
    ~Game();

    bool init(const char* title);
    void handleEvent();
    void update();
    void render();
    void clean();

    bool isRunning();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Uint32 lastTime;
    double deltaTime;

    Player player;
    Map gameMap;

    std::vector<Enemy> enemies;
};

#endif
