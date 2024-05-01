#include "raylib.h"
const int screenWidth = 1024;
const int screenHeight = 768;

static bool startMenu = true;

static int clickCount = 0;
static int gameStage = 1;
static int win = 0;
static int winner = 0;

typedef struct menuBtn
{
    Texture2D img;
    Vector2 position;
    bool clicked;
} menuBtn;
static menuBtn startBtnImg, exitBtnImg;

typedef struct item
{
    Texture2D img;
    Vector2 position;
} item;
static item cross[3], zero[3], tempItem, crossWonImg,zeroWonImg;

static Texture2D mainBackgroundImg, mainBoardImg, startMenuImg;

typedef struct box
{
    Vector2 position;
    bool clicked;
    Vector2 startClickPosition;
    Vector2 endClickPosition;
    int containedItem;
} box;
static box boxNum[3][3];

static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UnloadGame(void);

static void gameMenu();
static void mainGameWindow();
static void winCondition();

int main(void)
{

    InitWindow(screenWidth, screenHeight, "Movable Tic Tac Toe");
    InitGame();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateGame();
        DrawGame();
    }
    UnloadGame();
    CloseWindow();
}

void InitGame(void)
{
    startMenuImg = LoadTexture("images/startMenuImg.png");
    mainBackgroundImg = LoadTexture("images/jungleThemeBackgroundImg.png");
    mainBoardImg = LoadTexture("images/mainBoard.png");
    crossWonImg.img = LoadTexture("images/crossWon.png");
    zeroWonImg.img = LoadTexture("images/zeroWon.png");

    for (int k = 0; k <= 2; k++)
    {
        cross[k].img = LoadTexture("images/crossImg.png");
        zero[k].img = LoadTexture("images/zeroImg.png");
    }

    startBtnImg.img = LoadTexture("images/startBtnImg.png");
    exitBtnImg.img = LoadTexture("images/exitBtnImg.png");

    crossWonImg.position.x = screenWidth;
    crossWonImg.position.y = screenHeight;
    zeroWonImg.position.x = screenWidth;
    zeroWonImg.position.y = screenHeight;

    startBtnImg.position.x = 450;
    startBtnImg.position.y = 40;
    startBtnImg.clicked = false;
    static int crossInitialPosition[3][2] = {
        {860, 250}, {855, 330}, {865, 390}};
    static int zeroInitialPosition[3][2] = {
        {120, 570}, {20, 550}, {60, 600}};

    for (int k = 0; k <= 2; k++)
    {
        cross[k].position.x = crossInitialPosition[k][0];
        cross[k].position.y = crossInitialPosition[k][1];
        zero[k].position.x = zeroInitialPosition[k][0];
        zero[k].position.y = zeroInitialPosition[k][1];
    }

    static int itemHoldingBoxPosition[9][2] = {
        {310, 222}, {475, 222}, {630, 222}, {310, 365}, {475, 365}, {630, 373}, {310, 510}, {475, 515}, {630, 530}};

    for (int k = 0; k <= 2; k++)
    {
        for (int i = 0; i <= 2; i++)
        {
            boxNum[k][i].position.x = itemHoldingBoxPosition[k * 3 + i][0];
            boxNum[k][i].position.y = itemHoldingBoxPosition[k * 3 + i][1];
            boxNum[k][i].containedItem = 0;
        }
    }

    static int clickableBoxStartPosition[9][2] = {
        {280, 230}, {450, 225}, {620, 225}, {305, 365}, {445, 370}, {625, 370}, {300, 510}, {450, 520}, {620, 530}};
    static int clickableBoxEndPosition[9][2] = {
        {420, 340}, {600, 335}, {730, 335}, {420, 475}, {595, 490}, {730, 500}, {420, 620}, {600, 625}, {730, 630}};
    for (int k = 0; k <= 2; k++)
    {
        for (int i = 0; i <= 2; i++)
        {
            boxNum[k][i].startClickPosition.x = clickableBoxStartPosition[k * 3 + i][0];
            boxNum[k][i].startClickPosition.y = clickableBoxStartPosition[k * 3 + i][1];

            boxNum[k][i].endClickPosition.x = clickableBoxEndPosition[k * 3 + i][0];
            boxNum[k][i].endClickPosition.y = clickableBoxEndPosition[k * 3 + i][1];
        }
    }
}

void UpdateGame(void)
{
    if (startMenu)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 clickPoint = GetMousePosition();
            if (clickPoint.x >= 480 && clickPoint.x <= 710 && clickPoint.y >= 250 && clickPoint.y <= 370)
            {
                startBtnImg.clicked = true;
            }
            if (startBtnImg.clicked)
            {
                startMenu = !startMenu;
            }
        }
    }
    else
    {
        if (!win)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 clickPoint = GetMousePosition();
                for (int k = 0; k <= 2; k++)
                {
                    for (int i = 0; i <= 2; i++)
                    {
                        if (clickPoint.x >= boxNum[k][i].startClickPosition.x && clickPoint.y >= boxNum[k][i].startClickPosition.y && clickPoint.x <= boxNum[k][i].endClickPosition.x && clickPoint.y <= boxNum[k][i].endClickPosition.y)
                        {
                            if (clickCount < 6)
                            {
                                gameStage = 1;
                            }
                            else
                            {
                                gameStage = 2;
                            }
                            if (gameStage == 1)
                            {
                                if (boxNum[k][i].containedItem == 0)
                                {
                                    clickCount += 1;
                                    if (clickCount % 2 != 0)
                                    {
                                        boxNum[k][i].containedItem = 1;
                                        cross[clickCount / 2].position.x = boxNum[k][i].position.x;
                                        cross[clickCount / 2].position.y = boxNum[k][i].position.y;
                                    }
                                    else
                                    {
                                        boxNum[k][i].containedItem = 2;
                                        zero[clickCount / 2 - 1].position.x = boxNum[k][i].position.x;
                                        zero[clickCount / 2 - 1].position.y = boxNum[k][i].position.y;
                                    }
                                    winCondition();
                                }
                            }
                            if (gameStage == 2)
                            {
                                if ((clickCount + 1 - 7) % 4 == 0)
                                {
                                    if (boxNum[k][i].containedItem == 1)
                                    {
                                        for (int l = 0; l <= 2; l++)
                                        {
                                            if (boxNum[k][i].position.x == cross[l].position.x && boxNum[k][i].position.y == cross[l].position.y)
                                            {
                                                tempItem.position.x = boxNum[k][i].position.x;
                                                tempItem.position.y = boxNum[k][i].position.y;
                                                boxNum[k][i].containedItem = 0;
                                                break;
                                            }
                                        }
                                        clickCount++;
                                    }
                                }
                                else if ((clickCount + 1 - 7) % 4 == 2)
                                {
                                    if (boxNum[k][i].containedItem == 2)
                                    {
                                        for (int l = 0; l <= 2; l++)
                                        {
                                            if (boxNum[k][i].position.x == zero[l].position.x && boxNum[k][i].position.y == zero[l].position.y)
                                            {
                                                tempItem.position.x = boxNum[k][i].position.x;
                                                tempItem.position.y = boxNum[k][i].position.y;
                                                boxNum[k][i].containedItem = 0;
                                                break;
                                            }
                                        }
                                        clickCount++;
                                    }
                                }
                                else if ((clickCount + 1 - 7) % 4 == 1)
                                {
                                    if (boxNum[k][i].containedItem == 0)
                                    {
                                        if (boxNum[k][i].position.x != tempItem.position.x || boxNum[k][i].position.y != tempItem.position.y)
                                        {
                                            for (int l = 0; l <= 2; l++)
                                            {
                                                if (tempItem.position.x == cross[l].position.x && tempItem.position.y == cross[l].position.y)
                                                {
                                                    cross[l].position.x = boxNum[k][i].position.x;
                                                    cross[l].position.y = boxNum[k][i].position.y;
                                                    boxNum[k][i].containedItem = 1;
                                                    break;
                                                }
                                            }
                                            winCondition();
                                            clickCount++;
                                        }
                                    }
                                }
                                else
                                {
                                    if (boxNum[k][i].containedItem == 0)
                                    {
                                        if (boxNum[k][i].position.x != tempItem.position.x || boxNum[k][i].position.y != tempItem.position.y)
                                        {
                                            for (int l = 0; l <= 2; l++)
                                            {
                                                if (tempItem.position.x == zero[l].position.x && tempItem.position.y == zero[l].position.y)
                                                {
                                                    zero[l].position.x = boxNum[k][i].position.x;
                                                    zero[l].position.y = boxNum[k][i].position.y;
                                                    boxNum[k][i].containedItem = 2;
                                                    break;
                                                }
                                            }
                                            winCondition();
                                            clickCount++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void DrawGame(void)
{
    BeginDrawing();
    startMenu ? gameMenu() : mainGameWindow();
    EndDrawing();
}

void UnloadGame(void)
{
    UnloadTexture(exitBtnImg.img);
    UnloadTexture(startBtnImg.img);
    for (int k = 0; k <= 2; k++)
    {
        UnloadTexture(cross[k].img);
        UnloadTexture(zero[k].img);
    }
    UnloadTexture(mainBackgroundImg);
    UnloadTexture(mainBoardImg);
    UnloadTexture(startMenuImg);
    UnloadTexture(crossWonImg.img);
    UnloadTexture(zeroWonImg.img);
}

void gameMenu()
{

    DrawTexture(startMenuImg, 0, 0, WHITE);
    DrawTexture(startBtnImg.img, startBtnImg.position.x, startBtnImg.position.y, WHITE);
    DrawTexture(exitBtnImg.img, 0, 500, WHITE);
    // DrawRectangle(480,250,230,120,WHITE);
}

void mainGameWindow()
{
    ClearBackground(WHITE);
    DrawTexture(mainBackgroundImg, 0, 0, WHITE);
    DrawTexture(mainBoardImg, 250, 150, WHITE);
    DrawTexture(crossWonImg.img, crossWonImg.position.x, crossWonImg.position.y, WHITE);
    DrawTexture(zeroWonImg.img, zeroWonImg.position.x, zeroWonImg.position.y, WHITE);

    for (int k = 0; k <= 2; k++)
    {
        DrawTexture(cross[k].img, cross[k].position.x, cross[k].position.y, WHITE);
        DrawTexture(zero[k].img, zero[k].position.x, zero[k].position.y, WHITE);
    }

    if (win)
    {
        if (winner == 1)
        {
            crossWonImg.position.x = 300;
            crossWonImg.position.y = 585;
        }
        else if (winner == 2)
        {
            zeroWonImg.position.x = 300;
            zeroWonImg.position.y = 585;
        }
    }
}

void winCondition()
{

    for (int k = 0; k <= 2; k++)
    {
        if (boxNum[k][0].containedItem == boxNum[k][1].containedItem && boxNum[k][1].containedItem == boxNum[k][2].containedItem)
        {
            if (boxNum[k][0].containedItem == 0)
                continue;
            win = 1;
            winner = boxNum[k][0].containedItem;
            goto end;
        }
        if (boxNum[0][k].containedItem == boxNum[1][k].containedItem && boxNum[1][k].containedItem == boxNum[2][k].containedItem)
        {
            if (boxNum[0][k].containedItem == 0)
                continue;
            win = 1;
            winner = boxNum[0][k].containedItem;
            goto end;
        }
    }
    if (boxNum[0][0].containedItem == boxNum[1][1].containedItem && boxNum[1][1].containedItem == boxNum[2][2].containedItem)
    {
        win = 1;
        if (boxNum[0][0].containedItem == 0)
            win = 0;
        winner = boxNum[0][0].containedItem;
    }
    if (boxNum[0][2].containedItem == boxNum[1][1].containedItem && boxNum[1][1].containedItem == boxNum[2][0].containedItem)
    {
        win = 1;
        if (boxNum[0][2].containedItem == 0)
            win = 0;
        winner = boxNum[0][2].containedItem;
    }
end:
}