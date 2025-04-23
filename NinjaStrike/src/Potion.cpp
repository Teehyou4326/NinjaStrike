#include "Potion.h"

Potion::Potion()
{
    x = 0;
    y = 0;
}
Potion::~Potion() {}

bool Potion::load(SDL_Renderer* renderer)
{
    int randomIndex = rand() % static_cast<int>(PotionEffect::Count);
    effect = static_cast<PotionEffect>(randomIndex);

    if(!sheet.load(renderer, "res/potion/potion.png", "res/potion/potion.json"))
    {
        std::cout << "[Potion] load potion that bai " << std::endl;
        return false;
    }
    return true;
}

void Potion::update(float dt)
{
    if(claimedFlag) return;

    x -= 2;

    sheet.setSpeed(0.25f);
    sheet.update(dt);
}

void Potion::draw(SDL_Renderer* renderer)
{
    if(claimedFlag) return;

    sheet.draw(renderer, x, y);
}

void Potion::applyEffect(Player* player)
{
    switch(effect)
    {
        case PotionEffect::Heal:
        {
            player->hp += 150;
            if(player->hp > 500) player->hp = 500;

            std::cout << "HEAL -- " << "PLAYER HP: " << player->hp << " /500" << std::endl;
            break;
        }
        case PotionEffect::SpeedBoost:
        {
            player->speed *= 1.5;
            player->SpeedBoostTime = SDL_GetTicks() + 7000;
            player->SpeedBoostFlag = true;
            std::cout << "SPEED: " << player->speed << std::endl;

            break;
        }
        case PotionEffect::DmgBoost:
        {
            playerDMG *= 1.5;
            player->DmgBoostTime = SDL_GetTicks() + 7000;
            player->DmgBoostFlag = true;
            std::cout << "DAMAGE: " << playerDMG << std::endl;
            break;
        }
        case PotionEffect::Invincible:
        {
            player->InvincibleTime = SDL_GetTicks() + 5000;
            player->InvincibleFlag = true;
            std::cout << "INVINCIBLE" << std::endl;
            break;
        }
        case PotionEffect::Count:
            break;
    }
}

void Potion::clean()
{
    sheet.clean();
}

void Potion::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

SDL_Rect Potion::getHitbox() const
{
    return SDL_Rect{static_cast<int>(x) + offsetX, static_cast<int>(y) + offsetY, 31, 43};
}

void Potion::drawHitbox(SDL_Renderer* renderer)
{
    SDL_Rect potionHitbox = getHitbox();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 100, 0, 100, 100);  // Màu xanh lá, alpha = 100 (mờ)
    SDL_RenderFillRect(renderer, &potionHitbox);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
