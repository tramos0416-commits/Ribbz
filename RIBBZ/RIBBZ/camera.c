#include "raylib.h"
#include "camera.h"

static Camera2D cam;

// higher = snappier camera
static float smoothness = 6.0f;

void CameraInit(int screenW, int screenH)
{
    cam.offset = (Vector2){ screenW / 2.0f, screenH / 2.0f };
    cam.target = (Vector2){ 0, 0 };
    cam.rotation = 0.0f;
    cam.zoom = 1.0f;
}

void CameraUpdate(Vector2 target)
{
    float dt = GetFrameTime();

    // Smooth follow (lerp)
    cam.target.x += (target.x - cam.target.x) * smoothness * dt;
    cam.target.y += (target.y - cam.target.y) * smoothness * dt;
}

Camera2D CameraGet(void)
{
    return cam;
}