#include "libs/project/imports.h"

int main()
{
    win = initWin("DOOM");
    renderer = win->renderer;

    mainLoop(win, eventFunc, loopFunc);

    return 0;
}
