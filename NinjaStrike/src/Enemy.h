#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include "SpriteSheet.h"

class Enemy
{
public:
    Enemy(int x, int y, SDL_Renderer* renderer);
    ~Enemy();

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);

    SDL_Rect getCollisionBox() const;
    SDL_Rect getHitbox() const;
    void takeDamage();
private:
    float x, y;
    float velX, velY;
    int enemyWidth, enemyHeight;
    bool facingLeft;

    enum State
    {
        ENEMY_IDLE,
        ENEMY_WALK,
        ENEMY_ATTACK,
        ENEMY_HURT,
        ENEMY_DEAD
    };
    State currentState;

    SpriteSheet idleSheet;
    SpriteSheet walkSheet;
    SpriteSheet attackSheet;
    SpriteSheet hurtSheet;
    SpriteSheet deadSheet;

    void changeState(State newState);
};
#endif // ENEMY_H
