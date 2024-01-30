#define FOV 66 // 66
#define FOV2 FOV / 2
#define VDIST 500 // vision dist

#define GRID true

#define MOUSE false

int moving = -1;
int rotating = 0;

int keys[4] = {SDLK_w, SDLK_d, SDLK_s, SDLK_a};

int distances[FOV][2] = {};

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
                // SDL_SetRenderDrawColor(renderer, 255 / (worldMap[j][i] + 1), 255 / (worldMap[j][i] + 1), 255 / (worldMap[j][i] + 1), 255);
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer, &current);
                SDL_RenderDrawRect(renderer, &current);
            }

            // grid
            if (GRID)
            {
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
                SDL_RenderDrawRect(renderer, &current2);
            }
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

void drawCenterSight(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT / 2;
    int z = 10; // length/2

    SDL_RenderDrawLine(renderer, x, y - z, x, y + z);
    SDL_RenderDrawLine(renderer, x - z, y, x + z, y);
}

void draw2DRays(SDL_Renderer *renderer)
{
    float ax;
    float ay;

    float x2;
    float y2;

    int i;
    int j;

    float dx;
    float d;

    for (float a = -FOV2 + player->a; a < FOV2 + player->a; a++)
    {

        ax = sin(a * RADIANS) * sqrt(2);
        ay = -cos(a * RADIANS) * sqrt(2);

        x2 = player->x;
        y2 = player->y;

        d = 0;
        dx = sqrt(pow(ax, 2) + pow(ay, 2));

        do
        {
            x2 += ax;
            y2 += ay;

            i = (x2 - worldX) / (UNIT2D);
            j = (y2 - worldY) / (UNIT2D);

            d += dx;

            if (!validCoords(i, j)) // existance check
            {
                if (d - dx > 0)
                {
                    d -= dx;
                }
                break;
            }
            if (worldMap[j][i])
            {
                break;
            }

        } while (d < VDIST);

        distances[((int)a + (int)FOV2 - (int)player->a)][0] = worldMap[j][i];
        distances[((int)a + (int)FOV2 - (int)player->a)][1] = d; // i, dist

        // 2D
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2);
    }
}

void draw3DRays(SDL_Renderer *renderer)
{
    float d;
    float lineH;
    float offsetY;
    float xR = 0;
    float xS = SCREEN_WIDTH / FOV;
    // 3D
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int x = 0; x < FOV; x++)
    {
        d = distances[x][1];
        if (d > 0)
        {
            xR = x * SCREEN_WIDTH / FOV;

            lineH = SCREEN_HEIGHT / (d)*WORLD_H;
            if (lineH > SCREEN_HEIGHT)
                lineH = SCREEN_HEIGHT;

            offsetY = (SCREEN_HEIGHT - lineH) / 2;

            SDL_RenderDrawLine(renderer, xR, offsetY, xR, offsetY + lineH);
        }
    }

    // anti alias
    float wallsDs[WALLS_NUM][FOV] = {};
    int wallsLines[WALLS_NUM] = {};
    for (int i = 0; i < WALLS_NUM; i++) // init indxs
    {
        wallsLines[i] = 0;
    }

    int currentWall;
    float currentD;
    int miniID;

    for (int i = 0; i < FOV; i++)
    {
        currentWall = distances[i][0];
        currentWall--;
        currentD = distances[i][1];

        miniID = wallsLines[currentWall];
        wallsLines[currentWall]++;

        wallsDs[currentWall][miniID] = currentD;
    }
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
    draw2DRays(renderer);

    SDL_RenderPresent(renderer);

    // window2
    SDL_SetRenderDrawColor(renderer2, 0, 0, 0, 255);
    SDL_RenderClear(renderer2);

    draw3DRays(renderer2);

    drawCenterSight(renderer2);
    SDL_RenderPresent(renderer2);
}
