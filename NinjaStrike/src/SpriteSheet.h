#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <SDL.h>
#include <vector>
#include <string>

#include "Texture.h"

struct Frame
{
    int x, y, w, h;
};

class SpriteSheet
{
public:
    SpriteSheet();
    ~SpriteSheet();

    bool load(SDL_Renderer* renderer, const char* imagePath, const char* jsonPath);
    void update(float dt);
    void draw(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void clean();
    void setFrame(int index);
    void setSpeed(float speed) { frameSpeed = speed; }
    bool isAnimationFinished() const
    {
        return static_cast<size_t>(frameIndex) >= frames.size() - 1;
    }

    int getFrameCount() const;

private:
    Texture texture;
    std::vector<Frame> frames;

    int frameIndex = 0;
    float frameTimer = 0.0f;
    float frameSpeed;


};

#endif // SPRITESHEET_H
