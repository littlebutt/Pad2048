#include "game.h"

void Game_Init(Game_Ctx *Ctx)
{
    memset(Ctx->Board, 0, sizeof(Ctx->Board));
    Ctx->Score = 0;
    Ctx->CurrentSpace = 16;
    srand(65535);
}

int _Game_GetNum(Game_Ctx *Ctx)
{
    int target = rand() % 16;
    if (Ctx->Score > 512)
    {
        if (target > 12)
        {
            return 8;
        }
        else if (target > 6)
        {
            return 4;
        }
        else
        {
            return 2;
        }
        
    }
    else if (Ctx->Score > 128)
    {
        if (target > 8)
        {
            return 4;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        return 2;
    }
}

void Game_GenerateNum(Game_Ctx *Ctx)
{
    int target = rand() % Ctx->CurrentSpace;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (Ctx->Board[i][j] == 0)
            {
                if (target == 0)
                {
                    Ctx->Board[i][j] = _Game_GetNum(Ctx);
                    Ctx->CurrentSpace --;
                    return;
                }
                target --;
            }
        }
    }
}

void _Game_DoMove(Game_Ctx *Ctx, int arr[4])
{
    int i = 1, j = 0;
    while (i < 4) {
        if (arr[i] == 0 || i == j) {
            i++;
        } else if (arr[i] == arr[j]) {
            arr[j] += arr[i];
            Ctx->CurrentSpace ++;
            arr[i] = 0;
            i++;
            j++;
        } else if (arr[j] == 0) {
            arr[j] = arr[i];
            arr[i] = 0;
            i++;
        } else {
            j++;
        }
    }
}

void Game_Move(Game_Ctx *Ctx, int Dir)
{
    int strap[4][4];
    if (Dir == LEFT)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                strap[i][j] = Ctx->Board[i][j];
            }
        }
        for (int i = 0; i < 4; i++)
        {
            _Game_DoMove(Ctx, strap[i]);
        }
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Ctx->Board[i][j] = strap[i][j];
            }
        }
    }
    else if (Dir == RIGHT)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                strap[i][j] = Ctx->Board[i][3 - j];
            }
        }
        for (int i = 0; i < 4; i++)
        {
            _Game_DoMove(Ctx, strap[i]);
        }
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                Ctx->Board[i][3 - j] = strap[i][j];
            }
        }

    }
    else if (Dir == UP)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                strap[j][i] = Ctx->Board[i][j]; 
            }
        }
        for (int i = 0; i < 4; i++)
        {
           _Game_DoMove(Ctx, strap[i]);
        }
        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                Ctx->Board[i][j] = strap[j][i]; 
            }
        }
    }
    else if (Dir == DOWN)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                strap[j][i] = Ctx->Board[3 - i][j]; 
            }
        }
        for (int i = 0; i < 4; i++)
        {
            _Game_DoMove(Ctx, strap[i]);
        }
        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                Ctx->Board[3 - i][j] = strap[j][i]; 
            }
        }
    }
}

void Game_Display(Game_Ctx *Ctx)
{
    OLED_Clear();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (Ctx->Board[i][j] == 0)
            {
                continue;
            }
            OLED_ShowNum(i + 1, 2 * j + 1, Ctx->Board[i][j], Ctx->Board[i][j] > 9 ? 2 : 1);
        }
    }
    for (int i = 0; i < 4; i++)
    {
        OLED_ShowChar(i + 1, 9, '|');
    }
}
