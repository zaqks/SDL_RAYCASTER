#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define REFRESHRATE 33
#define FULLSCREEN false

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

typedef struct
{
    int width;
    int height;
    SDL_Window *win;
    SDL_Renderer *renderer;
    //
    int padding;
} Window;

Window *initWin(char *title)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Init(SDL_INIT_VIDEO);

    if (FULLSCREEN)
    {
        SDL_DisplayMode dm;
        SDL_GetDesktopDisplayMode(0, &dm);
        SCREEN_WIDTH = dm.w;
        SCREEN_HEIGHT = dm.h;
    }

    Window *newWin = (Window *)malloc(sizeof(Window));

    newWin->width = SCREEN_WIDTH;
    newWin->height = SCREEN_HEIGHT;
    newWin->padding = SCREEN_WIDTH / SCREEN_HEIGHT;

    char *newTitle = "window";
    if (title != NULL)
    {
        newTitle = title;
    }

    newWin->win = SDL_CreateWindow(newTitle, 10, 10, newWin->width, newWin->height, SDL_WINDOW_SHOWN);

    if (FULLSCREEN)
    {
        SDL_SetWindowFullscreen(newWin->win, FULLSCREEN);
        SDL_ShowCursor(0);
    }

    newWin->renderer = SDL_CreateRenderer(newWin->win, -1, 0);
    SDL_SetRenderDrawColor(newWin->renderer, 0, 0, 0, 255);
    SDL_RenderClear(newWin->renderer);

    TTF_Init();

    return newWin;
}

void killWin(Window *win)
{
    // kill widnow
    SDL_DestroyWindow(win->win);

    // quit sdl
    SDL_Quit();

    free(win);
}

void mainLoop(Window *win, void (*eFunc)(SDL_Event e), void (*func)(Window *win))
{
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = true;
                }
            }
            //
            if (eFunc != NULL)
            {
                (*eFunc)(e);
            }
            //
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        if (func != NULL)
        {
            (*func)(win);
        }

        SDL_Delay(REFRESHRATE);
    }
    killWin(win);
}
