#ifndef RAYGJK_MAIN_H
#define RAYGJK_MAIN_H
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <raymath.h>

typedef enum rotate {
    flat  = 1,
    point = 0,
} Rotation;

typedef struct poly_collider {
    Color   color;
    Vector2 center;
    float   radius;
    float   rotation;
    int     sides;
    Vector2 *vertices;
    Vector2 *vertOffsets;
} Polygon;

Polygon ColliderInit(Polygon *collider, Vector2 origin, float size);

Polygon NewPolygon(int sides, float size, Color color, Rotation rotation);

void DrawPolygon(Polygon *shape, Vector2 center, Color color);

void Input(Polygon *shape);

int GJK(const Vector2 *vertices1, size_t count1,
        const Vector2 *vertices2, size_t count2);

#endif //RAYGJK_MAIN_H
