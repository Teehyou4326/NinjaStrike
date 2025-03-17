#ifndef TEXTURE_H
#define TEXTURE_H


#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class Texture
{
public:
    Texture();
    ~Texture();

    bool load(SDL_Renderer* renderer, const char* fileName);
    void draw(SDL_Renderer* renderer, int x, int y, int width, int height);
    void draw(SDL_Renderer* renderer, SDL_Rect* srcRect, SDL_Rect* dstRect, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void clean();

private:
    SDL_Texture* texture;
};

#endif
