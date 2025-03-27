#ifndef SHURIKEN_H
#define SHURIKEN_H

#include <SDL.h>

#include "Texture.h"
#include "Config.h"

class Shuriken
{
public:
    Shuriken(int startX, int startY, bool facingRight, SDL_Renderer* renderer);
    ~Shuriken();

    void update(float dt);
    void draw(SDL_Renderer* renderer);

    float getDistanceFlew() const { return distanceFlew; }
    float getMaxDistance() const { return maxDistance; }
    bool isOffScr() const {return x > SCREEN_WIDTH || x < 0; }

private:
    float x, y;
    float speed;
    int direction;
    Texture texture;
    bool facingRight;

    float distanceFlew = 0.0f;
    float maxDistance = 1000.0f;
};

#endif // SHURIKEN_H
