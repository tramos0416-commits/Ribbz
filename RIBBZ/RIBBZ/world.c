#include "raylib.h"
#include "world.h"

#define TILE_W 64
#define TILE_H 32

#define MAP_W 31
#define MAP_H 30

static Texture2D tileRed;
static Texture2D tileYellow;

static Texture2D tileCornerCliff;
static Texture2D tileLeftCliff;
static Texture2D tileRightCliff;

// ==================================================
// Legend for making the tiles and stuff
//
//0 = empty
//1 = red
//2 = yellow
//3 = cornerCliff
//4 = leftCliff
//5 = rightCliff
// ==================================================

static int map[MAP_H][MAP_W] =
{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,2,1,2,1,2,1,2,1,2},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,5,2,1,2,1,2,1,2,1,2,1},
    {1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,5,1,2,1,2,1,2,1,2,1,2},
    {1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1,5,2,1,2,1,2,1,2,1,2,1},
    {1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,5,1,2,1,2,1,2,1,2,1,2},
    {1,2,1,2,1,2,2,2,2,2,2,2,2,2,2,1,2,1,2,1,5,2,1,2,1,2,1,2,1,2,1},
    {1,2,1,2,1,2,1,1,1,1,1,1,1,1,2,1,2,1,2,1,5,1,2,1,2,1,2,1,2,1,2},
    {1,2,1,2,1,2,1,2,2,2,2,2,2,1,2,1,2,1,2,1,5,2,1,2,1,2,1,2,1,2,1},
    {1,2,1,2,1,2,1,2,1,1,1,1,2,1,2,1,2,1,2,1,5,1,2,1,2,1,2,1,2,1,2},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,2,1,2,1,2,1,2,1,2,1},

    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},

    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3},
    {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3}

};

static Vector2 IsoToScreen(int x, int y)
{
    return (Vector2) {
        (float)((x - y) * (TILE_W / 2)),
            (float)((x + y) * (TILE_H / 2))
    };
}

void InitWorld(void)
{
    tileRed = LoadTexture("assets/tiles/red.png");
    tileYellow = LoadTexture("assets/tiles/yellow.png");

    tileCornerCliff = LoadTexture("assets/tiles/cornerCliff.png");
    tileLeftCliff = LoadTexture("assets/tiles/leftCliff.png");
    tileRightCliff = LoadTexture("assets/tiles/rightCliff.png");

    SetTextureFilter(tileRed, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileYellow, TEXTURE_FILTER_POINT);

    SetTextureFilter(tileCornerCliff, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileLeftCliff, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileRightCliff, TEXTURE_FILTER_POINT);
}

void SetWorldMoveDir(Vector2 dir)
{
    // unused for now
    (void)dir;
}

void DrawWorld(Vector2 playerPos)
{
    (void)playerPos;

    for (int y = 0; y < MAP_H; y++)
    {
        for (int x = 0; x < MAP_W; x++)
        {
            if (map[y][x] == 0)
                continue;

            Vector2 pos = IsoToScreen(x, y);

            Texture2D* tile = &tileRed;

            switch (map[y][x])
            {
            case 0:
                continue; // skip empty tiles

            case 1:
                tile = &tileRed;
                break;

            case 2:
                tile = &tileYellow;
                break;

            case 3:
                tile = &tileCornerCliff;
                break;

            case 4:
                tile = &tileLeftCliff;
                break;

            case 5:
                tile = &tileRightCliff;
                break;

            default:
                tile = &tileRed;
                break;
            }

            DrawTexture(
                *tile,
                (int)pos.x,
                (int)pos.y,
                WHITE
            );
        }
    }
}

void UnloadWorld(void)
{
    if (tileRed.id)
        UnloadTexture(tileRed);

    if (tileYellow.id)
        UnloadTexture(tileYellow);

    if (tileCornerCliff.id)
        UnloadTexture(tileCornerCliff);

    if (tileLeftCliff.id)
        UnloadTexture(tileLeftCliff);

    if (tileRightCliff.id)
        UnloadTexture(tileRightCliff);
}