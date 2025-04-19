#include "Enemy.h"
#include "Config.h"
#include <cmath>

Enemy::Enemy() : x(0), y(0), state(State::Idle),  dx(0), dy(0) {}

Enemy::~Enemy() {}

bool Enemy::load(SDL_Renderer* renderer)
{
    if (!idleSheet.load(renderer, "res/enemy/idle.png", "res/enemy/idle.json")) return false;
    if (!walkSheet.load(renderer, "res/enemy/walk.png", "res/enemy/walk.json")) return false;
    if (!attackSheet.load(renderer, "res/enemy/attack.png", "res/enemy/attack.json")) return false;
    if (!hurtSheet.load(renderer, "res/enemy/hurt.png", "res/enemy/hurt.json")) return false;
    if (!deadSheet.load(renderer, "res/enemy/dead.png", "res/enemy/dead.json")) return false;
    return true;
}

void Enemy::update(float dt)
{

    if(dt == 0) return;

    if (state != State::Attack && state != State::Hurt && state != State::Dead)
    {
        if (dx != 0)
        {
            state = State::Walk;
        }
        else
        {
            state = State::Idle;
        }
    }

    x += dx * dt;
    y += dy * dt;

    if(y < groundY) dy += gravity * dt;
    else if( y >= groundY)
    {
        y = groundY;
        dy = 0;
    }

    switch (state)
    {
        case State::Idle:
            idleSheet.setSpeed(0.1f);
            idleSheet.update(dt);
            break;
        case State::Walk:
            walkSheet.setSpeed(0.08f);
            walkSheet.update(dt);
            break;
        case State::Attack:
            attackSheet.setSpeed(0.05f);
            attackSheet.update(dt);
            if (attackSheet.isAnimationFinished()) state = State::Idle;
            break;
        case State::Hurt:
            hurtSheet.setSpeed(0.02f);
            hurtSheet.update(dt);
            if (hurtSheet.isAnimationFinished()) state = State::Idle;
            break;
        case State::Dead:
            deadSheet.setSpeed(0.25f);
            deadSheet.update(dt);
            if (deadSheet.isAnimationFinished()) state = State::Idle;
            break;
    }
}

void Enemy::draw(SDL_Renderer* renderer)
{
    switch (state)
    {
    case State::Idle:
        idleSheet.draw(renderer, x, y);
        break;
    case State::Walk:
        walkSheet.draw(renderer, x, y);
        break;
    case State::Attack:
        attackSheet.draw(renderer, x, y);
        break;
    case State::Hurt:
        hurtSheet.draw(renderer, x, y);
        break;
    case State::Dead:
        deadSheet.draw(renderer, x, y);
        break;
    }
}

void Enemy::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Enemy::takeDamage()
{
    std::cout << "enemy --hp" << std::endl;
    state = State::Hurt;
    count ++;
    if(count == 3)
    {
        state = State::Dead;
        count = 0;
    }
}

SDL_Rect Enemy::getHitbox() const
{
    return SDL_Rect{static_cast<int>(x) + offsetX, static_cast<int>(y) + offsetY, enemyW, enemyH};
}
///debug
void Enemy::drawHitbox(SDL_Renderer* renderer)
{
    SDL_Rect enemyHitbox = getHitbox();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);  // Màu xanh lá, alpha = 100 (mờ)
    SDL_RenderFillRect(renderer, &enemyHitbox);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
