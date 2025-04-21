#ifndef POTION_H
#define POTION_H

#include "SpriteSheet.h"
#include <SDL.h>
#include <vector>

#include "Map.h"

class Potion
{
public:
    Potion();
    ~Potion();

    bool load(SDL_Renderer* renderer);
    void update(float dt);
    void draw(SDL_Renderer* renderer);
    void clean();
    void setPosition(int x, int y);
    void setMap(Map* map) { this->map = map; };

    SDL_Rect getHitbox() const;

    void drawHitbox(SDL_Renderer* renderer);
private:
    int offsetX = 12;
    int offsetY = 4;

    int x, y;

    SpriteSheet sheet;

    Map* map = nullptr;
};

#endif // POTION_H
