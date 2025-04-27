#include "GameOverScreen.h"
#include <SDL.h>
#include <SDL_ttf.h>

GameOverScreen::GameOverScreen(SDL_Renderer* renderer)
    : renderer(renderer), font(nullptr), active(false), restart(false), backToMenu(false), finalScore(0)
{
    TTF_Init();
    font = TTF_OpenFont("res/font/s_font.ttf", 36);

    restartButton = {500, 300, 200, 50};
    menuButton = {500, 600, 200, 50};
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

        if(SDL_PointInRect(&mousePos, &restartButton))
        {
            restart = true;
            active = false;
        }
        else if(SDL_PointInRect(&mousePos, &menuButton))
        {
            backToMenu = true;
            active = false;
        }

    }
}

void GameOverScreen::draw()
{
    //Game over
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Rect dstRect;

    surface = TTF_RenderText_Solid(font, "GAME OVER !!!", white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    dstRect = {200, 100, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    //Restart
    std::string scoreText = "Score " + std::to_string(finalScore);
    surface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    dstRect = {200, 200, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_RenderFillRect(renderer, &restartButton);
    surface = TTF_RenderText_Solid(font, "RESTART", white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    dstRect = {restartButton.x + 50, restartButton.y + 10, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    //Return to menu
    SDL_SetRenderDrawColor(renderer, 100, 255, 100, 255);
    SDL_RenderFillRect(renderer, &menuButton);
    surface = TTF_RenderText_Solid(font, "Back to menu", white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    dstRect = {menuButton.x + 30, menuButton.y + 10, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void GameOverScreen::resetFlags()
{
    restart = false;
    backToMenu = false;
}
