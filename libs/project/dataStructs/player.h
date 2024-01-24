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
    player->a += a;
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

    printf("\n%d deg\n", player->a);
    printf("%f\n", player->ax);
    printf("%f\n", player->ay);
}

Player *initPlayer()
{
    Player *rslt = (Player *)malloc(sizeof(Player));

    rslt->x = 250;
    rslt->y = 250;

    rslt->a = 0;

    rotatePlayer(rslt, 0);

    return rslt;
}

void movePlayer(Player *player, int d)
{
    // 0 1 2 3

    switch (d)
    {
    case 0:
        player->x += player->ax * DEP; // 1
        player->y += player->ay * DEP; // 1
        printf("forward\n");
        break;
    case 1:
        player->x += player->ax2 * DEP; // 1
        player->y += player->ay2 * DEP; // 1
        printf("forward\n");
        break;
    case 2:
        player->x -= player->ax * DEP; //-1
        player->y -= player->ay * DEP; //-1
        printf("backward\n");
        break;
    case 3:
        player->x -= player->ax2 * DEP; // -1
        player->y -= player->ay2 * DEP; // -1
        break;
    default:
        break;
    }

    printf("%f %f\n", player->x, player->y);
}