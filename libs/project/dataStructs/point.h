#define RADIANS M_PI / 180;

typedef struct
{
    double x;
    double y;
    double z;
} Point;

Point *initPoint(double x,
                 double y,
                 double z)
{
    Point *pnt = (Point *)malloc(sizeof(Point));
    pnt->x = x;
    pnt->y = y;
    pnt->z = z;

    return pnt;
}

Point *rotatePointX(Point *src, double a)
{
    /*
    x
    cy - sz
    sy + cz
    */
    double b = a * RADIANS;

    return initPoint(src->x, cos(b) * src->y - sin(b) * src->z, sin(b) * src->y + cos(b) * src->z);
}

Point *rotatePointY(Point *src, double a)
{
    /*
    cx + sz
    y
    -sx + cz
    */
    double b = a * RADIANS;

    return initPoint(cos(b) * src->x + sin(b) * src->z, src->y, -sin(b) * src->x + cos(b) * src->z);
}

Point *rotatePointZ(Point *src, double a)
{
    /*
    cx-sy
    sx+cy
    z
    */
    double b = a * RADIANS;

    return initPoint(
        cos(b) * src->x - sin(b) * src->y,
        sin(b) * src->x + cos(b) * src->y,
        src->z
    );
}
