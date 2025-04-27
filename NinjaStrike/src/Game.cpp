#include "Game.h"
#include "SoundManager.h"

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

    //font
    if(TTF_Init() == -1)
    {
        std::cout << "TTF Init failed: " << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("res/font/s_font.ttf", 28);
    if(!font)
    {
        std::cout << "Load font fail " << TTF_GetError() << std::endl;
    }

    //load map
    if(!gameMap.loadMap(renderer, "res/map/map1.tmj"))
    {
        std::cout << "Load spawn map fail" << std::endl;
        return false;
    }
    if(!backgroundTexture.load(renderer, "res/map/BG/BG3.png"))
    {
        std::cout << "load bg fail" << std::endl;
        return false;
    }

    //truyen map vao player
    player.setMap(&gameMap);

    //khoi tao player
    if(!player.init("res/Ninja/idle_0.png", renderer, startX, startY))
    {
        return false;
    }

    spawnEnemies();
    spawnPotions();

    //load HUD
    if(!hpBar.load(renderer, "res/HUD/HealthBar.png"))
    {
        std::cout << "Load HP bar that bai" << std::endl;
        return false;
    }
    if(!stateBar.load(renderer, "res/HUD/StateBar.png"))
    {
        std::cout << "Load state bar fail " << std::endl;
        return false;
    }
    if(!scoreBoard.load(renderer, "res/HUD/ScoreBoard.png"))
    {
        std::cout << "Load score board fail" << std::endl;
        return false;
    }

    //Icon
    if(!DmgBoostIcon.load(renderer, "res/HUD/Icons/DmgBoost.png") ||
       !ReverseControlIcon.load(renderer, "res/HUD/Icons/ReverseControl.png") ||
       !InvincibleIcon.load(renderer, "res/HUD/Icons/Invincible.png") ||
       !StunIcon.load(renderer, "res/HUD/Icons/Stun.png") ||
       !SpeedBoostIcon.load(renderer, "res/HUD/Icons/SpeedBoost.png"))
    {
        std::cout << "load icon fail" << std::endl;
        return false;
    }

    //sound
    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
    {
        std::cerr << "K mo dc audio " << Mix_GetError() << std::endl;
    }

    SoundManager::getInstance().loadSound("attack", "res/sound/attack.wav");
    SoundManager::getInstance().loadSound("claim", "res/sound/claim.wav");
    SoundManager::getInstance().loadSound("jump", "res/sound/jump.wav");
    SoundManager::getInstance().loadSound("throw", "res/sound/throw.wav");
    SoundManager::getInstance().loadSound("run", "res/sound/run.wav");

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
    if(player.getX() < -35 || player.getY() > SCREEN_HEIGHT || player.hp < 0)
    {
        gameOver = true;
    }

    const int frameDelay = 1000/60;

    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0;
    lastTime = currentTime;

    if(deltaTime > 0.1) deltaTime = 0.1;

    //score
    distanceTravelled = std::max(distanceTravelled, int(player.x) + gameMap.cameraX - startX);
    score = distanceTravelled*0.05 + enemiesDefeated*100 + potionsCollected*50;

    //object
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
                player.takeDamage(75);
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

        if(enemy->isDying() && !enemy->defeatedCounter)
        {
            enemiesDefeated ++;
            enemy->defeatedCounter = true;
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
            potionsCollected ++;
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

void Game::drawScore(SDL_Renderer* renderer, int score)
{
    //labelscore
    SDL_Color labelColor = {255, 255, 255};
    SDL_Surface* labelSurface = TTF_RenderText_Solid(font, "Score", labelColor);
    SDL_Texture* labelTexture = SDL_CreateTextureFromSurface(renderer, labelSurface);
    SDL_Rect labelRect = {1139, 35, labelSurface->w, labelSurface->h};

    //score
    SDL_Color color = {255, 255, 0};
    std::string text = std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = {1176 - surface->w/2, 63, surface->w, surface->h};

    SDL_RenderCopy(renderer, labelTexture, nullptr, &labelRect);
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(labelSurface);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(labelTexture);
    SDL_DestroyTexture(texture);
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer , 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    //draw map, player, enemy, potion.
    backgroundTexture.draw(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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

    //draw HUD
    int barX = 30;
    int barY = 20;
    int barW = 220;
    int barH = 30;

    float hpPercent = static_cast<float>(player.hp) / 600;
    int fillW = static_cast<int>(barW * hpPercent);

    SDL_Rect hpFillRect = {barX + 33, barY + 7, static_cast<int>(fillW / 1.2f), barH - 10};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hpFillRect);
    hpBar.draw(renderer, barX, barY, barW, barH);

    stateBar.draw(renderer, -12, 20, 500, 130);
    scoreBoard.draw(renderer, 1050, 0, 250, 180);

    //score
    drawScore(renderer, score);

    //icon
    if(player.SpeedBoostFlag)
        SpeedBoostIcon.draw(renderer, 42, 67, 44, 44);
    if(player.DmgBoostFlag)
        DmgBoostIcon.draw(renderer, 84, 67, 32, 32);
    if(player.ReverseControlFlag)
        ReverseControlIcon.draw(renderer, 114, 60, 44, 44);
    if(player.InvincibleFlag)
        InvincibleIcon.draw(renderer, 158, 68, 26, 33);
    if(player.StunFlag)
        StunIcon.draw(renderer, 172, 48, 72, 72);

//    gameMap.drawCollisionTiles(renderer);
//    player.drawHitbox(renderer);
//    for(auto& enemy : enemies)
//    {
//        enemy->drawHitbox(renderer);
//    }
//    for(auto& potion : potions)
//    {
//        potion->drawHitbox(renderer);
//    }

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

bool Game::spawnEnemies()
{
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
    return true;
}

bool Game::spawnPotions()
{
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
    return true;
}

void Game::reset()
{
    distanceTravelled = 0;
    enemiesDefeated = 0;
    potionsCollected = 0;
    gameOver = false;

    player.hp = 600;
    player.x = startX;
    player.y = startY;
    player.dx = 0;
    player.dy = 0;
    player.isJumping = false;
    player.doubleJump = false;
    player.state = PlayerState::Idle;
    player.shurikenCooldown = 0;

    player.SpeedBoostFlag = false;
    player.DmgBoostFlag = false;
    player.InvincibleFlag = false;
    player.ReverseControlFlag = false;
    player.StunFlag = false;

    enemies.clear();
    potions.clear();

    gameMap.setCameraX();
    spawnEnemies();
    spawnPotions();

    isResetDone = true;
}
