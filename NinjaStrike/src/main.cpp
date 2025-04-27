#include "Game.h"
#include "Menu.h"
#include "GameOverScreen.h"

Game game;

int main(int argc, char* argv[])
{
    if(!game.init("Ninja Strike")) return 1;

    SDL_Event event;
    Menu menu(game.getRenderer());
    GameOverScreen gameOverScreen(game.getRenderer());

    while(game.isRunning())
    {
        while(SDL_PollEvent(&event))
        {
            if(menu.isActive())
                menu.handleEvent(event);
            else if(gameOverScreen.isActive())
                gameOverScreen.handleEvent(event);
            else
                game.handleEvent(event);

            if(event.type == SDL_QUIT) game.quit();
        }


        game.clear();

        if(menu.isActive())
            menu.draw();
        else if(gameOverScreen.isActive())
        {
            game.reset();
            gameOverScreen.draw();
        }
        else
        {
            game.update();
            game.render();

            if(game.isGameOver())
                gameOverScreen.show(game.getScore());
        }

        game.present();

        if(gameOverScreen.toGame())
        {
            gameOverScreen.resetFlags();
        }
        else if(gameOverScreen.toMenu())
        {
            gameOverScreen.resetFlags();
            menu.activate();
        }
    }

    game.clean();

    return 0;
}
