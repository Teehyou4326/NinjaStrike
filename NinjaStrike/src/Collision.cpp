#include "Collision.h"

bool Collision::checkCollision(const SDL_Rect& a, const SDL_Rect& b)
{
    if(a.x + a.w <= b.x) return false;
    if(a.x >= b.x + b.w) return false;
    if(a.y + a.h <= b.y) return false;
    if(a.y >= b.y + b.h) return false;

    return true;
}
