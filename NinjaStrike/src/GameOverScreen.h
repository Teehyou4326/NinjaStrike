#ifndef GAMEOVERSCREEN_H
#define GAMEOVERSCREEN_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class GameOverScreen
{
public:
    GameOverScreen(SDL_Renderer* renderer);
    ~GameOverScreen();

    void handleEvent(SDL_Event &event);
    void draw();
    bool isActive() const { return active; }
    void show(int score);
    bool toGame() const { return restart; }
    bool toMenu() const { return backToMenu; }
    void resetFlags();

private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool active;
    bool restart;
    bool backToMenu;
    int finalScore;

    SDL_Rect restartButton;
    SDL_Rect menuButton;

};

#endif // GAMEOVERSCREEN_H
