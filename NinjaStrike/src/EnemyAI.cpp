#include "EnemyAI.h"

EnemyAI::EnemyAI(Enemy* enemy, Player* player,
                 int leftX, int rightX,
                 float moveSpeed, float detectRange,
                 Map* map)
    : enemy(enemy), player(player),
    patrolLeftX(leftX), patrolRightX(rightX),
    speed(moveSpeed), detectionRange(detectRange),
    map(map)
{}

void EnemyAI::update()
{
    if(enemy->isDead() || enemy->isDying()) return;

    float enemyX = enemy->getX();
    float enemyY = enemy->getY();
    float playerX = player->getX();
    float playerY = player->getY();

    float distanceX = std::abs(enemyX - playerX);
    float distanceY = std::abs(enemyY - playerY);

    Uint32 now = SDL_GetTicks();

    if(state == EnemyState::Idle)
    {
        if(now - idleStartTime < idleDuration)
        {
            enemy->dx = 0;
            return;
        }
        else
        {
            if(distanceX <= 50 && distanceY <= 40)
            {
                state = EnemyState::Attack;
            }
            else if(distanceX <= detectionRange && distanceY <= detectionRange)
            {
                state = EnemyState::Chase;
            }
            else
            {
                state = EnemyState::Patrol;
            }
        }
    }
    else
    {
            if(distanceX <= 40 && distanceY <= 40)
        {
            state = EnemyState::Attack;
        }
        else if(distanceX <= detectionRange && distanceY <= detectionRange)
        {
            state = EnemyState::Chase;
        }
        else
        {
            state = EnemyState::Patrol;
        }
    }

    //Define state.
    switch(state)
    {
        case EnemyState::Patrol:
        {
            int nextX = enemyX + direction * speed;
            int footY = enemy->getY() + enemyH;

            bool wallAhead = map->isWallAt(static_cast<int>(nextX), static_cast<int>(enemy->getY()) );
            bool noGroundAhead = !map->isGroundBelow(static_cast<int>(nextX) + 15, static_cast<int>(footY) + 1);

            enemy->dx = direction * speed;

            if(wallAhead || noGroundAhead)
            {
                direction *= -1;
            }

            break;
        }
        case EnemyState::Chase:
        {
            int nextX = enemyX + ((playerX < enemyX) ? -1 : 1) * speed;
            int footY = enemy->getY() + enemyH;

            bool wallAhead = map->isWallAt(static_cast<int>(nextX), static_cast<int>(enemy->getY()) );
            bool noGroundAhead = !map->isGroundBelow(static_cast<int>(nextX) + 15, static_cast<int>(footY) + 1);

            if(playerX < enemyX)
            {
                enemy->dx = -speed * 3.5f;
            }
            else
            {
                enemy->dx = speed * 3.5f;
            }

            if(wallAhead || noGroundAhead)
            {
                enemy->dx = 0;
            }

            break;
        }
        case EnemyState::Attack:
        {
            int nextX = enemyX + ((playerX < enemyX) ? -1 : 1) * speed;
            int footY = enemy->getY() + enemyH;

            bool wallAhead = map->isWallAt(static_cast<int>(nextX), static_cast<int>(enemy->getY()) );
            bool noGroundAhead = !map->isGroundBelow(static_cast<int>(nextX) + 15, static_cast<int>(footY) + 1);

            if(SDL_GetTicks() - lastAttackTime > attackCooldown)
            {
                enemy->attack();
                lastAttackTime = SDL_GetTicks();

                state = EnemyState::Idle;
                idleStartTime = SDL_GetTicks();
            }

            if(wallAhead || noGroundAhead)
            {
                enemy->dx = 0;
            }

            break;
        }
        case EnemyState::Idle:
        {
            enemy->dx = 0;
            break;
        }
    }
}
