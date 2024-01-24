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
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    const int FOV = 135;

    int drawFact;

    double ax;
    double ay;

    double x2;
    double y2;

    double *distance;
    Queue *walls = initQueue();

    for (double a = player->a - FOV / 2; a < player->a + FOV / 2; a += 1) // draw multiple rays (10)
    {
        drawFact = DEP * 2;

        ax = sin(a * RADIANS);
        ay = -cos(a * RADIANS);

        x2 = player->x + ax * drawFact;
        y2 = player->y + ay * drawFact;

        // normalize the coords
        int i = (x2 - worldX) / UNIT2D;
        int j = (y2 - worldY) / UNIT2D;

        while (!world[i + j * 8]) // wall not found
        {
            drawFact += DEP;
            // extend the ray if the wall is not found
            x2 = player->x + ax * drawFact;
            y2 = player->y + ay * drawFact;

            // normalize the coords
            i = (x2 - worldX) / UNIT2D;
            j = (y2 - worldY) / UNIT2D;
        }

        // wall found
        SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2); // draw ray

        // wall distance
        distance = (double *)malloc(sizeof(double));
        *distance = pow(pow(x2 - player->x, 2) + pow(y2 - player->y, 2), 0.5);

        pushQueueNode(walls, distance);
    }

    // draw wall
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    double lineX = 0;
    double lineXS = SCREEN_WIDTH / (double)FOV;

    double lineH;
    double offset;

    while (!emptyQueue(walls))
    {
        distance = popQueueNode(walls);

        lineH = SCREEN_HEIGHT * 8 / *distance;
        offset = (SCREEN_HEIGHT - lineH) / (double)2;

        SDL_RenderDrawLine(renderer, lineX, offset, lineX, offset + lineH);

        free(distance);
        lineX += lineXS;
    }
    free(walls);
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
    //
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //

    // draw rays
    drawRays(renderer);

    // draw 2 map
    drawMap2D(renderer);
    // draw player
    drawPlayer(renderer);

    drawCenterSight(renderer);

    //
    SDL_RenderPresent(renderer);
}
