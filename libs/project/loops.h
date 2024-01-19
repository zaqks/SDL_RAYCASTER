

void eventFunc(SDL_Event e)
{
}

void loopFunc(Window *win)
{
    //
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //
    SDL_RenderPresent(renderer);
}
