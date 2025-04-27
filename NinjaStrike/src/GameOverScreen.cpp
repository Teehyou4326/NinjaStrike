#include "GameOverScreen.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>

GameOverScreen::GameOverScreen(SDL_Renderer* renderer)
    : renderer(renderer), font(nullptr), active(false), restart(false), backToMenu(false), finalScore(0)
{
    TTF_Init();
    font = TTF_OpenFont("res/font/s_font.ttf", 64);
}

GameOverScreen::~GameOverScreen()
{
    TTF_CloseFont(font);
    TTF_Quit();
}

void GameOverScreen::show(int score)
{
    active = true;
    finalScore = score;
}

void GameOverScreen::handleEvent(SDL_Event& event)
{
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        SDL_Point mousePos = {x,y};

        if(SDL_PointInRect(&mousePos, &restartTextRect))
        {
            restart = true;
            active = false;
            isResetDone = false;
        }
        else if(SDL_PointInRect(&mousePos, &menuTextRect))
        {
            backToMenu = true;
            active = false;
            isResetDone = false;
        }
    }
}

void GameOverScreen::draw()
{

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_Point mousePos = {mouseX, mouseY};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color color = {255, 255, 255, 255};
    SDL_Color hoverColor = {255, 240, 0, 255};
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Rect dstRect;

    surface = IMG_Load("res/map/BG/GameOverBG.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_DestroyTexture(texture);

    //Game over
    font = TTF_OpenFont("res/font/s_font.ttf", 120);
    SDL_Color titleColor = {200, 0, 0, 255};
    surface = TTF_RenderText_Solid(font, "GAME OVER", titleColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    dstRect = {640 - surface->w / 2, 100, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    //Score
    font = TTF_OpenFont("res/font/s_font.ttf", 88);

    std::string scoreText = "Score   " + std::to_string(finalScore);
    surface = TTF_RenderText_Solid(font, scoreText.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    dstRect = {640 - surface->w / 2, 300, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    //Restart
    bool hoverRestart = false;
    font = TTF_OpenFont("res/font/s_font.ttf", 48);

    surface = TTF_RenderText_Solid(font, "RESTART", color);
    restartTextRect = {640 - surface->w / 2, 540, surface->w, surface->h};
    if(SDL_PointInRect(&mousePos, &restartTextRect))
        hoverRestart = true;
    SDL_FreeSurface(surface);

    if (hoverRestart)
        font = TTF_OpenFont("res/font/s_font.ttf", 54);
    else
        font = TTF_OpenFont("res/font/s_font.ttf", 48);

    SDL_Color restartColor = hoverRestart ? hoverColor : color;
    surface = TTF_RenderText_Solid(font, "RESTART", restartColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    restartTextRect = {640 - surface->w / 2, 540, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &restartTextRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    //Menu
    bool hoverMenu = false;
    font = TTF_OpenFont("res/font/s_font.ttf", 48);
    surface = TTF_RenderText_Solid(font, "Back  to  menu", color);
    menuTextRect = {640 - surface->w / 2, 600, surface->w, surface->h};
    if (SDL_PointInRect(&mousePos, &menuTextRect))
        hoverMenu = true;
    SDL_FreeSurface(surface);

    if (hoverMenu)
        font = TTF_OpenFont("res/font/s_font.ttf", 54);
    else
        font = TTF_OpenFont("res/font/s_font.ttf", 48);

    SDL_Color menuColor = hoverMenu ? hoverColor : color;
    surface = TTF_RenderText_Solid(font, "Back  to  menu", menuColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    menuTextRect = {640 - surface->w / 2, 600, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &menuTextRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

}

void GameOverScreen::resetFlags()
{
    restart = false;
    backToMenu = false;
}
