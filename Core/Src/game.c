#include "game.h"

uint32_t _GetSeed() {
    uint32_t seed;
    volatile uint8_t *ram = (uint8_t *)&seed;
    for (int i = 0; i < 4; i++)
    {
        ram[i] = *(volatile uint8_t*)(0x20000000 + i);
    }
    return seed;
}

void Game_Init(Game_Ctx *Ctx)
{
    memset(Ctx->Board, 0, sizeof(Ctx->Board));
    Ctx->Score = 0;
    Ctx->Steps = 0;
    Ctx->CurrentSpace = 16;
    srand(_GetSeed());
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
            Ctx->Score += arr[i];
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
    Ctx->Steps ++;
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
            OLED_ShowGameNum(i + 1, j + 1, Ctx->Board[i][j]);
        }
    }
    for (int i = 0; i < 4; i++)
    {
        OLED_ShowChar(i + 1, 9, '|');
    }
    OLED_ShowString(1, 10, "Score:");
    OLED_ShowNum(2, 13, Ctx->Score, 3);
    OLED_ShowString(3, 10, "Steps:");
    OLED_ShowNum(4, 13, Ctx->Steps, 3);
}
