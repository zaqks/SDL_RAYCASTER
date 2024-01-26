typedef struct
{
    int x1;
    int y1;
    int w;
    int h;
    int x3;
    int y3;

} Parallelogram;

Parallelogram *initParallelogram(int x1,
                                 int y1,
                                 int w,
                                 int h,
                                 int x3,
                                 int y3)
{
    Parallelogram *shape = (Parallelogram *)malloc(sizeof(Parallelogram));

    shape->x3 = x3;
    shape->y3 = y3;

    shape->x1 = x1;
    shape->y1 = y1;
    shape->w = w;
    shape->h = h;

    

    return shape;
}

void drawParallelogram(SDL_Renderer *renderer, Parallelogram *shape, bool fill)
{

    int x2 = shape->x1 + shape->w + shape->x3;
    int y2 = shape->y1 + shape->h + shape->y3;

    // fill
    if (fill)
    {
      

        int dx1;
        int dy1;
        int dx2;
        int dy2;

        if (shape->x3)
        {
            x2 = shape->x1 + shape->x3;
            y2 = shape->y1 + shape->h + shape->y3;

            dx1 = shape->x1;
            dy1 = shape->y1;
            dx2 = x2;
            dy2 = y2;

            for (int i = 0; i < shape->w; i++)
            {
                SDL_RenderDrawLine(renderer, dx1, dy1, dx2, dy2);

                dx1 += 1;
                dx2 += 1;
            }
        }
        else
        {
            if (shape->y3)
            {
                dx1 = shape->x1;
                dy1 = shape->y1;
                dx2 = x2;
                dy2 = dy1 + shape->y3;

                for (int i = 0; i < shape->h; i++)
                {
                    SDL_RenderDrawLine(renderer, dx1, dy1, dx2, dy2);

                    dy1 += 1;
                    dy2 += 1;
                }
            }
        }
    }
    else
    {
        int lines[4][4] =
            {
                {shape->x1, shape->y1, x2 - shape->x3, shape->y1 + shape->y3}, // 1
                {x2 - shape->x3, shape->y1 + shape->y3, x2, y2},               // 2
                {shape->x1 + shape->x3, y2 - shape->y3, x2, y2},               // 3
                {shape->x1, shape->y1, shape->x1 + shape->x3, y2 - shape->y3}, // 4
            };

        for (int i = 0; i < 4; i++)
        {
            SDL_RenderDrawLine(renderer, lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
        }
    }
}

 