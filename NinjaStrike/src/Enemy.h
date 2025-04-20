#ifndef ENEMY_H
#define ENEMY_H

#include "SpriteSheet.h"
#include <SDL.h>
#include <vector>

#include "Map.h"

class Enemy
{
public:
    Enemy();
    ~Enemy();

    bool load(SDL_Renderer* renderer);
    void update(float dt);
    void draw(SDL_Renderer* renderer);
    void clean();
    void setPosition(int x, int y);
    void setMap(Map* map) { this->map = map; };

    void takeDamage();
    SDL_Rect getHitbox() const;

    void drawHitbox(SDL_Renderer* renderer);
private:
    int offsetX = 48;
    int offsetY = 33;

    enum class State { Idle, Walk, Attack, Hurt, Dead};

    SpriteSheet idleSheet;
    SpriteSheet walkSheet;
    SpriteSheet attackSheet;
    SpriteSheet hurtSheet;
    SpriteSheet deadSheet;

    int x, y;
    State state;
    float dx, dy;
    int prevX, prevY;

    Map* map = nullptr;

    int count = 0;
};

#endif // ENEMY_H
