#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Texture.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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

    bool isRunning(){return running;}

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Texture ninja;
};

#endif
