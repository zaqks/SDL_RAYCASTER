#include "libs/project/imports.h"

int main()
{
    win = initWin("DOOM");
    renderer = win->renderer;
    SDL_SetRelativeMouseMode(1);
    
    //init map
    initWorld();

    // init player
    player = initPlayer();

    mainLoop(win, eventFunc, loopFunc);

    return 0;
}
