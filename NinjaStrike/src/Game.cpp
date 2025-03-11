#include "Game.h"
#include "Texture.h"

Game::Game()
{
    window = nullptr;
    renderer = nullptr;
    running = false;

}
Game::~Game()
{
    clean();
}

bool Game::init(const char* title)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
        }

    if(!IMG_Init(IMG_INIT_PNG))
    {
        std::cout << "IMG_init Error:" << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Ninja Strike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
        {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
        }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
        }

    if(!ninja.load("res/Ninja/idle_0.png", renderer))
    {
        return false;
    }

    running = true;

    return true;
}

void Game::handleEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) running = false;
    }
}

void Game::update()
{

}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer , 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    ninja.draw(renderer, 100, 100, 100, 64);
    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    ninja.clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
