#include <stdio.h>

#include "nob.h"
#include <raylib.h>
#include <raymath.h>

typedef struct {
    int key;
    Vector2 vec;
} Control;

static Control controls[] = {
    {KEY_S, {0, 1}},
    {KEY_W, {0, -1}},
    {KEY_A, {-1, 0}},
    {KEY_D, {1, 0}},
};

#define PLAYER_SPEED 1000
#define REAL_FPS 24
#define TARGET_FPS 480
#define TARGET_DT (1.0/TARGET_FPS)
#define GRAVITY 1000.0f
#define COLLISION_DAMPING 0.8

typedef struct {
    Vector2 *items;
    size_t count;
    size_t capacity;
} Vector2s;

typedef struct {
    Color *items;
    size_t count;
    size_t capacity;
} Colors;

int main()
{
    InitWindow(800, 600, "Subframes");
    SetTargetFPS(REAL_FPS);
    Vector2s positions = {0};
    Vector2s velocities = {0};
    Colors colors = {0};
    Vector2 prev_winpos = GetWindowPosition();
    while (!WindowShouldClose()) {
        Vector2 winpos = GetWindowPosition();
        Vector2 dwinpos = Vector2Subtract(winpos, prev_winpos);
        float w = GetScreenWidth();
        float h = GetScreenHeight();
        float real_dt = GetFrameTime();
        float radius = 20.0f;

        BeginDrawing();
            ClearBackground(GetColor(0x181818FF));

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                nob_da_append(&positions, GetMousePosition());
                nob_da_append(&velocities, ((Vector2) {500, 500}));
                nob_da_append(&colors, ColorFromHSV(GetRandomValue(0, 360), 0.5, 0.8));
            }

            for (float t = 0.0; t < real_dt; t += TARGET_DT) {
                for (size_t i = 0; i < positions.count; ++i) {
                    float f = t/real_dt;

                    positions.items[i] = Vector2Subtract(positions.items[i], Vector2Scale(dwinpos, TARGET_DT/real_dt));
                    velocities.items[i].y += GRAVITY*TARGET_DT;

                    float nx = positions.items[i].x + velocities.items[i].x*TARGET_DT;
                    if (nx - radius <= 0) {
                        positions.items[i].x = radius;
                        velocities.items[i].x *= -COLLISION_DAMPING;
                    } else if (nx + radius >= w) {
                        positions.items[i].x = w - radius;
                        velocities.items[i].x *= -COLLISION_DAMPING;
                    } else {
                        positions.items[i].x = nx;
                    }

                    float ny = positions.items[i].y + velocities.items[i].y*TARGET_DT;
                    if (ny - radius <= 0) {
                        positions.items[i].y = radius;
                        velocities.items[i].y *= -COLLISION_DAMPING;
                    } else if (ny + radius >= h) {
                        positions.items[i].y = h - radius;
                        velocities.items[i].y *= -COLLISION_DAMPING;
                    } else {
                        positions.items[i].y = ny;
                    }

                    DrawRing(positions.items[i], radius*0.8, radius, 0, 360, 100, ColorAlpha(colors.items[i], f));
                }
            }

            DrawText(TextFormat("%f %f", dwinpos.x, dwinpos.y), 0, 0, 32, WHITE);
        EndDrawing();

        prev_winpos = winpos;
    }
    CloseWindow();
    return 0;
}
