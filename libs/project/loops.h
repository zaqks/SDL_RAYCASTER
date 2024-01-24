
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

    const int unit = DEP * 4;

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
    const int raysDiv = 1;

    int drawFact;

    double ax;
    double ay;

    double x2;
    double y2;

    double *distance;

    Queue *wallsH = initQueue();

    for (int a = player->a - FOV / 2; a < player->a + FOV / 2; a += raysDiv) // draw multiple rays (10)
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
        pushQueueNode(wallsH, distance);
    }

    // draw walls
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    double lineX = 0;
    double lineH;
    double offset;
    while (!emptyQueue(wallsH))
    {
        distance = popQueueNode(wallsH);

        lineH = SCREEN_HEIGHT * 8 / *distance;
        offset = (SCREEN_HEIGHT - lineH) / 2;
        SDL_RenderDrawLine(renderer, lineX, offset, lineX, lineH + offset);

        lineX += SCREEN_WIDTH / FOV;
        free(distance);
    }

    free(wallsH);
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

    //
    SDL_RenderPresent(renderer);
}
