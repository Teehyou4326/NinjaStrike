#ifndef PLAYER_H
#define PLAYER_H

#include "Texture.h"
#include <SDL.h>

class Player
{
public:
    Player();
    ~Player();

    bool init(const char* FileName, SDL_Renderer* renderer, int startX, int startY);
    void handleInput(const SDL_Event& event);
    void update();
    void draw(SDL_Renderer* renderer);
    void clean();

private:
    Texture texture;
    double x, y;
    double dx, dy;
    double speed;
    bool isJumping;
    bool doubleJump;

};

#endif // PLAYER_H
