#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <vector>
#include <memory>

#include "Map.h"
#include "SpriteSheet.h"
#include "Config.h"

enum class State { Idle, Walk, Attack, Hurt, Dead};

class EnemyAI;

class Enemy
{
friend class Potion;

public:
    Enemy();
    ~Enemy();

    bool load(SDL_Renderer* renderer);
    void update(float dt);
    void draw(SDL_Renderer* renderer);
    void clean();

    void setPosition(int x, int y);
    void setMap(Map* map) { this->map = map; };

    void takeDamage(int type);
    SDL_Rect getHitbox() const;
    State getState() const { return state; }

    float x, y, dx, dy;

    std::unique_ptr<EnemyAI> ai;
    void setAI(std::unique_ptr<EnemyAI> newAI);
    float getX() const { return x + offsetX; }
    float getY() const { return y + offsetY; }

    void attack();
    SDL_Rect attackHitbox() const;

    bool isDead() const { return isDeadFlag; };
    bool isDying() const { return isDyingFlag; };
    bool defeatedCounter = false;

    void drawHitbox(SDL_Renderer* renderer);

    int defeated = 0;
private:
    int offsetX = 52;
    int offsetY = 33;

    SpriteSheet idleSheet;
    SpriteSheet walkSheet;
    SpriteSheet attackSheet;
    SpriteSheet hurtSheet;
    SpriteSheet deadSheet;

    State state;
    int prevX, prevY;

    bool facingRight = true;

    Map* map = nullptr;

    int hp = 100;
    bool isDyingFlag = false;
    bool isDeadFlag = false;

    Uint32 lastHitTime = 0;
    Uint32 damageCooldown = 400;
};

#endif // ENEMY_H
