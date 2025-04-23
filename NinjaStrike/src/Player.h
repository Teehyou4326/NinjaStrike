#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>
#include <map>

#include "SpriteSheet.h"
#include "Texture.h"
#include "Shuriken.h"
#include "Config.h"
#include "Map.h"

enum class PlayerState{Idle, Running, Jumping, Attacking, Throwing, Hurt};

class Player
{
    friend class Potion;

public:
    Player();
    ~Player();

    bool init(const char* FileName, SDL_Renderer* renderer, int startX, int startY);
    void handleInput(const SDL_Event& event);
    void update(double dt);
    void draw(SDL_Renderer* renderer);
    void clean();

    SDL_Rect getHitbox() const ;
    SDL_Rect attackHitbox() const;
    void takeDamage(int dmg);
    int getHP() const { return hp; };
    PlayerState getState() const { return state; }

    void setMap(Map* map) { this->map = map; };

    std::vector<Shuriken>& getShurikens()
    {
        return shurikens;
    }

    float getX() const { return x; };
    float getY() const { return y; };

    void drawHitbox(SDL_Renderer* renderer);
private:
    int offsetX = 44;
    int offsetY = 26;

    PlayerState state;

    SDL_Renderer* renderer;

    SpriteSheet idleSheet;
    SpriteSheet runSheet;
    SpriteSheet jumpSheet;
    SpriteSheet attackSheet;
    SpriteSheet throwSheet;
    SpriteSheet hurtSheet;

    float x, y;
    float dx, dy;
    float prevX, prevY;
    float speed;
    float jump;
    bool isJumping;
    bool doubleJump;

    bool facingRight = true;

    int shurikenCooldown;
    std::vector<Shuriken> shurikens;

    Map* map = nullptr;

    Uint32 lastHitTime = 0;
    Uint32 damageCooldown = 800;
    int hp = 500;

    Uint32 SpeedBoostTime = 0;
    bool SpeedBoostFlag = false;

    Uint32 DmgBoostTime = 0;
    bool DmgBoostFlag = false;

    Uint32 InvincibleTime = 0;
    bool InvincibleFlag = false;
};

#endif // PLAYER_H
