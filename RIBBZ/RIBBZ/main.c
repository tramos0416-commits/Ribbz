#include "raylib.h"
#include "world.h"
#include "player.h"
#include "camera.h"

int main(void)
{
    // TRUE INTERNAL RESOLUTION
    const int gameW = 320;
    const int gameH = 240;

    // WINDOW SCALE
    const int scale = 5;

    InitWindow(
        gameW * scale,
        gameH * scale,
        "RIBBZ ENGINE - ALPHA version 1"
    );

    SetTargetFPS(24);

    InitWorld();
    InitPlayer();
    CameraInit(gameW, gameH);

    // SKY
    Texture2D sky =
        LoadTexture("assets/sky.png");

    SetTextureFilter(
        sky,
        TEXTURE_FILTER_POINT
    );

    // LOW RES BUFFER
    RenderTexture2D target =
        LoadRenderTexture(gameW, gameH);

    while (!WindowShouldClose())
    {
        UpdatePlayer();
        CameraUpdate(GetPlayerPos());

        // -----------------------------
        // DRAW TO LOW RES BUFFER
        // -----------------------------
        BeginTextureMode(target);

        ClearBackground(BLACK);

        // -----------------------------
        // SKY (BACKGROUND)
        // -----------------------------
        DrawTexturePro(
            sky,
            (Rectangle)
        {
            0,
                0,
                (float)sky.width,
                (float)sky.height
        },
            (Rectangle)
        {
            0,
                0,
                (float)gameW,
                (float)gameH
        },
            (Vector2) {
            0, 0
        },
            0.0f,
            WHITE
        );

        // -----------------------------
        // WORLD
        // -----------------------------
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
            (Rectangle)
        {
            0,
                0,
                (float)gameW,
                (float)-gameH
        },
            (Rectangle)
        {
            0,
                0,
                (float)(gameW * scale),
                (float)(gameH * scale)
        },
            (Vector2) {
            0, 0
        },
            0.0f,
            WHITE
        );

        EndDrawing();
    }

    UnloadTexture(sky);

    UnloadRenderTexture(target);

    UnloadWorld();
    UnloadPlayer();

    CloseWindow();

    return 0;
}