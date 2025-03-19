#include "Shuriken.h"


Shuriken::Shuriken(int startX, int startY, bool facingRight, SDL_Renderer* renderer)
{
    x = startX;
    y = startY;
    speed = 1000;
    direction = facingRight ? 1 : -1;

    texture.load(renderer, "res/shuriken.png");
}

Shuriken::~Shuriken()
{
    texture.clean();
}

void Shuriken::update(float dt)
{
    x += float(direction) * speed * dt;
}

void Shuriken::draw(SDL_Renderer* renderer)
{
    texture.draw(renderer, x, y, 32, 32);
}
