#include "Player.h"

const float gravity = 2200.0f;   //Trọng lực.
const int groundY = 500; // Giả lập mặt đất.


Player::Player()
{
    x = 0;
    y = 0;
    dx = 0;
    dy = 0;
    speed = 450.0f;
    jump = 850.0f;
    isJumping = false;
    doubleJump = false;
}

Player::~Player()
{
    clean();
}

bool Player::init(const char* fileName, SDL_Renderer* renderer, int startX, int startY )
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
                    dy = -jump;
                    isJumping = true;
                    doubleJump = true;
                }
                else if (doubleJump)
                {
                    dy = -jump * 0.8f;
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

void Player::update(double dt)
{
    x += dx * dt ;
    y += dy * dt ;

    if( y < groundY ) dy += gravity * dt * 1.5f ;
    else if(y >= groundY)
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


