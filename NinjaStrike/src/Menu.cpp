#include "Menu.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

Menu::Menu(SDL_Renderer* renderer)
    :renderer(renderer), hoveredIndex(-1), active(true), startSelected(false)
{
    TTF_Init();
    font = TTF_OpenFont("res/font/font.ttf", 40);
    items = {"START GAME", "QUIT"};

    SDL_Surface* bgSurface = IMG_Load("res/menu_pic.png");
    if(bgSurface)
    {
        background = SDL_CreateTextureFromSurface(renderer, bgSurface);
        SDL_FreeSurface(bgSurface);
    }

    updateItemRects();
}

Menu::~Menu()
{
    TTF_CloseFont(font);
    TTF_Quit();
}

void Menu::handleEvent(SDL_Event& event)
{
    if(!active) return;

    if(event.type == SDL_MOUSEMOTION)
    {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;
        SDL_Point mousePoint = {mouseX, mouseY};
        hoveredIndex = -1;
        for(size_t i=0; i < itemRects.size(); i++)
        {
            if(SDL_PointInRect(&mousePoint, &itemRects[i]))
            {
                hoveredIndex = static_cast<int>(i);
                break;
            }
        }
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if(hoveredIndex == 0)
        {
            startSelected = true;
            active = false;
        }
        else if(hoveredIndex == 1)
        {
            SDL_Event quitEvent;
            quitEvent.type = SDL_QUIT;
            SDL_PushEvent(&quitEvent);
        }
    }
}

SDL_Texture* Menu::renderText(const std::string& text, SDL_Color color)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Menu::draw()
{
    if(!active) return;

    if(background)
    {
        SDL_RenderCopy(renderer, background, nullptr, nullptr);
    }

    int y = 385;
    itemRects.clear();

    for(size_t i=0; i< items.size(); i++)
    {
        SDL_Color color = (static_cast<int>(i) == hoveredIndex) ? SDL_Color{255, 255, 0, 255} : SDL_Color{255, 255, 255, 255};
        SDL_Texture* textTex = renderText(items[i], color);

        int texW = 0, texH = 0;
        SDL_QueryTexture(textTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect dstRect = {745 - texW / 2, y, texW, texH};
        SDL_RenderCopy(renderer, textTex, nullptr, &dstRect);

        itemRects.push_back(dstRect);
        SDL_DestroyTexture(textTex);
        y += 50;
    }
}

void Menu::updateItemRects()
{
    int y = 385;
    itemRects.clear();

    for(const std::string& item :items)
    {
        SDL_Texture* textTex = renderText(item, SDL_Color{255, 255, 255, 255});
        int texW = 0, texH = 0;
        SDL_QueryTexture(textTex, nullptr, nullptr, &texW, &texH);
        SDL_Rect dstRect = {745 - texW / 2, y, texW, texH};
        SDL_RenderCopy(renderer, textTex, nullptr, &dstRect);

        itemRects.push_back(dstRect);
        SDL_DestroyTexture(textTex);
        y += 50;
    }
}

bool Menu::isActive() const
{
    return active;
}

void Menu::deactivate()
{
    active = false;
}

bool Menu::startGameSelect() const
{
    return startSelected;
}
