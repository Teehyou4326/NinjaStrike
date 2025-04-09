#include "Game.h"
#include "Texture.h"
#include "Map.h"
#include "Collision.h"

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

    if(!player.init("res/Ninja/idle_0.png", renderer, startX, startY))
    {
        return false;
    }
    /**
    if(!gameMap.loadMap("res/map.json", "res/tileset.tsx", renderer))
    {
        std::cout << "K the tai map" << std::endl;
        return false;
    }
    */
    running = true;
    lastTime = SDL_GetTicks();

    return true;
}

void Game::handleEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) running = false;

        player.handleInput(event);
    }
}

void Game::update()
{
    const int frameDelay = 1000/60;

    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    if(deltaTime > 0.1) deltaTime = 0.1;

    player.update(deltaTime);

    for(Enemy& enemy : enemies)
    {
        enemy.update(deltaTime);

        if(Collision::checkCollision(player.getHitbox(), enemy.getHitbox()))
        {
            player.takeDamage();
        }

        for(auto& shuriken : player.getShurikens())
        {
            if(Collision::checkCollision(shuriken.getHitbox(), enemy.getHitbox()))
            {
                enemy.takeDamage();
                shuriken.setInactive();
            }
        }
    }

    Uint32 frameTime = SDL_GetTicks() - currentTime;
    if(frameDelay > frameTime)
    {
        SDL_Delay(frameDelay - frameTime);
    }

}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer , 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    gameMap.render(renderer);
    player.draw(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    player.clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::isRunning()
{
    return running;
}
