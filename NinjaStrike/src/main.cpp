#include "Game.h"
#include "Menu.h"

Game game;

int main(int argc, char* argv[])
{
    if(!game.init("Ninja Strike")) return 1;

    SDL_Event event;
    Menu menu(game.getRenderer());

    while(game.isRunning())
    {
        while(SDL_PollEvent(&event))
        {
            if(menu.isActive())
                menu.handleEvent(event);
            else
                game.handleEvent(event);

            if(event.type == SDL_QUIT) game.quit();
        }


        game.clear();

        if(menu.isActive()) menu.draw();
        else
        {
            game.update();
            game.render();
        }

        game.present();

    }
    game.clean();

    return 0;
}
