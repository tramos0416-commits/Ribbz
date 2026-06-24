#define _CRT_SECURE_NO_WARNINGS

#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include <stdio.h>
#include <math.h>

#define FRAME_COUNT 24
#define DIR_COUNT 8

// -------------------------
// PLAYER STATE
// -------------------------
static Vector2 pos = { 0 };
static Vector2 vel = { 0 };

// movement
static float accel = 900.0f;
static float friction = 8.0f;
static float maxSpeed = 220.0f;
static float sprintSpeed = 360.0f;

// -------------------------
// ANIMATION
// -------------------------
static Texture2D flyFrames[DIR_COUNT][FRAME_COUNT];

static int currentFrame = 0;
static int currentDir = 4; // start SOUTH

static float animTimer = 0.0f;
static float animSpeed = 0.05f;

// -------------------------
// DIRECTIONS
// -------------------------
enum
{
    DIR_N,
    DIR_NE,
    DIR_E,
    DIR_SE,
    DIR_S,
    DIR_SW,
    DIR_W,
    DIR_NW
};

// -------------------------
// VELOCITY → DIRECTION
// -------------------------
static int GetDirection(Vector2 v)
{
    if (Vector2Length(v) < 5.0f)
        return currentDir;

        float a = atan2f(-v.y, v.x);
    a *= 180.0f / PI;

    if (a < 0) a += 360.0f;

    if (a < 22.5f || a >= 337.5f) return DIR_E;
    if (a < 67.5f)  return DIR_NE;
    if (a < 112.5f) return DIR_N;
    if (a < 157.5f) return DIR_NW;
    if (a < 202.5f) return DIR_W;
    if (a < 247.5f) return DIR_SW;
    if (a < 292.5f) return DIR_S;
    return DIR_SE;

}

// -------------------------
// INIT
// -------------------------
void InitPlayer(void)
{
    pos = (Vector2){ 0, 0 };
    vel = (Vector2){ 0, 0 };

        const char* dirs[DIR_COUNT] =
    {
        "n","ne","e","se","s","sw","w","nw"
    };

    for (int d = 0; d < DIR_COUNT; d++)
    {
        for (int i = 0; i < FRAME_COUNT; i++)
        {
            char path[256];

            sprintf(path,
                "assets/fly/%s/%04d.png",
                dirs[d],
                i
            );

            flyFrames[d][i] = LoadTexture(path);
            SetTextureFilter(flyFrames[d][i], TEXTURE_FILTER_POINT);
        }
    }

}

// -------------------------
// UPDATE
// -------------------------
void UpdatePlayer(void)
{
    float dt = GetFrameTime();

        Vector2 input = { 0 };

        if (IsKeyDown(KEY_W)) input.y -= 1;
        if (IsKeyDown(KEY_S)) input.y += 1;
        if (IsKeyDown(KEY_A)) input.x -= 1;
        if (IsKeyDown(KEY_D)) input.x += 1;

        /* Gamepad input (Gamepad 0) */
        if (IsGamepadAvailable(0))
        {
            /* D-pad (digital buttons) */
            if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP)) input.y -= 1;
            if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) input.y += 1;
            if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) input.x -= 1;
            if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) input.x += 1;

            /* Left stick (analog with deadzone) */
            const float stickDeadzone = 0.2f;
            float stickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
            float stickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);

            if (fabsf(stickX) > stickDeadzone) input.x += stickX;
            if (fabsf(stickY) > stickDeadzone) input.y += stickY;
        }

    if (Vector2Length(input) > 0)
        input = Vector2Normalize(input);

    float speedLimit = maxSpeed;
    if (IsKeyDown(KEY_LEFT_SHIFT))
        speedLimit = sprintSpeed;
    if (IsGamepadAvailable(0) && IsGamepadButtonDown(0, 4))  /* 4 = LEFT_SHOULDER */
        speedLimit = sprintSpeed;

    if (Vector2Length(input) > 0)
    {
        vel.x += input.x * accel * dt;
        vel.y += input.y * accel * dt;
    }
    else
    {
        float decay = 1.0f - friction * dt;
        if (decay < 0) decay = 0;

        vel.x *= decay;
        vel.y *= decay;
    }

    float speed = Vector2Length(vel);

    if (speed > speedLimit)
        vel = Vector2Scale(Vector2Normalize(vel), speedLimit);

    pos.x += vel.x * dt;
    pos.y += vel.y * dt;

    // direction follows momentum
    currentDir = GetDirection(vel);

    // animation
    animTimer += dt;

    if (animTimer >= animSpeed)
    {
        animTimer = 0.0f;
        currentFrame++;

        if (currentFrame >= FRAME_COUNT)
            currentFrame = 0;
    }

}

// -------------------------
// DRAW
// -------------------------
void DrawPlayer(void)
{
    Texture2D frame = flyFrames[currentDir][currentFrame];

        DrawTexture(
            frame,
            (int)(pos.x - frame.width / 2),
            (int)(pos.y - frame.height),
            WHITE
        );

}

Vector2 GetPlayerPos(void)
{
    return pos;
}

void UnloadPlayer(void)
{
    for (int d = 0; d < DIR_COUNT; d++)
    {
        for (int i = 0; i < FRAME_COUNT; i++)
        {
            if (flyFrames[d][i].id > 0)
                UnloadTexture(flyFrames[d][i]);
        }
    }
}