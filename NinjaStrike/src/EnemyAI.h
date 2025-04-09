#ifndef ENEMY_AI_H
#define ENEMY_AI_H

class Map;

class EnemyAI
{
public:
    EnemyAI(float patrolMinX, float patrolMaxX, float detectionRange, Map* gameMap);
    void update(float& x, float& velX, float playerX, float feetY, bool facingLeft);

private:
    float minX, maxX;
    float detectionRange;
    bool chasingPlayer;

    Map* map;

};

#endif // ENEMY_AI_H
