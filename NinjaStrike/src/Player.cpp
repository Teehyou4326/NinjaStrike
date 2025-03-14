#include "Player.h"

const double gravity = 0.009;   //Trọng lực.
const int groundY = 500; // Giả lập mặt đất.

Player::Player()
{
    x = 0;
    y = 0;
    dx = 0;
    dy = 0;
    speed = 1.5;
    isJumping = false;
    doubleJump = false;
}

Player::~Player()
{
    clean();
}

bool Player::init(const char* fileName, SDL_Renderer* renderer, int startX, int startY)
{
    if(!texture.load(fileName, renderer)) return false;
    x = startX;
    y = startY;
    return true;
}

void Player::handleInput(const SDL_Event& event)
{
    if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym) // xác định phím được nhấn hoặc thả
        {
            case SDLK_LEFT:
                dx= -speed ;
                break;
            case SDLK_RIGHT:
                dx= speed ;
                break;
            case SDLK_UP:
                if(!isJumping)
                {
                    dy = -4;
                    isJumping = true;
                    doubleJump = true;
                }
                else if (doubleJump)
                {
                    dy = -4;
                    doubleJump = false;
                }
                break;
        }
    }
    else if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT:
            case SDLK_RIGHT:
                dx = 0;
                break;
        }
    }
}

void Player::update()
{
    x += dx * 0.15;
    y += dy * 0.15;

    if( y < groundY ) dy += gravity;
    else
    {
        y = groundY;
        dy = 0;
        isJumping = false;
    }
}

void Player::draw(SDL_Renderer* renderer)
{
    texture.draw(renderer, x, y, 100, 64);
}

void Player::clean()
{
    texture.clean();
}


