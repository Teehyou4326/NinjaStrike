#include "Enemy.h"

#include <cmath>
#include "EnemyAI.h"

Enemy::Enemy() : x(0), y(0), dx(0), dy(0), state(State::Idle) {}

Enemy::~Enemy() {}

bool Enemy::load(SDL_Renderer* renderer)
{
    if (!idleSheet.load(renderer, "res/enemy/idle.png", "res/enemy/idle.json"))
    {
        std::cout << "[Enemy] load idle sheet that bai" << std::endl;
        return false;
    }
    if (!walkSheet.load(renderer, "res/enemy/walk.png", "res/enemy/walk.json")) return false;
    if (!attackSheet.load(renderer, "res/enemy/attack.png", "res/enemy/attack.json")) return false;
    if (!hurtSheet.load(renderer, "res/enemy/hurt.png", "res/enemy/hurt.json")) return false;
    if (!deadSheet.load(renderer, "res/enemy/dead.png", "res/enemy/dead.json")) return false;
    return true;
}

void Enemy::update(float dt)
{
    if(ai) ai->update();

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

    prevX = x;
    prevY = y;

    x += dx * dt - 2;
    SDL_Rect futureXHitbox = {static_cast<int>(x) + offsetX, static_cast<int>(y) + offsetY, enemyW, enemyH};
    if(map && map->checkCollision(futureXHitbox))
    {
        x = prevX - 2;
    }

    y += dy * dt;
    SDL_Rect futureYHitbox = {static_cast<int>(x) + offsetX, static_cast<int>(y) + offsetY, enemyW, enemyH};
    if(map && map->checkCollision(futureYHitbox))
    {
        y = prevY;
        dy = 0;
    }

    dy += gravity * dt * 1.5f ;

    switch (state)
    {
        case State::Idle:
            idleSheet.setSpeed(0.12f);
            idleSheet.update(dt);
            break;
        case State::Walk:
            walkSheet.setSpeed(0.1f);
            walkSheet.update(dt);
            break;
        case State::Attack:
            attackSheet.setSpeed(0.08f);
            attackSheet.update(dt);
            if (attackSheet.isAnimationFinished()) state = State::Idle;
            break;
        case State::Hurt:
            hurtSheet.setSpeed(0.1f);
            hurtSheet.update(dt);
            if (hurtSheet.isAnimationFinished()) state = State::Idle;
            break;
        case State::Dead:
            deadSheet.setSpeed(0.17f);
            deadSheet.update(dt);
            if (deadSheet.isAnimationFinished())
            {
                dx = 0;
                isDeadFlag = true;
            }
            break;
    }
}

void Enemy::draw(SDL_Renderer* renderer)
{
    if(isDead()) return;

    bool flip = dx < 0;

    SDL_RendererFlip flipFlag = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    switch (state)
    {
    case State::Idle:
        idleSheet.draw(renderer, x, y, flipFlag);
        break;
    case State::Walk:
        walkSheet.draw(renderer, x, y, flipFlag);
        break;
    case State::Attack:
        attackSheet.draw(renderer, x, y, flipFlag);
        break;
    case State::Hurt:
        hurtSheet.draw(renderer, x, y, flipFlag);
        break;
    case State::Dead:
        deadSheet.draw(renderer, x, y, flipFlag);
        break;
    }

    int barW = 40;
    int barH = 5;
    int barX = x + 45 + (enemyW - barW) / 2;
    int barY = y + 10;

    SDL_Rect bgRect = { barX, barY, barW, barH };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    int hpW = (int)(( hp / 100.0f ) * barW);
    SDL_Rect hpRect = { barX, barY, hpW, barH};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &hpRect);
}

void Enemy::clean()
{
    idleSheet.clean();
    walkSheet.clean();
    attackSheet.clean();
    hurtSheet.clean();
    deadSheet.clean();
}

void Enemy::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Enemy::takeDamage(int dmg)
{
    Uint32 now = SDL_GetTicks();
    if(now - lastHitTime < damageCooldown) return;

    if(state == State::Dead || isDyingFlag) return;

    hp -= dmg;
    //std::cout << "Enemy HP: " << hp << "/100" << std::endl;
    state = State::Hurt;
    lastHitTime = now;

    if(hp <= 0)
    {
        hp = 0;
        defeated ++;
        std::cout << "Enemy died" << std::endl;
        state = State::Dead;
        isDyingFlag = true;
        deadSheet.setFrame(0);
    }
}

void Enemy::attack()
{
    if(state != State::Attack)
    {
        state = State::Attack;
    }
}

SDL_Rect Enemy::attackHitbox() const
{
    bool facingRight = true;
    if(dx > 0) facingRight = true;
    else if(dx < 0) facingRight = false;

    int attackWidth = 40;
    int attackHeight = 70;
    int offsetAttackX = facingRight ? 90 : 20;
    return SDL_Rect{static_cast<int>(x + offsetAttackX), static_cast<int>(y + offsetY), attackWidth, attackHeight};
}

SDL_Rect Enemy::getHitbox() const
{
    return SDL_Rect{static_cast<int>(x) + offsetX, static_cast<int>(y) + offsetY, enemyW, enemyH};
}

void Enemy::setAI(std::unique_ptr<EnemyAI> newAI)
{
    ai = std::move(newAI);
}


///debug
void Enemy::drawHitbox(SDL_Renderer* renderer)
{
    SDL_Rect enemyHitbox = getHitbox();
    SDL_Rect enemyAttackHitbox = attackHitbox();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 100, 100, 100);  // Màu xanh lá, alpha = 100 (mờ)
    SDL_RenderFillRect(renderer, &enemyHitbox);
    SDL_SetRenderDrawColor(renderer, 200, 100, 0, 100);
    SDL_RenderFillRect(renderer, &enemyAttackHitbox);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
