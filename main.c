#include "libs/project/imports.h"

int main()
{
    win = initWin("2D");
    renderer = win->renderer;

    /*
    win2 = initWin("3D");
    renderer2 = win2->renderer;
    */
    // SDL_SetRelativeMouseMode(1);

    // init map
    initWorld();

    // init player
    player = initPlayer();

    mainLoop(win, eventFunc, loopFunc);

    return 0;
}
