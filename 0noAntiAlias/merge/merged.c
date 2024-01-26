#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define REFRESHRATE 1000 / 30 // 1000/fps

#define FULLSCREEN false

#define CURSOR false

#define SCREEN_X 50

#define SCREEN_Y 50


int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

typedef struct
{
    int width;
    int height;
    SDL_Window *win;
    SDL_Renderer *renderer;
    //
    int padding;
} Window;

Window *initWin(char *title)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Init(SDL_INIT_VIDEO);

    if (FULLSCREEN)
    {
        SDL_DisplayMode dm;
        SDL_GetDesktopDisplayMode(0, &dm);
        SCREEN_WIDTH = dm.w;
        SCREEN_HEIGHT = dm.h;
    }

    Window *newWin = (Window *)malloc(sizeof(Window));

    newWin->width = SCREEN_WIDTH;
    newWin->height = SCREEN_HEIGHT;
    newWin->padding = SCREEN_WIDTH / SCREEN_HEIGHT;

    char *newTitle = "window";
    if (title != NULL)
    {
        newTitle = title;
    }

    newWin->win = SDL_CreateWindow(newTitle, SCREEN_X, SCREEN_Y, newWin->width, newWin->height, SDL_WINDOW_SHOWN);

    if (FULLSCREEN)
    {
        SDL_SetWindowFullscreen(newWin->win, FULLSCREEN);
    }

    if (!CURSOR)
    {
        SDL_ShowCursor(0);
    }

    newWin->renderer = SDL_CreateRenderer(newWin->win, -1, 0);
    SDL_SetRenderDrawColor(newWin->renderer, 0, 0, 0, 255);
    SDL_RenderClear(newWin->renderer);

    TTF_Init();

    return newWin;
}

void killWin(Window *win)
{
    // kill widnow
    SDL_DestroyWindow(win->win);

    // quit sdl
    SDL_Quit();

    free(win);
}

void mainLoop(Window *win, void (*eFunc)(SDL_Event e), void (*func)(Window *win))
{
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = true;
                }
            }
            //
            if (eFunc != NULL)
            {
                (*eFunc)(e);
            }
            //
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        if (func != NULL)
        {
            (*func)(win);
        }

        SDL_Delay(REFRESHRATE);
    }
    killWin(win);
}

//recursivity support for lists


typedef struct listNode
{
    void *val;
    struct listNode *next;
} ListNode;

typedef struct
{
    ListNode *head;
} List;

int getListLengthRec(ListNode *head, int indx)
{
    if (!head)
    {
        return indx;
    }
    getListLengthRec(head->next, indx + 1);
}

ListNode *getListQueueRec(ListNode *head)
{
    if (!head->next)
    {
        return head;
    }
    getListQueueRec(head->next);
}

void printListRec(ListNode *head)
{
    if (head)
    {
        printf("%p ", head->val);
        printListRec(head->next);
    }
}
List *initList()
{
    List *lst = (List *)malloc(sizeof(List));
    lst->head = NULL;
}

int getListLength(List *lst)
{
    return getListLengthRec(lst->head, 0);
}

ListNode *getListQueue(List *lst)
{
    return getListQueueRec(lst->head);
}

// queue mode
void addListNode1(List *lst, void *val)
{
    ListNode *newElem = (ListNode *)malloc(sizeof(ListNode));
    newElem->val = val;
    newElem->next = NULL;

    if (!lst->head)
    {
        lst->head = newElem;
    }
    else
    {
        // find the last one
        ListNode *current = lst->head;
        getListQueue(lst)->next = newElem;
    }
}
// stack mode
void addListNode2(List *lst, void *val)
{
    ListNode *newElem = (ListNode *)malloc(sizeof(ListNode));
    newElem->val = val;
    newElem->next = lst->head;

    lst->head = newElem;
}

int removeListNode(List *lst, int indx)
{
    if (indx < getListLength(lst))
    {
        ListNode *before = NULL;
        ListNode *current = lst->head;
        for (int i = 0; i < indx; i++)
        {
            before = current;
            current = current->next;
        }
        //
        // i = 0 whdha
        if (!indx)
        {
            lst->head = current->next;
        }
        // i = len - 1 whda

        //
        if (before)
        {
            before->next = current->next;
        }
        free(current);
        return 1;
    }
    printf("invalid indx\n");
    return 0;
}

void printList(List *lst)
{
    printf("[");
    printListRec(lst->head);
    printf("]");
}



typedef ListNode QueueNode;
typedef List Queue;

Queue *initQueue()
{
    return (Queue *)initList();
}

void pushQueueNode(Queue *lst, void *val)
{
    addListNode1(lst, val);
}

void *popQueueNode(Queue *lst)
{
    void *rslt = (void *)(lst->head->val);
    removeListNode(lst, 0);

    return rslt;
}

int queueLength(Queue *queue)
{
    return getListLength(queue);
}

int emptyQueue(Queue *queue)
{
    return !getListLength(queue);
}

void printQueue(Queue *queue)
{
    char *seprator = "______________";
    printf("%s\n", seprator);
    //

    void *val;
    for (int i = 0; i < queueLength(queue); i++)
    {
        val = popQueueNode(queue);
        pushQueueNode(queue, val);
        printf("%p\n%s\n", val, seprator);
    }
}

#define UNIT2D 30


int worldX = 0;
int worldY = 0;

int world[] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1};

void initWorld()
{
    worldX = 0;
    worldY = SCREEN_HEIGHT - UNIT2D * 8;
}
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

    int i = (newX + (dirs[d][0] > 0 ? 1 : -1) * DEP / 2 - worldX) / (double)UNIT2D;
    int j = (newY + (dirs[d][1] > 0 ? 1 : -1) * DEP / 2 - worldY) / (double)UNIT2D;

    if (!world[i + j * 8])
    {
        player->x = newX;
    }

    // y
    newX = player->x;
    newY = player->y + dirs[d][1] * DEP;

    i = (newX + (dirs[d][0] > 0 ? 1 : -1) * DEP / 2 - worldX) / (double)UNIT2D;
    j = (newY + (dirs[d][1] > 0 ? 1 : -1) * DEP / 2 - worldY) / (double)UNIT2D;

    if (!world[i + j * 8])
    {
        player->y = newY;
    }
}

Window *win;
SDL_Renderer *renderer;

Player *player;
int moving = -1;
int keys[4] = {SDLK_w, SDLK_d, SDLK_s, SDLK_a};

void eventFunc(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
            // rotation
        case SDLK_RIGHT:
            rotatePlayer(player, 1);
            break;
        case SDLK_LEFT:
            rotatePlayer(player, -1);
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

    if (e.type == SDL_MOUSEMOTION)
    {
        rotatePlayer(player, e.motion.xrel > 0 ? 1 : -1);
    }
}

void drawMap2D(SDL_Renderer *renderer)
{
    SDL_Rect current;
    current.w = UNIT2D - 1;
    current.h = UNIT2D - 1;

    // draw the walls
    for (int j = 0; j < 8; j++)
    {
        current.y = j * current.h + worldY;
        current.y += j;

        for (int i = 0; i < 8; i++)
        {
            if (world[i + 8 * j])
            {
                current.x = i * current.w + worldX;
                current.x += i;

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &current);
                SDL_RenderDrawRect(renderer, &current);
            }
        }
    }
}

void drawPlayer(SDL_Renderer *renderer)
{

    // draw body
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    const int unit = DEP;

    SDL_Rect rect;
    rect.x = player->x - unit / 2;
    rect.y = player->y - unit / 2;
    rect.w = unit;
    rect.h = unit;

    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);

    // draw sight

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int px = player->x;
    int py = player->y;
    SDL_RenderDrawLine(renderer, px, py, px + player->ax * unit, py + player->ay * unit);
}

void drawRays(SDL_Renderer *renderer)
{

    const int FOV = 80; // FOV rays

    int currentDistFact;

    double ax;
    double ay;

    double x2;
    double y2;

    double distance;

    double lineX = 0;
    double lineXS = SCREEN_WIDTH / (double)FOV;

    double lineH;
    double offset;

    for (int a = player->a - FOV / 2; a < player->a + FOV / 2; a += 1) // draw multiple rays (10)
    {
        currentDistFact = 1;

        ax = sin(a * RADIANS);
        ay = -cos(a * RADIANS);

        x2 = player->x + ax * currentDistFact;
        y2 = player->y + ay * currentDistFact;

        // normalize the coords
        int i = (x2 - worldX) / UNIT2D;
        int j = (y2 - worldY) / UNIT2D;

        while (!world[i + j * 8]) // wall not found
        {
            currentDistFact += 1;
            // extend the ray if the wall is not found
            x2 = player->x + ax * currentDistFact;
            y2 = player->y + ay * currentDistFact;

            // normalize the coords
            i = (x2 - worldX) / (double)UNIT2D;
            j = (y2 - worldY) / (double)UNIT2D;
        }

        // wall found
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawLine(renderer, player->x, player->y, x2, y2); // draw ray

        // wall distance
        distance = pow(pow(x2 - player->x, 2) + pow(y2 - player->y, 2), 0.5);

        // draw wall
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        distance *= cos(RADIANS * (player->a - a));
        lineH = SCREEN_HEIGHT * 8 / distance;
        offset = (SCREEN_HEIGHT - lineH) / (double)2;

        for (double i = 0; i < lineXS; i++)
        {
            SDL_RenderDrawLine(renderer, lineX + i, offset, lineX + i, offset + lineH);
        }

        lineX += lineXS;
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

    //
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);
    //

    // draw rays
    drawRays(renderer);

    // draw 2 map
    drawMap2D(renderer);
    drawPlayer(renderer); // draw player

    drawCenterSight(renderer);

    //
    SDL_RenderPresent(renderer);
}



int main()
{
    win = initWin("DOOM");
    renderer = win->renderer;
    SDL_SetRelativeMouseMode(1);
    
    //init map
    initWorld();

    // init player
    player = initPlayer();

    mainLoop(win, eventFunc, loopFunc);

    return 0;
}
