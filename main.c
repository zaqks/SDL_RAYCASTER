#include "libs/project/imports.h"

int main()
{
    win = initWin("DOOM");
    renderer = win->renderer;

    // init player
    player = initPlayer();

    mainLoop(win, eventFunc, loopFunc);

    return 0;
}
