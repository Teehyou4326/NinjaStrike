#include "Game.h"

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
///
    window = SDL_CreateWindow(title , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
///
    //load map
    if(!gameMap.loadMap(renderer, "res/map/map0.tmj"))
    {
        std::cout << "Load spawn map fail" << std::endl;
        return false;
    }

    //truyen map vao player
    player.setMap(&gameMap);

    //khoi tao player
    if(!player.init("res/Ninja/idle_0.png", renderer, startX, startY))
    {
        return false;
    }

    //spawn enemy
    std::vector<SDL_Point> spawnPoints = gameMap.getEnemySpawnPoints();
    for(const auto& pos : spawnPoints)
    {
        auto enemy = std::make_shared<Enemy>();

        if(!enemy->load(renderer))
        {
            std::cout << "Enemy load that bai" <<std::endl;
            return false;
        }

        enemy->setPosition(pos.x, pos.y);
        enemy->setMap(&gameMap);

        enemies.push_back(enemy);
    }

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

    for(auto& enemy : enemies)
    {
        enemy->update(deltaTime);

        if(Collision::checkCollision(player.getHitbox(), enemy->getHitbox()))
        {
            player.takeDamage();
        }

        for(auto& shuriken : player.getShurikens())
        {
            if(Collision::checkCollision(shuriken.getHitbox(), enemy->getHitbox()))
            {
                enemy->takeDamage();
            }
        }

        if(player.getState() == PlayerState::Attacking)
        {
            if(Collision::checkCollision(player.attackHitbox(), enemy->getHitbox()))
            {
                enemy->takeDamage();
            }
        }
    }

    Uint32 frameTime = SDL_GetTicks() - currentTime;
    if(frameDelay > frameTime)
    {
        SDL_Delay(frameDelay - frameTime);
    }

    gameMap.cameraX += static_cast<int>(10 * deltaTime);
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer , 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    gameMap.draw(renderer);
    gameMap.updateCamera();
    player.draw(renderer);
    for(auto& enemy : enemies)
    {
        enemy->draw(renderer);
    }

    gameMap.drawCollisionTiles(renderer);
    player.drawHitbox(renderer);
    for(auto& enemy : enemies)
    {
        enemy->drawHitbox(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    player.clean();
    for(auto& enemy : enemies)
    {
        enemy->clean();
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

bool Game::isRunning()
{
    return running;
}
