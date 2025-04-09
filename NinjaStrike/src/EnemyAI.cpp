#include "EnemyAI.h"
#include "Map.h"
#include <cmath>

EnemyAI::EnemyAI(float patrolMinX, float patrolMaxX, float detecionRange, Map* gameMap)
{
    minX = patrolMinX;
    maxX = patrolMaxX;
    detectionRange = detectionRange;
    chasingPlayer = false;
    map = gameMap;
}

void EnemyAI::update(float& x, float& velX, float playerX, float feetY, bool facingLeft)
{
    float distance = std::fabs(x-playerX);
    float checkX = x+(facingLeft ? -16.0f : 16.0f);
    float checkY = feetY + 1.0f;

    if(!map->isSolidAt(checkX, checkY))
    {
        velX = -velX;
        chasingPlayer = false;
        return;
    }

    if(distance < detectionRange) chasingPlayer = true;
    else if(distance > detectionRange + 100) chasingPlayer = false;

    if(chasingPlayer) velX = (playerX < x) ? -100.0f : 100.0f ;
    else
    {
        if(x <= minX) velX = 100.0f;
        else if(x >= maxX) velX = -100.0f;
    }
}
