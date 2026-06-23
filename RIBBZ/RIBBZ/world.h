#pragma once
#include "raylib.h"

void InitWorld(void);
void DrawWorld(Vector2 playerPos);
void UnloadWorld(void);

void SetWorldMoveDir(Vector2 dir);