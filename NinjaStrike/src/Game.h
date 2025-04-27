#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
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

    bool isGameOver() const { return gameOver; }
    int getScore() const { return score; }
    void reset();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    TTF_Font* font = nullptr;
    void drawScore(SDL_Renderer*renderer, int score);

    Uint32 lastTime;
    double deltaTime;

    Player player;
    Map gameMap;
    Texture backgroundTexture;

    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Potion>> potions;

    bool spawnEnemies();
    bool spawnPotions();

    int distanceTravelled = 0;
    int enemiesDefeated = 0;
    int potionsCollected = 0;
    int score = 0;
    bool gameOver = false;

    Texture hpBar;
    Texture stateBar;
    Texture scoreBoard;

    Texture DmgBoostIcon;
    Texture ReverseControlIcon;
    Texture InvincibleIcon;
    Texture StunIcon;
    Texture SpeedBoostIcon;

};

#endif
