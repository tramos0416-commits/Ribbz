#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "world.h"

static Vector2 pos = { 0 };
static Vector2 vel = { 0 };

// --------------------------------------------------
// SETTINGS (tweakable)
// --------------------------------------------------
static float accel = 900.0f;
static float friction = 8.0f;
static float maxSpeed = 220.0f;
static float sprintSpeed = 360.0f;

// --------------------------------------------------
// INIT
// --------------------------------------------------
void InitPlayer(void)
{
    pos = (Vector2){ 0, 0 };
    vel = (Vector2){ 0, 0 };
}

// --------------------------------------------------
// UPDATE
// --------------------------------------------------
void UpdatePlayer(void)
{
    float dt = GetFrameTime();

    Vector2 input = { 0 };

    // movement input
    if (IsKeyDown(KEY_W)) input.y -= 1;
    if (IsKeyDown(KEY_S)) input.y += 1;
    if (IsKeyDown(KEY_A)) input.x -= 1;
    if (IsKeyDown(KEY_D)) input.x += 1;

    // normalize diagonal movement
    if (Vector2Length(input) > 0)
        input = Vector2Normalize(input);

    // --------------------------------------------------
    // SEND DIRECTION TO WORLD (for look-ahead streaming)
    // --------------------------------------------------
    SetWorldMoveDir(input);

    // --------------------------------------------------
    // SPRINT
    // --------------------------------------------------
    float speedLimit = maxSpeed;
    if (IsKeyDown(KEY_LEFT_SHIFT))
        speedLimit = sprintSpeed;

    // --------------------------------------------------
    // ACCELERATION
    // --------------------------------------------------
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

    // clamp speed
    float speed = Vector2Length(vel);
    if (speed > speedLimit)
    {
        vel = Vector2Scale(Vector2Normalize(vel), speedLimit);
    }

    // apply movement
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
}

// --------------------------------------------------
// DRAW PLAYER
// --------------------------------------------------
void DrawPlayer(void)
{
    DrawCircleV(pos, 10, GREEN);
}

// --------------------------------------------------
// GET POSITION (for camera/world)
// --------------------------------------------------
Vector2 GetPlayerPos(void)
{
    return pos;
}