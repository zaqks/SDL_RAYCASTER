#define FOV 45 // 66
#define FOV2 FOV / 2

#define MOUSE false

int moving = -1;
int rotating = 0;

int keys[4] = {SDLK_w, SDLK_d, SDLK_s, SDLK_a};

void eventFunc(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
            // rotation
        case SDLK_RIGHT:
            rotating = 1;
            break;
        case SDLK_LEFT:
            rotating = -1;
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

    if (MOUSE)
    {
        if (e.type == SDL_MOUSEMOTION)
        {
            rotatePlayer(player, e.motion.xrel > 0 ? 1 : -1);
        }
    }
}

void drawMap2D(SDL_Renderer *renderer)
{
    SDL_Rect current;
    current.w = UNIT2D;
    current.h = UNIT2D;

    SDL_Rect current2;
    current2.w = UNIT2D;
    current2.h = UNIT2D;

    // draw the walls
    for (int j = 0; j < WORLD_H; j++)
    {
        current.y = j * current.h + worldY;
        current2.y = j * current2.h + worldY;

        for (int i = 0; i < WORLD_W; i++)
        {
            current.x = i * current.w + worldX;
            current2.x = i * current2.w + worldX;

            if (worldMap[j][i]) // wall
            {
                SDL_SetRenderDrawColor(renderer, 255 / (worldMap[j][i] + 1), 255 / (worldMap[j][i] + 1), 255 / (worldMap[j][i] + 1), 255);
                SDL_RenderFillRect(renderer, &current);
                SDL_RenderDrawRect(renderer, &current);
            }

            // guide
            SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            SDL_RenderDrawRect(renderer, &current2);
        }
    }
}

void drawPlayer(SDL_Renderer *renderer)
{

    // draw body
    /*
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, player->x, player->y);
    */

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    int px = player->x;
    int py = player->y;
    const int unit = 20;

    SDL_RenderDrawLine(renderer, px, py, px + player->ax * unit, py + player->ay * unit); // y
    SDL_RenderDrawLine(renderer,
                       px - player->ax2 * unit / 2,
                       py - player->ay2 * unit / 2,
                       px + player->ax2 * unit / 2,
                       py + player->ay2 * unit / 2); // x
}

void drawRays(SDL_Renderer *renderer)
{
    float ax;
    float ay;

    float x2;
    float y2;

    int i;
    int j;

    float d;

    float lineH;
    float offsetY;
    float x = 0;
    ;

    for (float a = -FOV2 + player->a; a < FOV2 + player->a; a++)
    {

        ax = sin(a * RADIANS);
        ay = -cos(a * RADIANS);

        x2 = player->x;
        y2 = player->y;

        do
        {

            x2 += ax;
            y2 += ay;

            i = x2 / (UNIT2D);
            j = y2 / (UNIT2D);

        } while (!worldMap[j][i]);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2);

        // 3D

        d = pow(pow(y2 - player->y, 2) + pow(x2 - player->x, 2), 0.5);

        lineH = SCREEN_HEIGHT / d;
        offsetY = (SCREEN_HEIGHT - lineH) / 2;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, x, offsetY, x, offsetY + lineH);
        x++;
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

    if (rotating)
    {
        rotatePlayer(player, rotating);
        rotating = 0;
    }

    //
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //

    // draw 2 map
    drawMap2D(renderer);
    drawPlayer(renderer); // draw player

    // draw rays
    drawRays(renderer);

    // drawCenterSight(renderer);
    //
    SDL_RenderPresent(renderer);
}
