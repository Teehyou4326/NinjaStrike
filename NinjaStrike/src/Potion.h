#ifndef POTION_H
#define POTION_H

#include "SpriteSheet.h"
#include <SDL.h>
#include <vector>
#include <cstdlib>

#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Config.h"

enum class PotionEffect
{
    Heal,
    SpeedBoost,
    DmgBoost,
    Invincible,

    Count // phan tu de dem
};

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

    void claim() { claimedFlag = true; }
    bool claimed() const { return claimedFlag; }

    void applyEffect(Player* player);

    void drawHitbox(SDL_Renderer* renderer);
private:
    int offsetX = 12;
    int offsetY = 4;

    Map* map = nullptr;

    int x, y;

    SpriteSheet sheet;

    bool claimedFlag = false;

    PotionEffect effect;
};

#endif // POTION_H
