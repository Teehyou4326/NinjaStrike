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
    float enemyX = enemy->getX();
    float playerX = player->getX();
    float distance = std::abs(enemyX - playerX);

    if(distance <= detectionRange)
    {
        state = EnemyState::Chase;
    }
    else
    {
        state = EnemyState::Patrol;
    }

    switch(state)
    {
        case EnemyState::Patrol:
        {
            int nextX = enemyX + direction * speed;
            int footY = enemy->getY() + enemyH;

            bool wallAhead = map->isWallAt(static_cast<int>(nextX), static_cast<int>(enemy->getY()) );
            bool noGroundAhead = !map->isGroundBelow(static_cast<int>(nextX), static_cast<int>(footY) + 1);

            if(wallAhead || noGroundAhead)
            {
                direction *= -1;
            }

            enemy->dx = direction * speed;
            break;
        }
        case EnemyState::Chase:
        {
            int nextX = enemyX + ((playerX < enemyX) ? -1 : 1) * speed;
            int footY = enemy->getY() + enemyH;

            bool wallAhead = map->isWallAt(static_cast<int>(nextX), static_cast<int>(enemy->getY()) );
            bool noGroundAhead = !map->isGroundBelow(static_cast<int>(nextX), static_cast<int>(footY) + 1);

            if(wallAhead || noGroundAhead)
            {
                enemy->dx = 0;
            }

            if(playerX < enemyX)
            {
                enemy->dx = -speed * 1.4;
            }
            else
            {
                enemy->dx = speed * 1.4;
            }

            break;
        }
    }
}
