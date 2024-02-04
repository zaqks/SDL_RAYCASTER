/*
the prjection is done from the
player's position point
*/

#define FOV 66 // 66
#define FOV2 FOV / 2
#define DRAW_DIST 100 // vision dist
#define RAYSNUM FOV

#define GRID true
#define MOUSE false

int moving = -1;
int rotating = 0;

int keys[4] = {SDLK_w, SDLK_d, SDLK_s, SDLK_a};

bool found;
float wallsD[RAYSNUM][2]; // wall num, dist
float wallsD2[4][4] = {}; // min and max distances (x, d1, d2, l) x: current indx to obtain the real x coord (x refers to d1's x)

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

    // init the walls
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            wallsD2[i][j] = -1;
        }
    }

    for (float a = -FOV2 + player->a; a < FOV2 + player->a; a += (FOV) / (float)(RAYSNUM))
    {
        found = false;

        ax = sin(a * RADIANS);  //* sqrt(2);
        ay = -cos(a * RADIANS); //* sqrt(2);

        x2 = player->x;
        y2 = player->y;

        i = (x2 - worldX) / (UNIT2D);
        j = (y2 - worldY) / (UNIT2D);

        d = 0;
        dx = sqrt(pow(ax, 2) + pow(ay, 2));

        while (validCoords(i, j))
        {
            if (worldMap[j][i]
                //|| d > DRAW_DIST
            )
            {
                found = true;
                break;
            }

            x2 += ax;
            y2 += ay;

            i = (x2 - worldX) / (UNIT2D);
            j = (y2 - worldY) / (UNIT2D);

            d += dx;
        };

        wallsD[(int)(a + FOV2 - player->a)][0] = found ? worldMap[j][i] : 0;
        wallsD[(int)(a + FOV2 - player->a)][1] = found ? d : -1;
        /*
        currentDistance = (float *)malloc(sizeof(float));
        *currentDistance = found ? d : -1;
        pushQueueNode(distances, currentDistance);
        */

        // draw
        if (found)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2);

            int wallIndx = worldMap[j][i] - 1;
            // save the wals vals
            // worldMap[j][i] - 1 => this is the wall indx
            // wallsD2[wallIndx][0] => x
            // wallsD2[wallIndx][1] => d1
            // wallsD2[wallIndx][2] => d2
            // wallsD2[wallIndx][3] => l
            if (wallsD2[wallIndx][0] > 0) // d1 already exists soo update d2 and l
            {
                wallsD2[wallIndx][2] = d;
                wallsD2[wallIndx][3] = (int)(a + FOV2 - player->a) - wallsD2[wallIndx][0];
            }
            else // create d1 and x
            {
                wallsD2[wallIndx][0] = (int)(a + FOV2 - player->a);
                wallsD2[wallIndx][1] = d;
            }
        }
    }
}

float normalize255(float val)
{
    return (val > 255 ? 255 : (val < 0 ? 0 : val));
}

void draw3DRays(SDL_Renderer *renderer)
{

    int currentWall;
    float currentD;
    float currentX;

    float lineH;
    float offsetY;

    for (int i = 0; i < RAYSNUM; i++)
    {
        currentWall = wallsD[i][0];
        currentD = wallsD[i][1];

        if (currentD > 0)
        {
            currentX = i * SCREEN_WIDTH / (float)(FOV);
            SDL_SetRenderDrawColor(renderer, normalize255(255 * (DRAW_DIST / currentD)), 0, 0, 255);

            lineH = SCREEN_HEIGHT * UNIT2D / currentD;
            if (lineH > SCREEN_HEIGHT)
            {
                lineH = SCREEN_HEIGHT;
            }
            offsetY = (SCREEN_HEIGHT - lineH) / 2;

            for (float j = 0; j < SCREEN_WIDTH / (float)(FOV); j++)
            {
                SDL_RenderDrawLine(renderer, currentX + j, offsetY, currentX + j, offsetY + lineH);
            }

            SDL_RenderDrawLine(renderer, currentX, offsetY, currentX, offsetY + lineH);
        }

        // let's do some stats

        /*
        for each wall:
        get the first d
        get the last d
        take into consideration the distance between the lines
        to get the right variation
        then resize every line except of the first and last one
        */
    }

    // let's draw using the new method
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    Trapezoid *wall;
    float gH; // greatest height to get the y coord
    float x, y;
    for (int i = 0; i < 4; i++)
    {
        gH = fmax(wallsD2[i][1], wallsD2[i][2]);
        y = (SCREEN_HEIGHT - gH) / 2;

        x = wallsD2[i][0] * SCREEN_WIDTH / (float)(FOV);
        wall = initTrapezoid(x, y, wallsD2[i][2], wallsD2[i][1], wallsD2[i][3] * SCREEN_WIDTH / (float)(FOV));

        drawTrapezoid(renderer2, wall);
        free(wall);
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

    SDL_SetRenderDrawColor(renderer2, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer2, &ground);
    SDL_SetRenderDrawColor(renderer2, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer2, &sky);

    SDL_RenderDrawRect(renderer2, &sky);
    SDL_RenderDrawRect(renderer2, &ground);

    draw3DRays(renderer2);

    drawCenterSight(renderer2);
    SDL_RenderPresent(renderer2);
}
