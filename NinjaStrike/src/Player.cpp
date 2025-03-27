#include "Player.h"
#include "Game.h"
#include "Config.h"

const float gravity = 2100.0f;   //Trọng lực.
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

    state = PlayerState::Idle;
    shurikenCooldown = 0;
}

Player::~Player()
{
    clean();
}

bool Player::init(const char* fileName, SDL_Renderer* renderer, int startX, int startY )
{
    this->renderer = renderer;
    if( !idleSheet.load(renderer, "res/Ninja/idle.png", "res/Ninja/idle.json")       ||
        !runSheet.load(renderer, "res/Ninja/run.png", "res/Ninja/run.json")          ||
        !jumpSheet.load(renderer, "res/Ninja/jump.png", "res/Ninja/jump.json")       ||
        !attackSheet.load(renderer, "res/Ninja/attack.png", "res/Ninja/attack.json") ||
        !throwSheet.load(renderer, "res/Ninja/throw.png", "res/Ninja/throw.json")    ||
        !hurtTexture.load(renderer, "res/Ninja/hurt.png"))
    {
        return false;
    }
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
                facingRight = false;
                break;
            case SDLK_RIGHT:
                dx= speed ;
                facingRight = true;
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
            case SDLK_z:
                state = PlayerState::Attacking;
                break;
            case SDLK_x:
                if(shurikenCooldown == 0)
                {
                state = PlayerState::Throwing;
                shurikens.emplace_back(x+(facingRight ? 50 : -10), y+20, facingRight, this->renderer);
                shurikenCooldown = 50;
                }
                break;
        }
    }
    else if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT:
                if(state[SDL_SCANCODE_RIGHT])
                {
                    dx= speed ;
                    facingRight = true;
                }
                else dx = 0;
                break;
            case SDLK_RIGHT:
                if(state[SDL_SCANCODE_LEFT])
                {
                    dx= -speed ;
                    facingRight = false;
                }
                else dx = 0;
                break;
        }
    }
}

void Player::update(double dt)
{
    x += dx * dt ;
    y += dy * dt ;

    //update state
    if(state != PlayerState::Attacking && state != PlayerState::Throwing)
    {
        if(isJumping)    state = PlayerState::Jumping;
        else if(dx != 0) state = PlayerState::Running;
        else             state = PlayerState::Idle;
    }

    //gravity
    if( y < groundY ) dy += gravity * dt * 1.5f ;
    else if(y >= groundY)
    {
        y = groundY;
        dy = 0;
        isJumping = false;
        jumpSheet.setFrame(2);
    }

    //animation
    switch (state)
    {
        case PlayerState::Idle:
            idleSheet.setSpeed(0.05f);
            idleSheet.update(dt);
            break;
        case PlayerState::Running:
            runSheet.setSpeed(0.04f);
            runSheet.update(dt);
            break;
        case PlayerState::Jumping:
            if(dy < 0 && !doubleJump)
            {
                jumpSheet.setFrame(0);
            }
            else if(dy < 0 && doubleJump )
            {
                jumpSheet.setFrame(3);
            }
            else if(dy > 0)
            {
                jumpSheet.setFrame(1);
            }
            break;
        case PlayerState::Attacking:
            attackSheet.setSpeed(0.015f);
            attackSheet.update(dt);
            if(attackSheet.isAnimationFinished()) state = PlayerState::Idle;
            break;
        case PlayerState::Throwing:
            throwSheet.setSpeed(0.015f);
            throwSheet.update(dt);
            if(throwSheet.isAnimationFinished()) state = PlayerState::Idle;
            break;
        default:
            break;
    }

    //shuriken
    for(auto it = shurikens.begin(); it != shurikens.end();)
    {
        it->update(dt);
        if(it->getDistanceFlew() >= it->getMaxDistance()) it = shurikens.erase(it);
        else it++;
    }

    //Cooldown
    if(shurikenCooldown > 0)
    {
        shurikenCooldown--;
    }
}

void Player::draw(SDL_Renderer* renderer)
{
    //Lật ngược Texture
    SDL_RendererFlip flip = facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    switch (state)
    {
        case PlayerState::Idle:
            idleSheet.draw(renderer,x, y, flip);
            break;
        case PlayerState::Running:
            runSheet.draw(renderer, x, y, flip);
            break;
        case PlayerState::Jumping:
            jumpSheet.draw(renderer, x, y, flip);
            break;
        case PlayerState::Attacking:
            attackSheet.draw(renderer,x, y, flip);
            break;
        case PlayerState::Throwing:
            throwSheet.draw(renderer, x, y, flip);
            break;
        case PlayerState::Hurt:
            hurtTexture.draw(renderer, x, y, playerW, playerH);
            break;
    }

    for(auto& shuriken : shurikens)
    {
        shuriken.draw(renderer);
    }
}
void Player::clean()
{
    idleSheet.clean();
    runSheet.clean();
    jumpSheet.clean();
    attackSheet.clean();
    throwSheet.clean();
    hurtTexture.clean();
}


