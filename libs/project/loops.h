#define DEP 10
int deps[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
int keys[4] = {SDLK_w, SDLK_d, SDLK_s, SDLK_a};

#define UNIT2D 50

void eventFunc(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN)
    {
        for (int i = 0; i < 4; i++)
        {
            if (e.key.keysym.sym == keys[i])
            {
                player->x += deps[i][0] * DEP;
                player->y += deps[i][1] * DEP;
                break;
            }
        }
    }
}

void drawMap2D(SDL_Renderer *renderer)
{
    SDL_Rect current;
    current.w = UNIT2D;
    current.h = UNIT2D;

    // draw the walls
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int j = 0; j < 8; j++)
    {
        current.y = j * current.h + worldY;
        for (int i = 0; i < 8; i++)
        {
            if (world[i + 8 * j])
            {
                current.x = i * current.w + worldX;
                SDL_RenderFillRect(renderer, &current);
                SDL_RenderDrawRect(renderer, &current);
            }
        }
    }
    // draw the outlines
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 128);
    for (int j = 0; j < 8; j++)
    {
        current.y = j * current.h + worldY;
        for (int i = 0; i < 8; i++)
        {
            current.x = i * current.w +  worldX;
            SDL_RenderDrawRect(renderer, &current);
        }
    }
}

void loopFunc(Window *win)
{
    //
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //

    // draw 2 map
    drawMap2D(renderer);

    // draw player
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, player);
    SDL_RenderDrawRect(renderer, player);

    //
    SDL_RenderPresent(renderer);
}
