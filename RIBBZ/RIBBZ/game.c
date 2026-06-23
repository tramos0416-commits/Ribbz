#include "game.h"

static GameState currentState = STATE_TITLE;

void SetGameState(GameState state)
{
    currentState = state;
}

GameState GetGameState(void)
{
    return currentState;
}