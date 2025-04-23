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
        enemy->setAI(std::make_unique<EnemyAI>(enemy.get(), &player, 600, 600, 25, 250.0f, &gameMap));

        enemies.push_back(enemy);
    }

    //spawn potion
    std::vector<SDL_Point> spawnPotionPoints = gameMap.getPotionSpawnPoints();
    for(const auto& pos : spawnPotionPoints)
    {
        auto potion = std::make_shared<Potion>();

        if(!potion->load(renderer))
        {
            std::cout << "Potion load that bai" << std::endl;
            return false;
        }

        potion->setPosition(pos.x, pos.y);
        potion->setMap(&gameMap);

        potions.push_back(potion);
    }

    if(!hpBar.load(renderer, "res/HUD/HealthBar.png"))
    {
        std::cout << "Load HP bar that bai" << std::endl;
        return false;
    }

    running = true;
    lastTime = SDL_GetTicks();

    return true;
}

void Game::handleEvent(SDL_Event& event)
{
        if (event.type == SDL_QUIT)
            running = false;

        player.handleInput(event);
}

void Game::update()
{
    const int frameDelay = 1000/60;

    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    if(deltaTime > 0.1) deltaTime = 0.1;

    player.update(deltaTime);

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                                 [](const std::shared_ptr<Enemy>& enemy)
                                 {
                                     return enemy->isDead();
                                 }),
                  enemies.end());

    for(auto& enemy : enemies)
    {
        enemy->update(deltaTime);

        //enemy attack - player
        if(enemy->getState() == State::Attack)
        {
            if(Collision::checkCollision(player.getHitbox(), enemy->attackHitbox()))
            {
                player.takeDamage(70);
            }
        }

        //shuriken - enemy
        for(auto& shuriken : player.getShurikens())
        {
            if(Collision::checkCollision(shuriken.getHitbox(), enemy->getHitbox()))
            {
                enemy->takeDamage(playerDMG / 2);
            }
        }

        //player attack - enemy
        if(player.getState() == PlayerState::Attacking)
        {
            if(Collision::checkCollision(player.attackHitbox(), enemy->getHitbox()))
            {
                enemy->takeDamage(playerDMG);
            }
        }
    }

    potions.erase(std::remove_if(potions.begin(), potions.end(),
                                 [](const std::shared_ptr<Potion>& potion)
                                 {
                                     return potion->claimed();
                                 }),
                  potions.end());

    for(auto& potion : potions)
    {
        potion->update(deltaTime);

        if(Collision::checkCollision(player.getHitbox(), potion->getHitbox()))
        {
            potion->claim();
            potion->applyEffect(&player);
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

    //draw map, player, enemy, potion.
    gameMap.draw(renderer);
    gameMap.updateCamera();
    player.draw(renderer);
    for(auto& enemy : enemies)
    {
        enemy->draw(renderer);
    }
    for(auto& potion : potions)
    {
        potion->draw(renderer);
    }

    //draw HP bar
    int barX = 20;
    int barY = 20;
    int barW = 200;
    int barH = 30;

    float hpPercent = static_cast<float>(player.getHP()) / 500;
    int fillW = static_cast<int>(barW * hpPercent);

    SDL_Rect hpFillRect = {barX + 30, barY + 7, fillW / 1.205f, barH - 10};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hpFillRect);
    hpBar.draw(renderer, barX, barY, barW, barH);

    gameMap.drawCollisionTiles(renderer);
    player.drawHitbox(renderer);
    for(auto& enemy : enemies)
    {
        enemy->drawHitbox(renderer);
    }
    for(auto& potion : potions)
    {
        potion->drawHitbox(renderer);
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
