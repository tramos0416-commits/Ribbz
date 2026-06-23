#pragma once

typedef enum
{
    STATE_TITLE,
    STATE_LEVEL,
    STATE_GAMEOVER
} GameState;

void SetGameState(GameState state);
GameState GetGameState(void);