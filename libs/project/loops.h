
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
                    movePlayer(player, i);
                    break;
                }
            }

            break;
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
            current.x = i * current.w + worldX;
            SDL_RenderDrawRect(renderer, &current);
        }
    }
}

void drawPlayer(SDL_Renderer *renderer)
{

    // draw body
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    const int unit = DEP*4;

    SDL_Rect rect;
    rect.x = player->x - unit / 2;
    rect.y = player->y - unit / 2;
    rect.w = unit;
    rect.h = unit;

    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);

    // draw sight
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int px = player->x;
    int py = player->y;
    SDL_RenderDrawLine(renderer, px, py, px + player->ax * unit, py + player->ay * unit);
    
}

void drawRay(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    const int FOV = 60;    
    const int raysDiv = 3;



    int drawDistance;

    double ax;
    double ay;

    double x2;
    double y2;

    for (int a = player->a - FOV/2; a < player->a + FOV/2; a+=raysDiv) // draw multiple rays (10)
    {
        drawDistance = 10;

        ax = sin(a * RADIANS);
        ay = -cos(a * RADIANS);

        x2 = player->x + ax * drawDistance;
        y2 = player->y + ay * drawDistance;

        // normalize the coords
        int i = x2 / UNIT2D;
        int j = y2 / UNIT2D;

        while (!world[i + j * 8]) // wall not found
        {
            drawDistance += 1;
            // extend the ray if the wall is not found
            x2 = player->x + ax * drawDistance;
            y2 = player->y + ay * drawDistance;

            // normalize the coords
            i = x2 / UNIT2D;
            j = y2 / UNIT2D;
        }

        // wall found

        SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2);
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
    drawPlayer(renderer);

    // draw rays
    drawRay(renderer);

    //
    SDL_RenderPresent(renderer);
}

