#ifndef GAME_H
#define GAME_H
#ifdef __cplusplus
extern "C" {
#endif
#include <string.h>
#include <stdlib.h>

#include "gyro.h"
#include "oled.h"

typedef struct
{
    int Board[4][4];
    int Score;
    int CurrentSpace;
    int Steps;
} Game_Ctx;

void Game_Init(Game_Ctx *Ctx);

void Game_GenerateNum(Game_Ctx *Ctx);

void Game_Move(Game_Ctx *Ctx, int Dir);

void Game_Display(Game_Ctx *Ctx);

#ifdef __cplusplus
}
#endif
#endif // GAME_H