#pragma once

#include "raylib.h"

void CameraInit(int screenW, int screenH);
void CameraUpdate(Vector2 target);
Camera2D CameraGet(void);