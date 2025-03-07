#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "../include/RenderWindow.h"
int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_VIDEO) > 0)
        std::cout << "SDL_Init has failed. SDL_Error: " << SDL_GetError() << std::endl;
    if(!(IMG_Init(IMG_INIT_PNG)))
        std::cout << "IMG_Init has failed, Error: " << SDL_GetError() << std::endl;

    RenderWindow window("Ninja Strike", 1280, 720);

    bool gameRunning = true;

    SDL_Event event;

    while (gameRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT) gameRunning == false;
        }
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}
