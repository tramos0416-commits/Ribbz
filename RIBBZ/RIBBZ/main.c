#include "raylib.h"
#include "world.h"
#include "player.h"
#include "camera.h"

int main(void)
{
    // TRUE INTERNAL RESOLUTION
    const int gameW = 1440;
    const int gameH = 960;

    // WINDOW SCALE (4x)
    const int scale = 1;

    InitWindow(gameW * scale, gameH * scale, "RIBBZ ENGINE - ALPHA version 1");

    SetTargetFPS(30);

    InitWorld();
    InitPlayer();
    CameraInit(gameW, gameH);

    // 🔥 THIS IS THE KEY PART
    RenderTexture2D target = LoadRenderTexture(gameW, gameH);

    while (!WindowShouldClose())
    {
        UpdatePlayer();
        CameraUpdate(GetPlayerPos());

        // -----------------------------
        // DRAW TO LOW RES BUFFER
        // -----------------------------
        BeginTextureMode(target);

        ClearBackground(DARKGRAY);

        BeginMode2D(CameraGet());

        DrawWorld(GetPlayerPos());
        DrawPlayer();

        EndMode2D();

        EndTextureMode();

        // -----------------------------
        // UPSCALE TO WINDOW
        // -----------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            target.texture,
            (Rectangle) {
            0, 0, gameW, -gameH
        }, // flipped Y fix
            (Rectangle) {
            0, 0, gameW* scale, gameH* scale
        },
            (Vector2) {
            0, 0
        },
            0.0f,
            WHITE
        );

        EndDrawing();
    }

    UnloadRenderTexture(target);

    UnloadWorld();
    CloseWindow();

    return 0;
}