#ifndef MATH_H
#define MATH_H

typedef struct vec2 {
    float x;
    float y;
} Vec2;

typedef struct vec2i {
    int x;
    int y;
} Vec2I;

#define V2_TO_V2I(v) ((Vec2I){(int)(v.x), (int)(v.y)})
#define V2I_TO_V2(v) ((Vec2){(float)(v.x), (float)(v.y)})

#endif // MATH_H