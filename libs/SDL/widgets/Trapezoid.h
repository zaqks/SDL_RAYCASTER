typedef struct
{
    float x, y, d1, d2, l;

} Trapezoid;

Trapezoid *initTrapezoid(float x, float y, float d1, float d2, float l)
{
    Trapezoid *widget = (Trapezoid *)malloc(sizeof(Trapezoid));
    widget->x = x;
    widget->y = y;

    widget->d1 = d1;
    widget->d2 = d2;
    widget->l = l;

    return widget;
}

void drawTrapezoid(SDL_Renderer *renderer, Trapezoid *widget)
{
    float x, y, d1, d2, l;
    x = widget->x;
    y = widget->y;
    d1 = widget->d1;
    d2 = widget->d2;
    l = widget->l;

    float a;

    a = (d2 - d1) / (float)2;
    a /= l;
    a *= -1;

    float x1, y1, x2, y2;
    x1 = x;
    x2 = x;

    y1 = y;
    y2 = y1 + d1;

    for (int i = 0; i < l; i++) // draw relatively to the center of d1
    {
        SDL_RenderDrawLine(renderer, x1 + i, y1 + i * a, x2 + i, y2 - i * a);
    }
}