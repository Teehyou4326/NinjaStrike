#include "Shuriken.h"


Shuriken::Shuriken(int startX, int startY, bool facingRight, SDL_Renderer* renderer)
{
    x = startX;
    y = startY;
    speed = 1000;
    direction = facingRight ? 1 : -1;
    this->facingRight = facingRight;

    texture.load(renderer, "res/shuriken.png");
}

Shuriken::~Shuriken()
{
    texture.clean();
}

void Shuriken::update(float dt)
{
    float moveDistance = speed * dt;
    x += direction * moveDistance;
    distanceFlew += moveDistance;
}

void Shuriken::draw(SDL_Renderer* renderer)
{
    texture.draw(renderer, x, y, 28, 28);
}

SDL_Rect Shuriken::getHitbox() const
{
    return  SDL_Rect{static_cast<int>(x), static_cast<int>(y), 28, 28};
}
