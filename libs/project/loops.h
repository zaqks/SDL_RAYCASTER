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

    const int unit = DEP;

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

void drawRays(SDL_Renderer *renderer)
{

    const int FOV = 61; // FOV rays

    int currentDistFact;

    double ax;
    double ay;

    double x2;
    double y2;

    double distance;

    double lineX = 0;
    double lineXS = SCREEN_WIDTH / (double)FOV;

    double lineH;
    double offset;

    for (int a = player->a - FOV / 2; a < player->a + FOV / 2; a += 1) // draw multiple rays (10)
    {
        currentDistFact = 1;

        ax = sin(a * RADIANS);
        ay = -cos(a * RADIANS);

        x2 = player->x + ax * currentDistFact;
        y2 = player->y + ay * currentDistFact;

        // normalize the coords
        int i = (x2 - worldX) / UNIT2D;
        int j = (y2 - worldY) / UNIT2D;

        while (!world[i + j * 8]) // wall not found
        {
            currentDistFact += 1;
            // extend the ray if the wall is not found
            x2 = player->x + ax * currentDistFact;
            y2 = player->y + ay * currentDistFact;

            // normalize the coords
            i = (x2 - worldX) / (double)UNIT2D;
            j = (y2 - worldY) / (double)UNIT2D;
        }

        // wall found
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2); // draw ray

        // wall distance
        distance = pow(pow(x2 - player->x, 2) + pow(y2 - player->y, 2), 0.5);

        // draw wall
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        distance *= cos(RADIANS * (player->a - a));
        lineH = SCREEN_HEIGHT * 8 / distance;
        offset = (SCREEN_HEIGHT - lineH) / (double)2;

        for (double i = 0; i < lineXS; i++)
        {
            SDL_RenderDrawLine(renderer, lineX + i, offset, lineX + i, offset + lineH);
        }

        lineX += lineXS;
    }
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
