#define RADIANS M_PI / 180
#define DEP 2 // move by fact
#define SENS 3

typedef struct
{
    double x;
    double y;

    int a;
    double ax;
    double ay;

    // lateral mouvement
    double ax2;
    double ay2;

} Player;

void rotatePlayer(Player *player, int a)
{
    // angle update
    player->a += a * SENS;
    if (player->a == 360)
    {
        player->a = 0;
    }
    if (player->a == -1)
    {
        player->a = 359;
    }

    // ax ay
    player->ax = sin(player->a * RADIANS);
    player->ay = -cos(player->a * RADIANS);

    player->ax2 = sin((player->a + 90) * RADIANS);
    player->ay2 = -cos((player->a + 90) * RADIANS);

    /*
    printf("\n%d deg\n", player->a);
    printf("%f\n", player->ax);
    printf("%f\n", player->ay);
    */
}

Player *initPlayer()
{
    Player *rslt = (Player *)malloc(sizeof(Player));

    rslt->x = worldX + UNIT2D * 4;
    rslt->y = worldY + UNIT2D * 4;

    rslt->a = 0;

    rotatePlayer(rslt, 0);

    return rslt;
}

void movePlayer(Player *player, int d)
{
    // 0 1 2 3
    double dirs[4][2] = {
        {player->ax, player->ay},
        {player->ax2, player->ay2},
        {-player->ax, -player->ay},
        {-player->ax2, -player->ay2}};

    // x
    double newX = player->x + dirs[d][0] * DEP;
    double newY = player->y;

    int i = (newX - worldX) / (double)UNIT2D;
    int j = (newY - worldY) / (double)UNIT2D;

    if (!world[i + j * 8])
    {
        player->x = newX;
    }

    // y
    newX = player->x;
    newY = player->y + dirs[d][1] * DEP;

    i = (newX - worldX) / (double)UNIT2D;
    j = (newY - worldY) / (double)UNIT2D;

    if (!world[i + j * 8])
    {
        player->y = newY;
    }
}
