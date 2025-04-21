#include "EnemyAI.h"

EnemyAI::EnemyAI(Enemy* enemy, Player* player,
                 int leftX, int rightX,
                 float moveSpeed, float detectRange)
    : enemy(enemy), player(player),
    patrolLeftX(leftX), patrolRightX(rightX),
    speed(moveSpeed), detectionRange(detectRange)
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
            enemy->dx = direction * speed;

            if(enemy->getX() <= patrolLeftX)
            {
                direction = 1;
            }
            else if(enemy->getX() >= patrolRightX)
            {
                direction = -1;
            }

            break;

        case EnemyState::Chase:
            if(playerX < enemyX) enemy->dx = -speed*1.3;
            else enemy->dx = speed*1.3;

            break;
    }
}
