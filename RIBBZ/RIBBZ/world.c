#include "raylib.h"
#include "world.h"

#define TILE_W 64
#define TILE_H 32

#define MAP_W 43
#define MAP_H 30

static Texture2D tileLightGrass;
static Texture2D tileDarkGrass;

static Texture2D tileCornerCliff;
static Texture2D tileLeftCliff;
static Texture2D tileRightCliff;
static Texture2D tileCreaseCliff;
static Texture2D tileLeftCliffCorner;
static Texture2D tileRightCliffCorner;

// ==================================================
// Legend for making the tiles and stuff
//
//0 = empty
//1 = lightGrass
//2 = darkGrass
//3 = cornerCliff
//4 = leftCliff
//5 = rightCliff
//6 = creaseCliff
//7 = leftCliffCorner
//8 = rightCliffCorner
// ==================================================

static int map[MAP_H][MAP_W] =
{
    {9,9,9,9,9,9,9,9,9,9,1,1,1,1,1,1,1,1,1,1,8,0,2,1,2,1,2,1,2,8},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,5,8,1,2,1,2,1,2,1,2,1,8},
    {1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,5,8},
    {1,2,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1,6,4,1,2,1,2,1,2,1,2,1,5,5,8},
    {1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,5,5,5,8},
    {1,2,1,2,1,2,2,2,2,2,2,2,2,2,2,1,2,1,2,1,1,2,1,2,1,2,1,2,1,2,1,5,5,5,5,8},
    {1,2,1,2,1,2,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,5,5,5,5,5,8},
    {1,2,1,2,1,2,1,2,2,2,2,2,2,1,2,1,2,1,2,1,6,4,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,8},
    {1,2,1,2,1,2,1,2,1,1,1,1,2,1,2,1,2,1,2,1,5,6,2,1,2,1,2,1,2,1,2,5,5,5,5,5,5,5,8},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,6,4,4,4,4,4,4,4,4,4,3,5,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,6,4,4,4,4,4,4,4,4,4,3,5,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,6,4,4,4,4,4,4,4,4,4,3,5,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,6,4,4,4,4,4,4,4,4,4,3,5,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,6,4,4,4,4,4,4,4,4,4,3,5,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,6,4,4,4,4,4,4,4,4,4,3,5},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,6,4,4,4,4,4,4,4,4,4,3},
    {1,2,1,2,1,2,1,2,1,2,2,1,2,1,2,1,2,1,2,1,5,5,5,5,5,5,5,5,5},

    {7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,5,5,5,5,5,5,5,5},
    {0,7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,5,5,5,5,5,5,5},
    {0,0,7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,5,5,5,5,5,5},
    {0,0,0,7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,5,5,5,5,5},
    {0,0,0,0,7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,5,5,5,5},
    {0,0,0,0,0,7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,5,5,5},
    {0,0,0,0,0,0,7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,5,5},
    {0,0,0,0,0,0,0,7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3,5},
    {0,0,0,0,0,0,0,0,7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3}

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
    tileLightGrass = LoadTexture("assets/tiles/lightGrass.png");
    tileDarkGrass = LoadTexture("assets/tiles/darkGrass.png");

    tileCornerCliff = LoadTexture("assets/tiles/cornerCliff.png");
    tileLeftCliff = LoadTexture("assets/tiles/leftCliff.png");
    tileRightCliff = LoadTexture("assets/tiles/rightCliff.png");
    tileCreaseCliff = LoadTexture("assets/tiles/creaseCliff.png");
    tileLeftCliffCorner = LoadTexture("assets/tiles/leftCliffCorner.png");
    tileRightCliffCorner = LoadTexture("assets/tiles/rightCliffCorner.png");

    SetTextureFilter(tileLightGrass, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileDarkGrass, TEXTURE_FILTER_POINT);

    SetTextureFilter(tileCornerCliff, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileLeftCliff, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileRightCliff, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileCreaseCliff, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileLeftCliffCorner, TEXTURE_FILTER_POINT);
    SetTextureFilter(tileRightCliffCorner, TEXTURE_FILTER_POINT);
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

            Texture2D* tile = &tileLightGrass;

            switch (map[y][x])
            {
            case 0:
                continue; // skip empty tiles

            case 1:
                tile = &tileLightGrass;
                break;

            case 2:
                tile = &tileDarkGrass;
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

            case 6:
                tile = &tileCreaseCliff;
                break;

            case 7:
                tile = &tileLeftCliffCorner;
                break;

            case 8:
                tile = &tileRightCliffCorner;
                break;

            default:
                tile = &tileLightGrass;
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
    if (tileLightGrass.id)
        UnloadTexture(tileLightGrass);

    if (tileDarkGrass.id)
        UnloadTexture(tileDarkGrass);

    if (tileCornerCliff.id)
        UnloadTexture(tileCornerCliff);

    if (tileLeftCliff.id)
        UnloadTexture(tileLeftCliff);

    if (tileRightCliff.id)
        UnloadTexture(tileRightCliff);

    if (tileRightCliff.id)
        UnloadTexture(tileCreaseCliff);
}