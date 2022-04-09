#include "main.h"
#include <raymath.h>

int main(void) {

    //      Initialization      \\

    const int screenWidth  = (GetMonitorWidth(1) / 2);
    const int screenHeight = (GetMonitorHeight(1) / 2);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Window");

    Camera2D camera;
    camera.target   = Vector2Zero();
    camera.zoom     = 10;
    camera.rotation = 0;
    camera.offset   = Vector2Zero();

    int polysides = 4;

    Polygon shape1 = NewPolygon(polysides, 100, BLUE, flat);
    Polygon shape2 = NewPolygon(polysides, 100, GREEN, flat);

    Vector2 center = (Vector2) {(float) GetScreenWidth() / 2, (float) GetScreenHeight() / 2};

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        ClearBackground(RAYWHITE);
        BeginMode2D(camera);
        BeginDrawing();

        DrawPolygon(&shape1, GetMousePosition(), shape1.color);
        DrawPolygon(&shape2, center, shape2.color);

        int collisionDetected = GJK(
                shape1.vertices, shape1.sides,
                shape2.vertices, shape2.sides);

        if (!collisionDetected) {
            shape2.color = RED;
        }
        else {
            shape2.color = BLACK;
        }

        EndDrawing();
        EndMode2D();
    }
    CloseWindow();
    return 0;
}