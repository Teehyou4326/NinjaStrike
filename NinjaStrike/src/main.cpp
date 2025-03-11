#include <SDL.h>
#include <iostream>

#include "Texture.h"
#include "Game.h"


Texture ninja;
Game game;

int main(int argc, char* argv[])
{
    if(!game.init("Ninja Strike")) return 1;
    while(game.isRunning())
    {
        game.handleEvent();
        game.update();
        game.render();
    }
    game.clean();

    return 0;
}
