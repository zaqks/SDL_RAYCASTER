#include "libs/project/imports.h"

int main()
{
    win = initWin("2D");
    renderer = win->renderer;

    win2 = initWin("3D");
    renderer2 = win2->renderer;

    // SDL_SetRelativeMouseMode(1);

    // init map
    initWorld();

    // init player
    player = initPlayer();

    // init 3d stuff
    sky.x = 0;
    sky.y = 0;
    sky.w = SCREEN_WIDTH;
    sky.h = SCREEN_HEIGHT / 2;

    ground.x = 0;
    ground.y = SCREEN_HEIGHT / 2;
    ground.w = SCREEN_WIDTH;
    ground.h = SCREEN_HEIGHT / 2;

    mainLoop(win, eventFunc, loopFunc);

    return 0;
}
