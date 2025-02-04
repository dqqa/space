#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 900
#define WINDOW_TITLE "space"

#define TARGET_FPS 60

#define SPEED_FACTOR 0.2f

#define STAR_LAYER_SPEED_FACTOR 0.5f
#define STAR_SIZE 2.0f
#define STAR_LAYERS 3
#define STARS_PER_LAYER 50
#define STAR_COLOR WHITE

#define SPACESHIP_STICKINESS_FACTOR 0.04f

#ifndef __cplusplus
#define BACKGROUND_COLOR ((Color){ 0x18, 0x18, 0x18, 0xFF })
#else
#define BACKGROUND_COLOR (Color{ 0x18, 0x18, 0x18, 0xFF })
#endif

#define ARR_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef struct
{
    Vector2 size;
    Vector2 pos;
} star_t;

static star_t stars[STAR_LAYERS][STARS_PER_LAYER];

void draw_spaceship(const Vector2 *center, const Vector2 *mouse)
{
    Vector2 delta = Vector2Scale(Vector2Subtract(*mouse, *center), SPACESHIP_STICKINESS_FACTOR);
    Vector2 actual_pos = Vector2Add(*center, delta);
    DrawRectangleV(actual_pos, (Vector2){ 20, 20 }, RED);
}

void draw_stars(void)
{
    for (size_t layer = 0; layer < ARR_LEN(stars); layer++)
        for (size_t i = 0; i < ARR_LEN(stars[0]); i++)
            DrawRectangleV(stars[layer][i].pos, stars[layer][i].size, STAR_COLOR);
}

void update_stars(const Vector2 *speed, float dt)
{
    Vector2 delta = Vector2Scale(*speed, dt * WINDOW_WIDTH);

    for (size_t layer = 0; layer < ARR_LEN(stars); layer++)
    {
        Vector2 actual_delta = Vector2Scale(delta, STAR_LAYER_SPEED_FACTOR * (layer + 1));
        for (size_t i = 0; i < ARR_LEN(stars[0]); i++)
        {
            Vector2 new_pos = Vector2Subtract(stars[layer][i].pos, actual_delta);

            if (new_pos.x <= 0)
            {
                new_pos.x = WINDOW_WIDTH;
                new_pos.y = rand() % WINDOW_HEIGHT;
            }
            else if (new_pos.x >= WINDOW_WIDTH)
            {
                new_pos.x = 0;
                new_pos.y = rand() % WINDOW_HEIGHT;
            }
            else if (new_pos.y <= 0)
            {
                new_pos.x = rand() % WINDOW_WIDTH;
                new_pos.y = WINDOW_HEIGHT;
            }
            else if (new_pos.y >= WINDOW_HEIGHT)
            {
                new_pos.x = rand() % WINDOW_WIDTH;
                new_pos.y = 0;
            }

            stars[layer][i].pos = new_pos;
        }
    }
}

void init_stars(void)
{
    for (size_t layer = 0; layer < ARR_LEN(stars); layer++)
    {
        for (size_t i = 0; i < ARR_LEN(stars[0]); i++)
        {
            stars[layer][i].pos = (Vector2){ .x = rand() % WINDOW_WIDTH,
                                             .y = rand() % WINDOW_HEIGHT };
            stars[layer][i].size = (Vector2){ STAR_SIZE * (layer + 1), STAR_SIZE * (layer + 1) };
        }
    }
}

int main(void)
{
    srand(time(NULL));
    init_stars();

    SetTraceLogLevel(LOG_ERROR);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        Vector2 center = { .x = (float)WINDOW_WIDTH / 2,
                           .y = (float)WINDOW_HEIGHT / 2 };

        Vector2 mouse_pos = GetMousePosition();
        Vector2 speed = Vector2Scale(Vector2Subtract(mouse_pos, center), 1 / ((float)WINDOW_WIDTH / 2) * SPEED_FACTOR);
#ifdef DEBUG
        char buf[256];
        snprintf(buf, sizeof(buf), "FPS: %.2f\nSPEED x: %10.2f%%, y: %10.2f%%", 1 / dt, speed.x * 100, speed.y * 100);
        DrawText(buf, 10, 10, 16, WHITE);
#endif
        update_stars(&speed, dt);

        draw_spaceship(&center, &mouse_pos);
        draw_stars();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
