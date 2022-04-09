
//  Created by Igor Kroitor on 29/12/15.

#include "main.h"

//-----------------------------------------------------------------------------
// Gilbert-Johnson-Keerthi (GJK) collision detection algorithm in 2D
// http://www.dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/
// http://mollyrocket.com/849
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Basic vector arithmetic operations
// **Replaced most of these with Raymath equivalents
Vector2 Vector2Perpendicular(Vector2 v) {
    Vector2 p = {v.y, -v.x};
    return p;
}
//-----------------------------------------------------------------------------
// Triple product expansion is used to calculate Vector2Perpendicular normal vectors
// which kinda 'prefer' pointing towards the Origin in Minkowski space

Vector2 TripProduct(Vector2 a, Vector2 b, Vector2 c) {

    Vector2 r;

    float ac = a.x * c.x + a.y * c.y; // perform a.dot(c)
    float bc = b.x * c.x + b.y * c.y; // perform b.dot(c)

    // perform b * a.dot(c) - a * b.dot(c)
    r.x = b.x * ac - a.x * bc;
    r.y = b.y * ac - a.y * bc;
    return r;
}

//-----------------------------------------------------------------------------
// This is to compute average center (roughly). It might be different from
// Center of Gravity, especially for bodies with nonuniform density,
// but this is ok as initial direction of simplex search in GJK.

Vector2 AvgPoint(const Vector2 *vertices, size_t count) {
    Vector2     avg = {0.f, 0.f};
    for (size_t i   = 0; i < count; i++) {
        avg.x += vertices[i].x;
        avg.y += vertices[i].y;
    }
    avg.x /= (float) count;
    avg.y /= (float) count;
    return avg;
}

//-----------------------------------------------------------------------------
// Get furthest vertex along a certain direction

size_t FurthestIndex(const Vector2 *vertices, size_t count, Vector2 direction) {

    float       maxProduct = Vector2DotProduct(direction, vertices[0]);
    size_t      index      = 0;
    for (size_t i          = 1; i < count; i++) {
        float product = Vector2DotProduct(direction, vertices[i]);
        if (product > maxProduct) {
            maxProduct = product;
            index      = i;
        }
    }
    return index;
}

//-----------------------------------------------------------------------------
// Minkowski sum support function for GJK

Vector2 Support(const Vector2 *vertices1, size_t count1,
                const Vector2 *vertices2, size_t count2, Vector2 direction) {

    // get furthest point of first body along an arbitrary direction
    size_t i = FurthestIndex(vertices1, count1, direction);

    // get furthest point of second body along the opposite direction
    size_t j = FurthestIndex(vertices2, count2, Vector2Negate(direction));

    // Vector2Subtract (Minkowski sum) the two points to see if bodies 'overlap'
    return Vector2Subtract(vertices1[i], vertices2[j]);
}

//-----------------------------------------------------------------------------
// The GJK yes/no test

int iter_count = 0;

int GJK(const Vector2 *vertices1, size_t count1,
        const Vector2 *vertices2, size_t count2) {

    size_t  index = 0; // index of current vertex of simplex
    Vector2 a, b, c, direction, ao, ab, ac, abperp, acperp, simplex[3];

    Vector2 position1 = AvgPoint(vertices1, count1); // not a CoG but
    Vector2 position2 = AvgPoint(vertices2, count2); // it's ok for GJK )

    // initial direction from the center of 1st body to the center of 2nd body
    direction = Vector2Subtract(position1, position2);

    // if initial direction is zero – set it to any arbitrary axis (we choose X)
    if ((direction.x == 0) && (direction.y == 0)) {
        direction.x = 1.f;
    }

    // set the first support as initial point of the new simplex
    a = simplex[0] = Support(vertices1, count1, vertices2, count2, direction);

    if (Vector2DotProduct(a, direction) <= 0) {
        return 0;
    } // no collision

    direction = Vector2Negate(
            a); // The next search direction is always towards the origin, so the next search direction is negate(a)

    while (1) {
        iter_count++;

        a = simplex[++index] = Support(
                vertices1, count1, vertices2, count2, direction);

        if (Vector2DotProduct(a, direction) <= 0)
            return 0; // no collision

        ao = Vector2Negate(a); // from point A to Origin is just negative A

        // simplex has 2 points (a line segment, not a triangle yet)
        if (index < 2) {
            b             = simplex[0];
            ab            = Vector2Subtract(b, a); // from point A to B
            direction     = TripProduct(
                    ab, ao, ab); // normal to AB towards Origin
            if (Vector2LengthSqr(direction) == 0)
                direction = Vector2Perpendicular(ab);
            continue; // skip to next iteration
        }

        b  = simplex[1];
        c  = simplex[0];
        ab = Vector2Subtract(b, a); // from point A to B
        ac = Vector2Subtract(c, a); // from point A to C

        acperp = TripProduct(ab, ac, ac);

        if (Vector2DotProduct(acperp, ao) >= 0) {

            direction = acperp; // new direction is normal to AC towards Origin

        }
        else {

            abperp = TripProduct(ac, ab, ab);

            if (Vector2DotProduct(abperp, ao) < 0)
                return 1; // collision

            simplex[0] = simplex[1]; // swap first element (point C)

            direction = abperp; // new direction is normal to AB towards Origin
        }

        simplex[1] = simplex[2]; // swap element in the middle (point B)
        --index;
    }

    return 0;
}

//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <float.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
float Perturbation()
{
    return (float) rand() / (float) RAND_MAX * FLT_EPSILON * 100.0f * ((rand() % 2) ? 1.0f : -1.0f);
}
#pragma clang diagnostic pop

Vector2 Jostle(Vector2 a)
{
    Vector2 b;
    b.x = a.x + Perturbation();
    b.y = a.y + Perturbation();
    return b;
}