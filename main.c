#include "libs/project/imports.h"

int main()
{
    win = initWin("DOOM");
    renderer = win->renderer;

    // init player
    player = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    player->w = 10;
    player->h = 10;
    player->x = 100;
    player->y = 100;

    mainLoop(win, eventFunc, loopFunc);

    return 0;
}
