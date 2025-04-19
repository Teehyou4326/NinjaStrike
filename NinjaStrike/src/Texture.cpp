#include "Texture.h"

Texture::Texture()
{
    texture = nullptr;
}

Texture::~Texture()
{
    //std::cout << "[~Texture] bi huy: " << this << ", SDL_Texture*: " << texture << std::endl;
}

bool Texture::load(SDL_Renderer* renderer, const char* fileName)
{
    std::cout << "[Texture::load] dang load: " << fileName << ", SDL_Texture*: " << texture <<std::endl;

    SDL_Surface* surface = IMG_Load(fileName);
    if(!surface)
    {
        std::cout << "IMG_Load Error: " << IMG_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface( surface);
    if(!texture)
    {
        std::cout << "SDL_CreateTextureFromSurface Error:" << SDL_GetError() << std ::endl;
        return false;
    }
    std::cout << "[Texture::load] thanh cong." << std::endl;
    return true;
}

void Texture::draw(SDL_Renderer* renderer, int x, int y, int width, int height)
{
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}

void Texture::draw(SDL_Renderer* renderer, SDL_Rect* srcRect, SDL_Rect* dstRect, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(renderer, texture, srcRect, dstRect, 0, nullptr, flip);
}
void Texture::clean()
{
    if(texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Texture::draw(SDL_Renderer* renderer, int x, int y, SDL_Rect* srcRect)
{
    if (texture == nullptr)
    {
        std::cout << "[Texture::draw] texture null!" << std::endl;
        return;
    }

    SDL_Rect dstRect = {x, y, 0, 0};
    if(srcRect)
    {
        dstRect.w = srcRect->w;
        dstRect.h = srcRect->h;
    }
    else
    {
        if(SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h) != 0)
        {
            std::cout << "[Texture] SDL_Query that bai...  " << SDL_GetError() << std::endl;
        }
    }
    if(SDL_RenderCopy(renderer, texture, srcRect, &dstRect) != 0)
    {
        std::cout << "[Texture] SDL_RenderCopy that bai: " << SDL_GetError() << std::endl;
    }
}
