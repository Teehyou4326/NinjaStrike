#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <memory>

#include "Player.h"
#include "Map.h"
#include "Enemy.h"
#include "Potion.h"
#include "EnemyAI.h"

class Game
{
public:
    Game();
    ~Game();

    bool init(const char* title);
    void handleEvent(SDL_Event& e);
    void update();
    void render();
    void clean();

    bool isRunning();
    void present() { SDL_RenderPresent(renderer); }
    SDL_Renderer* getRenderer() const { return renderer; }
    void clear() { SDL_RenderClear(renderer); }
    void quit() { running = false; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Uint32 lastTime;
    double deltaTime;

    Player player;
    Map gameMap;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Potion>> potions;

    Texture hpBar;
    Texture stateBar;
    Texture scoreBoard;

    Texture DmgBoostIcon;
    Texture ReverseControlIcon;
    Texture InvincibleIcon;

};

#endif
