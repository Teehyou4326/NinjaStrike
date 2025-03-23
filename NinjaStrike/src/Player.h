#ifndef PLAYER_H
#define PLAYER_H

#include "SpriteSheet.h"
#include "Texture.h"
#include <SDL.h>
#include <vector>
#include "Shuriken.h"
#include "Config.h"

//const int startX = 30;
//const int startY = 500;

class Player
{
public:
    Player();
    ~Player();

    bool init(const char* FileName, SDL_Renderer* renderer, int startX, int startY);
    void handleInput(const SDL_Event& event);
    void update(double dt);
    void draw(SDL_Renderer* renderer);
    void clean();

private:
    enum class PlayerState{Idle, Running, Jumping, Attacking, Throwing, Hurt};
    PlayerState state;
    SDL_Renderer* renderer;

    SpriteSheet idleSheet;
    SpriteSheet runSheet;
    SpriteSheet jumpSheet;
    SpriteSheet attackSheet;
    SpriteSheet throwSheet;

    Texture hurtTexture;

    float x, y;
    float dx, dy;
    float speed;
    float jump;
    bool isJumping;
    bool doubleJump;
    const int playerW = 100;
    const int playerH = 64;

    bool facingRight = true;

    int shurikenCooldown;
    const int maxCooldown = 10;

    std::vector<Shuriken> shurikens;

};

#endif // PLAYER_H
