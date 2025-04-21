#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <vector>
#include <memory>

#include "Map.h"
#include "SpriteSheet.h"
#include "Config.h"

class EnemyAI;

class Enemy
{
public:
    Enemy();
    ~Enemy();

    bool load(SDL_Renderer* renderer);
    void update(float dt);
    void draw(SDL_Renderer* renderer);
    void clean();
    void setPosition(int x, int y);
    void setMap(Map* map) { this->map = map; };

    void takeDamage();
    SDL_Rect getHitbox() const;

    float x, y, dx, dy;

    std::unique_ptr<EnemyAI> ai;
    void setAI(std::unique_ptr<EnemyAI> newAI);
    float getX() const { return x; }
    float getY() const { return y; }

    void drawHitbox(SDL_Renderer* renderer);
private:
    int offsetX = 50;
    int offsetY = 33;

    enum class State { Idle, Walk, Attack, Hurt, Dead};

    SpriteSheet idleSheet;
    SpriteSheet walkSheet;
    SpriteSheet attackSheet;
    SpriteSheet hurtSheet;
    SpriteSheet deadSheet;

    State state;
    int prevX, prevY;

    Map* map = nullptr;

    int count = 0;
};

#endif // ENEMY_H
