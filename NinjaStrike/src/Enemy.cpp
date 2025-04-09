#include "Enemy.h"
#include "EnemyAI.h"
#include "Map.h"

Enemy::Enemy(int x, int y)
{
    x = x;
    y = y;
    velX = 0;
    velY = 0;
    enemyHeight = 64;
    enemyWidth = 64;
    facingLeft = true;
    currentState = ENEMY_IDLE;

    idleSheet("","");
    walkSheet("","");
    attackSheet("","");
    hurtSheet("","");
    deadSheet("","");

    EnemyAI ai;
    ai = EnemyAI(patrolStartX, patrolEndX, detectionRange, &map);
}

Enemy::~Enemy(){}

void Enemy::update(float deltaTime)
{
    x += velX * deltaTime;

    switch (currentState)
    {
        case ENEMY_IDLE: idleSheet; break;
        case ENEMY_WALK: walkSheet; break;
        case ENEMY_ATTACK: attackSheet; break;
        case ENEMY_HURT: hurtSheet; break;
        case ENEMY_DEAD: deadSheet; break;
    }

    ai.update(position.x, velocity.x, player.x, position.y + enemyHeight, facingLeft);
}

SDL_Rect Enemy::getCollisionBox() const
{
    return SDL_Rect{ static_cast<int>(x), static_cast<int>(y), enemyWidth, enemyHeight};
}

void Enemy::changeState(State newState)
{
    if(currentState != newState)
    {
        currentState = newState;
    }
}

SDL_Rect Enemy::getHitbox() const
{
    return SDL_Rect{x, y, enemyWidth, enemyHeight};
}

void Enemy::takeDamage()
{
    std::cout << "enemy -- hp" << std::endl;
}
