#include "Texture.h"

Texture::Texture()
{
    texture = nullptr;
}

Texture::~Texture()
{
    clean();
}

bool Texture::load(const char* fileName, SDL_Renderer* renderer)
{
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
    return true;
}

void Texture::draw(SDL_Renderer* renderer, int x, int y, int width, int height)
{
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}

void Texture::clean()
{
    if(texture)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
