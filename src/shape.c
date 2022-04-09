#include "main.h"

Polygon NewPolygon(int sides, float size, Color color, Rotation rotation) {

    Polygon poly = {0};
    poly.vertOffsets = (Vector2 *) RL_MALLOC(sides * sizeof(Vector2));
    poly.vertices    = (Vector2 *) RL_MALLOC(sides * sizeof(Vector2));
    poly.rotation    = rotation;
    poly.sides       = sides;
    poly.radius      = size;
    poly.color       = color;

    float centralAngle = 0.0f;
    if (rotation) {
        centralAngle = +(360 / (float) sides) / 2;
    }

    for (int i = 0; i < sides; i++) {
        centralAngle += 360.0f / (float) sides;
        poly.vertOffsets[i] = (Vector2) {
                sinf(DEG2RAD * centralAngle) * size,
                cosf(DEG2RAD * centralAngle) * size
        };
    }
    return poly;
}

void DrawPolygon(Polygon *shape, Vector2 center, Color color) {

    shape->center = center;
    shape->color  = color;
    float rotationAngle = ((360 / (float) shape->sides) / 2) * rotationAngle;

    for (int c = 0; c < shape->sides; c++) {
        shape->vertices[c] = Vector2Add(
                shape->center,
                shape->vertOffsets[c]);
    }
    DrawPoly(
            shape->center, shape->sides,
            shape->radius, rotationAngle, color);
}