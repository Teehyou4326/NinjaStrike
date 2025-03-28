#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Player.h"
#include "Config.h"

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

    Player player;
    Uint32 lastTime;
    double deltaTime;
};

#endif
