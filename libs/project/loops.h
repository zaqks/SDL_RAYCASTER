int moving = -1;

int keys[4] = {SDLK_w, SDLK_d, SDLK_s, SDLK_a};

void eventFunc(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
            // rotation
        case SDLK_RIGHT:
            rotatePlayer(player, 1);

            break;
        case SDLK_LEFT:
            rotatePlayer(player, -1);

            break;

        default:
            // mouvement
            for (int i = 0; i < 4; i++)
            {
                if (keys[i] == e.key.keysym.sym)
                {
                    moving = i;

                    break;
                }
            }

            break;
        }
    }
    if (e.type == SDL_KEYUP)
    {
        moving = -1;
    }

    if (e.type == SDL_MOUSEMOTION)
    {
        rotatePlayer(player, e.motion.xrel > 0 ? 1 : -1);
    }
}

void drawMap2D(SDL_Renderer *renderer)
{
    SDL_Rect current;
    current.w = UNIT2D - 1;
    current.h = UNIT2D - 1;

    // draw the walls
    for (int j = 0; j < 8; j++)
    {
        current.y = j * current.h + worldY;
        current.y += j;

        for (int i = 0; i < 8; i++)
        {
            if (world[i + 8 * j])
            {
                current.x = i * current.w + worldX;
                current.x += i;

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &current);
                SDL_RenderDrawRect(renderer, &current);
            }
        }
    }
}

void drawPlayer(SDL_Renderer *renderer)
{

    // draw body
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_RenderDrawPoint(renderer, player->x, player->y);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int px = player->x;
    int py = player->y;
    const int unit = 20;

    SDL_RenderDrawLine(renderer, px, py, px + player->ax * unit, py + player->ay * unit); // y
    SDL_RenderDrawLine(renderer,
                       px - player->ax2 * unit/2,
                       py - player->ay2 * unit/2,
                       px + player->ax2 * unit/2,
                       py + player->ay2 * unit/2); // x
}

void drawRays(SDL_Renderer *renderer)
{
}

void drawCenterSight(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT / 2;
    int z = 10; // length/2

    SDL_RenderDrawLine(renderer, x, y - z, x, y + z);
    SDL_RenderDrawLine(renderer, x - z, y, x + z, y);
}

void loopFunc(Window *win)
{
    // move
    if (moving >= 0)
    {
        movePlayer(player, moving);
    }

    //
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //

    // draw rays
    drawRays(renderer);

    // draw 2 map
    drawMap2D(renderer);
    drawPlayer(renderer); // draw player

    drawCenterSight(renderer);

    //
    SDL_RenderPresent(renderer);
}
