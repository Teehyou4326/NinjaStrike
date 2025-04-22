#ifndef ENEMYAI_H
#define ENEMYAI_H

#include <cmath>

#include "Enemy.h"
#include "Player.h"
#include "Map.h"
#include "Config.h"

enum class EnemyState
{
    Patrol,
    Chase,
    Attack,
    Idle
};

class Enemy;
class Player;

class EnemyAI
{
public:
    EnemyAI(Enemy* enemy, Player* player,
            int leftX, int rightX,
            float moveSpeed, float detectRange,
            Map* map);

    void update();

private:
    Enemy* enemy;
    Player* player;
    int patrolLeftX, patrolRightX;
    float speed;
    float detectionRange;
    Map* map;

    EnemyState state = EnemyState::Patrol;
    int direction = 1;

    Uint32 lastAttackTime = 0;
    Uint32 attackCooldown = 500;
    Uint32 idleStartTime = 0;
    Uint32 idleDuration = 500;
};

#endif // ENEMYAI_H
