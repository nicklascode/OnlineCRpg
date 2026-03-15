#include <raylib.h>

#ifndef MATH_H
#define MATH_H

typedef struct Vector2I {
    int x;
    int y;
} Vector2I;

// Convert raylib's Vector2 to our Vector2I and vice versa
static inline Vector2 Vector2IToVector2(Vector2I v) {
    return (Vector2){ (float)v.x, (float)v.y };
}

static inline Vector2I Vector2ToVector2I(Vector2 v) {
    return (Vector2I){ (int)v.x, (int)v.y };
}

// Macros
#define V2I_TO_V2(v) Vector2IToVector2(v)
#define V2_TO_V2I(v) Vector2ToVector2I(v)

#endif // MATH_H