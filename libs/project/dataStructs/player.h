#define RADIANS M_PI / 180
#define DEP 5

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
    player->a += a * 10;
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

    rslt->x = 225;
    rslt->y = 225;

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

    double newX = player->x + dirs[d][0] * DEP;
    double newY = player->y + dirs[d][1] * DEP;

    int i = (newX + (dirs[d][0] > 0 ? 1 : -1) * DEP * 2) / (double)UNIT2D;
    int j = (newY + (dirs[d][1] > 0 ? 1 : -1) * DEP * 2) / (double)UNIT2D;

    if (!world[i + j * 8])
    {
        player->x = newX;
        player->y = newY;
    }
}
