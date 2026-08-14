#ifndef __INC_FGUI_DEFS_
#define __INC_FGUI_DEFS_

#define ColorArg(c) c.r, c.g, c.b, c.a
#define ColorArgNA(c) c.r, c.g, c.b

#include <SDL3/SDL.h>

// ============ SDL_Rect/SDL_Point -> SDL_FRect/SDL_FPoint 转换辅助 ============
// SDL3 的渲染 API（SDL_RenderFillRect/RenderRect/RenderTexture 等）改用 float 坐标，
// 需要 SDL_FRect/SDL_FPoint。Fgui 内部仍以 SDL_Rect/SDL_Point（int）作为逻辑坐标，
// 此处提供便捷转换函数，避免在调用处重复写转换代码。
inline SDL_FRect toFRect(const SDL_Rect& r){
    return {(float)r.x, (float)r.y, (float)r.w, (float)r.h};
}

inline SDL_FPoint toFPoint(const SDL_Point& p){
    return {(float)p.x, (float)p.y};
}

// SDL3 的 SDL_Vertex::color 为 SDL_FColor（float 分量）
inline SDL_FColor toFColor(const SDL_Color& c){
    return {c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f};
}

inline SDL_FColor toFColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255){
    return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
}

#endif
