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

    bool load(const char* fileName, SDL_Renderer* renderer);
    void draw(SDL_Renderer* renderer, int x, int y, int width, int height);
    void clean();

private:
    SDL_Texture* texture;
};

#endif
