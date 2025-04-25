#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>

#include "SoundManager.h"

class Menu
{
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    void handleEvent(SDL_Event& event);
    void draw();
    bool isActive() const;
    void deactivate();
    bool startGameSelect() const;

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> items;
    std::vector<SDL_Rect> itemRects;
    int hoveredIndex;
    bool active;
    bool startSelected;

    Mix_Music* menuMusic = nullptr;

    SDL_Texture* background;

    SDL_Texture* renderText(const std::string& text, SDL_Color color);
    void updateItemRects();
};

#endif // MENU_H
