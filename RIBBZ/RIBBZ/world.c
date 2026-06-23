#include "raylib.h"
#include "raymath.h"
#include "world.h"

// ==================================================
// CONFIG
// ==================================================
#define TILE_W 64
#define TILE_H 32

#define RADIUS 7   // 15x15 circle (7 tiles each direction)

// ==================================================
// TILES
// ==================================================
static Texture2D tileRed;
static Texture2D tileYellow;

// ==================================================
// MOVEMENT DIRECTION (for look-ahead)
// ==================================================
static Vector2 lastMoveDir = { 0, 0 };

// ==================================================
// ISO CONVERSION
// ==================================================
static Vector2 IsoToScreen(int x, int y)
{
    return (Vector2) {
        (x - y)* (TILE_W / 2),
            (x + y)* (TILE_H / 2)
    };
}

// ==================================================
// REQUIRED EXTERNAL HOOK (FIXES LNK2019)
// ==================================================
void SetWorldMoveDir(Vector2 dir)
{
    if (Vector2Length(dir) > 0.0001f)
    {
        Vector2 n = Vector2Normalize(dir);
        lastMoveDir = n;
    }
}

// ==================================================
// INIT WORLD
// ==================================================
void InitWorld(void)
{
    tileRed = LoadTexture("assets/tiles/red.png");
    tileYellow = LoadTexture("assets/tiles/yellow.png");

    if (tileRed.id == 0 || tileYellow.id == 0)
    {
        TraceLog(LOG_ERROR, "FAILED TO LOAD TILE TEXTURES");
    }

    SetTextureFilter(tileRed, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileYellow, TEXTURE_FILTER_POINT);
}

// ==================================================
// DRAW 15x15 CIRCLE AROUND PLAYER
// ==================================================
void DrawWorld(Vector2 playerPos)
{
    if (tileRed.id == 0 || tileYellow.id == 0) return;

    // -----------------------------------------
    // WORLD → TILE SPACE
    // -----------------------------------------
    float fx = playerPos.x / TILE_W;
    float fy = playerPos.y / TILE_H;

    int px = (int)fx;
    int py = (int)fy;

    // -----------------------------------------
    // LOOK-AHEAD (movement bias)
    // -----------------------------------------
    px += (int)(lastMoveDir.x * 1.5f);
    py += (int)(lastMoveDir.y * 1.5f);

    // -----------------------------------------
    // RENDER CIRCLE (15x15 area)
    // -----------------------------------------
    for (int y = -RADIUS; y <= RADIUS; y++)
    {
        for (int x = -RADIUS; x <= RADIUS; x++)
        {
            // circular mask (removes corners)
            if (x * x + y * y > RADIUS * RADIUS)
                continue;

            int wx = px + x;
            int wy = py + y;

            Vector2 pos = IsoToScreen(wx, wy);

            Texture2D tile =
                ((wx + wy) & 1) ? tileYellow : tileRed;

            DrawTexture(tile, (int)pos.x, (int)pos.y, WHITE);
        }
    }
}

// ==================================================
// CLEANUP
// ==================================================
void UnloadWorld(void)
{
    if (tileRed.id > 0) UnloadTexture(tileRed);
    if (tileYellow.id > 0) UnloadTexture(tileYellow);
}