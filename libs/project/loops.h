/*
the projection is done from the player's
position plane
*/
#define DRAW_DIST 100 // vision dist
#define RAYSNUM 99

#define GRID true
#define MOUSE false

int moving = -1;
int rotating = 0;

int keys[4] = {SDLK_w, SDLK_d, SDLK_s, SDLK_a};

float distances[RAYSNUM] = {};

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
    float a;

    float x1;
    float y1;
    float x2;
    float y2;

    float ax1;
    float ay1;
    float ax2;
    float ay2;

    int i;
    int j;

    float dx;
    float d;
    bool found = false;

    ax1 = player->ax;
    ay1 = player->ay;
    ax2 = player->ax2;
    ay2 = player->ay2;

    dx = sqrt(pow(ax1, 2) + pow(ay1, 2));

    for (int d2 = -RAYSNUM / 2; d2 < RAYSNUM / 2 + 1; d2++)
    {
        x1 = player->x + ax2 * d2;
        y1 = player->y + ay2 * d2;
        x2 = player->x + ax2 * d2;
        y2 = player->y + ay2 * d2;

        i = x2 / (UNIT2D);
        j = y2 / (UNIT2D);

        d = 0;

        while (validCoords(i, j))
        {
            if (worldMap[j][i])
            {
                found = true;
                break;
            }

            x2 += ax1;
            y2 += ay1;
            d += dx;

            i = x2 / (UNIT2D);
            j = y2 / (UNIT2D);
        }

        distances[d2 + RAYSNUM / 2] = found ? d : -1;
        /*
        //distances[d2 + RAYSNUM / 2] = found ? (sqrt(pow(d, 2) + pow(d2, 2))) : -1; 
        what in the actual fuck should I do next
        if none of these fucking formulas worked
        */

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

float normalize255(float val) // for rgb codes & digital signals
{
    return (val > 255 ? 255 : (val < 0 ? 0 : val));
}

void draw3DRays(SDL_Renderer *renderer)
{
    int xS = SCREEN_WIDTH / (RAYSNUM); // xSpacing

    float currentD;
    float currentX = 0;
    float lineHeight;
    float offsetY;

    SDL_SetRenderDrawColor(renderer2, 255, 0, 0, 255);
    for (int x = 0; x < RAYSNUM; x++)
    {
        currentD = distances[x];

        if (currentD > 0)
        {
            lineHeight = SCREEN_HEIGHT * UNIT2D / currentD;

            if (lineHeight > SCREEN_HEIGHT)
            {
                lineHeight = SCREEN_HEIGHT;
            }

            offsetY = (SCREEN_HEIGHT - lineHeight) / 2;
            SDL_RenderDrawLine(renderer2, currentX, offsetY, currentX, lineHeight + offsetY);
        }

        currentX += xS;
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
    SDL_RenderDrawRect(renderer2, &sky);

    SDL_SetRenderDrawColor(renderer2, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer2, &sky);
    SDL_RenderDrawRect(renderer2, &ground);

    draw3DRays(renderer2);

    drawCenterSight(renderer2);
    SDL_RenderPresent(renderer2);
}
