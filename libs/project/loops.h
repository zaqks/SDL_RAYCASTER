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
    for (int j = 0; j < 8; j++)
    {
        current.y = j * current.h + worldY;
        current2.y = j * current2.h + worldY;

        for (int i = 0; i < 8; i++)
        {
            current.x = i * current.w + worldX;
            current2.x = i * current2.w + worldX;

            if (world[i + 8 * j]) // wall
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &current);
                SDL_RenderDrawRect(renderer, &current);
            }

            // guide
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
            SDL_RenderDrawRect(renderer, &current2);
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
                       px - player->ax2 * unit / 2,
                       py - player->ay2 * unit / 2,
                       px + player->ax2 * unit / 2,
                       py + player->ay2 * unit / 2); // x
}

void drawRays(SDL_Renderer *renderer)
{
    bool vert = true;
    bool horz = true;

    for (int a = 0; a < 4; a++) // a%2 0 vert 1 horz
    {
        if (abs(player->a) == a * 90)
        {
            if (a % 2) // horz
            {
                horz = false;
            }
            else // vert
            {
                vert = false;
            }
        }
    }

    if (horz || vert) // check if we need to init the vars
    {

        int rX = (int)player->x % UNIT2D;
        int rY = (int)player->y % UNIT2D;

        double ta = tan(RADIANS * (player->a));

        double dx;
        double dy;

        double x2; // h
        double y2; // h
        double d2 = 0;

        double x3; // v
        double y3; // v
        double d3 = 0;

        int i;
        int j;
        int indx;

        double dx2;
        double dy2;

        if (horz) // horizontal intersection (don't check for |90deg| ya l boubou)

        {
            // int lUP = ((abs(player->a) <= 90 || abs(player->a) >= 270) ? 1 : -1); // are we looking UP or down :/
            int lUP = -player->ay / fabs(player->ay);

            dy = (lUP == 1 ? rY : UNIT2D - rY);
            dx = dy * ta;

            x2 = player->x + dx * lUP;
            y2 = player->y - dy * lUP; // invert the y

            i = x2 / UNIT2D;
            j = y2 / UNIT2D - (lUP == 1 ? 1 : 0);
            indx = i + 8 * j;

            dy2 = UNIT2D;
            dx2 = dy2 * ta;

            while (!world[indx])
            {
                x2 += dx2 * lUP;
                y2 -= dy2 * lUP;

                i = x2 / UNIT2D;
                j = y2 / UNIT2D - (lUP == 1 ? 1 : 0);
                indx = i + 8 * j;

                if (indx > 63 || indx < 0)
                {
                    break;
                }
            }

            d2 = pow(pow(x2 - player->x, 2) + pow(y2 - player->y, 2), 0.5);

            // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            // SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2);
        }

        if (vert) // vertical intersection}
        {
            int lRH = player->ax / fabs(player->ax);

            dx = (lRH == 1 ? UNIT2D - rX : rX);
            dy = dx / ta;

            x3 = player->x + dx * lRH;
            y3 = player->y - dy * lRH;

            i = x3 / UNIT2D - (lRH == 1 ? 0 : 1);
            j = y3 / UNIT2D;
            indx = i + 8 * j;

            dx2 = UNIT2D;
            dy2 = dx2 / ta;

            while (!world[indx])
            {
                x3 += dx2 * lRH;
                y3 -= dy2 * lRH;

                i = x3 / UNIT2D - (lRH == 1 ? 0 : 1);
                j = y3 / UNIT2D;
                indx = i + 8 * j;

                if (indx > 63 || indx < 0)
                {
                    break;
                }
            }

            d3 = pow(pow(x3 - player->x, 2) + pow(y3 - player->y, 2), 0.5);

            // SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            // SDL_RenderDrawLine(renderer, player->x, player->y, x3, y3);
        }

        // get the shortest distance and draw
        /*
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, player->x, player->y, x3, y3);
        */

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        if (d2 < d3)
        {
            SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2);
        }
        else
        {
            SDL_RenderDrawLine(renderer, player->x, player->y, x3, y3);
        }
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
