#ifndef PLAYER_H
#define PLAYER_H

#include "Texture.h"
#include <SDL.h>

const int startX = 30;
const int startY = 500;

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
    Texture texture;
    float x, y;
    float dx, dy;
    float speed;
    float jump;
    bool isJumping;
    bool doubleJump;

};

#endif // PLAYER_H
