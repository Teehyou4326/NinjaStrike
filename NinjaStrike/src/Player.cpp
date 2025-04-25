#include "Player.h"
#include "SoundManager.h"

Player::Player()
{
    x = 0;
    y = 0;
    dx = 0;
    dy = 0;
    speed = 430.0f;
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
        !hurtSheet.load(renderer, "res/Ninja/hurt.png", "res/Ninja/hurt.json"))
    {
        return false;
    }
    x = startX;
    y = startY;
    return true;
}

void Player::handleInput(const SDL_Event& event)
{
    if(StunFlag) return;

    if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym) // xác định phím được nhấn hoặc thả
        {
            case SDLK_LEFT:
                if(dy == 0)
                    SoundManager::getInstance().playSound("run");
                dx = ReverseControlFlag ? speed : -speed;
                facingRight = ReverseControlFlag;
                break;
            case SDLK_RIGHT:
                if(dy == 0)
                    SoundManager::getInstance().playSound("run");
                dx = ReverseControlFlag ? -speed : speed;
                facingRight = !ReverseControlFlag;
                break;
            case SDLK_UP:
                if(!isJumping)
                {
                    SoundManager::getInstance().playSound("jump");
                    dy = -jump;
                    isJumping = true;
                    doubleJump = true;
                }
                else if (doubleJump)
                {
                    SoundManager::getInstance().playSound("jump");
                    dy = -jump * 0.8f;
                    doubleJump = false;
                }
                break;
            case SDLK_z:
                SoundManager::getInstance().playSound("attack");
                state = PlayerState::Attacking;
                break;
            case SDLK_x:
                if(shurikenCooldown == 0)
                {
                SoundManager::getInstance().playSound("throw");
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
                    dx = ReverseControlFlag ? speed : -speed;
                    facingRight = ReverseControlFlag;
                }
                else dx = 0;
                break;
            case SDLK_RIGHT:
                if(state[SDL_SCANCODE_LEFT])
                {
                    dx = ReverseControlFlag ? -speed : speed;
                    facingRight = !ReverseControlFlag;
                }
                else dx = 0;
                break;
        }
    }
}

void Player::update(double dt)
{
    prevX = x;
    prevY = y;

    x += dx * dt - 2;
    SDL_Rect futureXHitboxRight = {static_cast<int>(x) + offsetX, static_cast<int>(y + offsetY), playerW, playerH };
    if(map && map->checkCollision(futureXHitboxRight))
    {
        x = prevX - 2;
    }
    SDL_Rect futureXHitboxLeft = {static_cast<int>(x) + offsetX, static_cast<int>(y + offsetY), playerW, playerH };
    if(map && map->checkCollision(futureXHitboxLeft))
    {
        x = prevX;
    }

    y += dy * dt ;
    SDL_Rect futureYHitbox = {static_cast<int>(x) + offsetX, static_cast<int>(y + offsetY), playerW, playerH };
    if(map && map->checkCollision(futureYHitbox))
    {
        y = prevY;
        dy = 0;
        isJumping = false;
    }

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

    if(StunFlag)
    {
        dx = 0;
        dy = 0;
        isJumping = false;
    }

    //animation
    switch (state)
    {
        case PlayerState::Idle:
            idleSheet.setSpeed(0.1f);
            idleSheet.update(dt);
            break;
        case PlayerState::Running:
            {
            Uint32 now = SDL_GetTicks();
            if(now - lastRun >= 200)
            {
                SoundManager::getInstance().playSound("run");
                lastRun = now;
            }
            runSheet.setSpeed(0.045f);
            runSheet.update(dt);
            break;
            }
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
            throwSheet.setSpeed(0.025f);
            throwSheet.update(dt);
            if(throwSheet.isAnimationFinished()) state = PlayerState::Idle;
            break;
        case PlayerState::Hurt:
            hurtSheet.setSpeed(1);
            hurtSheet.update(dt);
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

    //Potion effect
    if(SpeedBoostFlag && SDL_GetTicks() >= SpeedBoostTime)
    {
        speed /= 1.5;
        SpeedBoostFlag = false;
        std::cout << "END speed: " << speed << std::endl;
    }
    if(DmgBoostFlag && SDL_GetTicks() >= DmgBoostTime)
    {
        playerDMG /= 1.5;
        DmgBoostFlag = false;
        std::cout << "END damage: " << playerDMG << std::endl;
    }
    if(InvincibleFlag && SDL_GetTicks() >= InvincibleTime)
    {
        InvincibleFlag = false;
        std::cout << "END Invincible" << std::endl;
    }
    if(ReverseControlFlag && SDL_GetTicks() >= ReverseControlTime)
    {
        ReverseControlFlag = false;
        std::cout << "END REVERSE " << std::endl;
    }
    if(StunFlag && SDL_GetTicks() >= StunTime)
    {
        StunFlag = false;
        std::cout << "END STUN " << std::endl;
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
            hurtSheet.draw(renderer, x, y, flip);
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
    hurtSheet.clean();
}

void Player::takeDamage(int dmg)
{
    Uint32 now = SDL_GetTicks();
    if(now - lastHitTime < damageCooldown) return;
    lastHitTime = now;

    if(!InvincibleFlag) hp -= dmg;
    else hp -= 0;
    std::cout << "PLAYER HP: " << hp << " / 600" << std::endl;
    state = PlayerState::Hurt;

    if(hp <= 0)
    {
        std::cout << "Player died. You losed" << std::endl;
    }
}

SDL_Rect Player::getHitbox() const
{
    return SDL_Rect{ static_cast<int>(x) + offsetX, static_cast<int>(y) + offsetY, playerW, playerH };
}

SDL_Rect Player::attackHitbox() const
{
    int attackWidth = 30;
    int attackHeight = 30;
    int offsetAttackX = facingRight ? 78 : 14;
    return SDL_Rect{static_cast<int>(x + offsetAttackX), static_cast<int>(y + offsetY), attackWidth, attackHeight};
}

///debug
void Player::drawHitbox(SDL_Renderer* renderer)
{
    SDL_Rect playerHitbox = getHitbox();
    SDL_Rect playerAttackHitbox = attackHitbox();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);  // Màu xanh lá, alpha = 100 (mờ)
    SDL_RenderFillRect(renderer, &playerHitbox);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 100);
    SDL_RenderFillRect(renderer, &playerAttackHitbox);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

