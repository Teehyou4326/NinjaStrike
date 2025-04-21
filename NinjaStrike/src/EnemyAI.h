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
    Chase
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

};

#endif // ENEMYAI_H
