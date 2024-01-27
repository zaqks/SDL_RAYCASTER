#define RADIANS M_PI / 180
#define DEP 2 // move by fact
#define SENS 3

typedef struct
{
    float x;
    float y;

    int a;
    float ax;
    float ay;

    // lateral mouvement
    float ax2;
    float ay2;

} Player;

void rotatePlayer(Player *player, int a)
{
    // angle update
    player->a += a * SENS;
    if ((player->a / abs(player->a)) * player->a >= 360)
    {
        player->a = 0;
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
    float dirs[4][2] = {
        {player->ax, player->ay},
        {player->ax2, player->ay2},
        {-player->ax, -player->ay},
        {-player->ax2, -player->ay2}};

    // x
    float newX = player->x + dirs[d][0] * DEP;
    float newY = player->y;

    int i = (newX - worldX) / (float)UNIT2D;
    int j = (newY - worldY) / (float)UNIT2D;

    if (!world[i + j * 8])
    {
        player->x = newX;
    }

    // y
    newX = player->x;
    newY = player->y + dirs[d][1] * DEP;

    i = (newX - worldX) / (float)UNIT2D;
    j = (newY - worldY) / (float)UNIT2D;

    if (!world[i + j * 8])
    {
        player->y = newY;
    }
}
