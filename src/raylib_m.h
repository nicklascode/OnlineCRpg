#ifndef RAYLIB_M_H
#define RAYLIB_M_H

#undef CloseWindow
#undef ShowCursor
#undef LoadImage
#undef DrawText
#undef DrawTextEx
#undef PlaySound

#define CloseWindow _RaylibCloseWindow
#define ShowCursor  _RaylibShowCursor
#define LoadImage   _RaylibLoadImage
#define DrawText    _RaylibDrawText
#define DrawTextEx  _RaylibDrawTextEx
#define PlaySound   _RaylibPlaySound

#include <raylib.h>

#undef CloseWindow
#undef ShowCursor
#undef LoadImage
#undef DrawText
#undef DrawTextEx
#undef PlaySound

#endif // RAYLIB_M_H
