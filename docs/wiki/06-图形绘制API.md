# 06 - 图形绘制 API

`Fgui_graphic.hpp` 提供了大量便捷绘制函数，可直接在 `ActionOnPaint` 或任意渲染器上使用。所有函数以 `paint*` 开头。

## 1. 文本绘制

```cpp
// 普通文本，center 为对齐基准（{0.5,0.5}=居中，{0,0.5}=左对齐垂直居中）
SDL_Rect paintText(SDL_Renderer* r, int x, int y, TTF_Font* f, const char* str,
                   SDL_Color color, const SDL_FPoint& center = {0.0f,0.0f});

// 按长度换行
SDL_Rect paintText_wrapped(SDL_Renderer* r, int x, int y, int warpLen,
                           TTF_Font* f, const char* str, SDL_Color color,
                           const SDL_FPoint& center = {0.0f,0.0f});

// 限制最大宽度（超出裁剪）
SDL_Rect paintText_wm(SDL_Renderer* r, int x, int y, int wmax,
                      TTF_Font* f, const char* str, SDL_Color color,
                      const SDL_FPoint& center = {0.0f,0.0f});

// 跑马灯滚动文本
SDL_Rect paintText_roll(SDL_Renderer* r, int x, int y, int wmax, float speed,
                        TTF_Font* f, const char* str, SDL_Color color,
                        SDL_FPoint center = {0.0f,0.0f});

// 压缩宽度（缩放填满 wmax）
SDL_Rect paintText_compressed(SDL_Renderer* r, int x, int y, int wmax,
                              TTF_Font* f, const char* str, SDL_Color color,
                              SDL_FPoint center = {0.0f,0.0f});

// 带背景矩形的文本
SDL_Rect paintTextWithRect(SDL_Renderer* r, int x, int y, TTF_Font* f,
                           const char* str, SDL_Color color, SDL_Color color_rect);

SDL_Rect getTextSize(TTF_Font* f, const char* str);   // 文本尺寸
```

> 控件内部文本渲染一般用 `FontEx::paintText_Blended`（封装更高级，支持对齐、样式）。

## 2. 圆形 / 圆角矩形（重点，性能经过优化）

这些函数是纯整数运算、无临时纹理、无渲染目标切换、无堆分配，适合每帧绘制。

```cpp
// 空心圆
void paintCircle(SDL_Renderer* r, SDL_Color color, SDL_Point center, int radius);

// 空心圆 + 填充（flags 控制填充区域，PCIRCLE_ALL = 整圆填充）
void paintCircleEx(SDL_Renderer* r, SDL_Color edge_color, SDL_Color fill_color,
                   SDL_Point center, int radius, uint32_t flags = PCIRCLE_ALL);

// 圆角矩形（radius<=0 退化为普通矩形；INT_MAX = 胶囊/全圆）
void paintRoundedRect(SDL_Renderer* r, SDL_Rect rect, SDL_Color edge_color,
                      SDL_Color fill_color, int radius);

// 实心圆（float 半径）
void paintFilledCircle(SDL_Renderer* r, SDL_Color color, SDL_Point center, float radius);

// 椭圆
void paintEllipse(SDL_Renderer* r, const SDL_Rect& rect, /* ... */);
```

```cpp
// 示例：胶囊形轨道（ToggleButton 内部实现）
paintRoundedRect(renderer, track_rect, color_kit.BorderColor, color_kit.ForegroundColor, INT_MAX);

// 示例：圆角按钮
paintRoundedRect(renderer, draw_area, edge, fill, 8);
```

## 3. 渐变 / 填充

```cpp
void paintGradientRectV(SDL_Renderer* ren, SDL_Rect r, SDL_Color in, SDL_Color out); // 垂直渐变
void paintGradientRectH(SDL_Renderer* ren, SDL_Rect r, SDL_Color in, SDL_Color out); // 水平渐变
void paintHatchingLines(SDL_Renderer* renderer, SDL_Color color, SDL_Rect rect,
                        int line_size = 1, int freq = 8);    // 斜线阴影填充
void paintFilledTriangle(SDL_Renderer* ren, const SDL_Point& p1, const SDL_Point& p2,
                         const SDL_Point& p3, SDL_Color color);
```

## 4. 特殊图形

```cpp
// 对话气泡（direction: 1上 2右 3下 4左）
void paintSpeechBubble(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color fill_color,
                       SDL_Color edge_color, int direction);

// "写字"动画（按 process 0~1 逐步画折线，CheckBox 勾选动画用）
void paintWritingLines(SDL_Renderer* renderer, const SDL_Point* points, int count, double process);

// 关闭按钮 X（配合 SDL_RenderDrawLine 用）
void paintX(SDL_Renderer* render, SDL_Rect rct, SDL_Color color);

// 凹陷/凸起边框
SDL_Rect paintInRelief(SDL_Renderer* ren, SDL_Rect base);
SDL_Rect paintOutRelief(SDL_Renderer* ren, SDL_Rect base);
SDL_Rect paintReliefButton(SDL_Renderer* r, SDL_Rect rect, TTF_Font* font,
                           const char* str, bool state);

// 卡片翻转（3D 透视）
SDL_Rect paintCard(SDL_Renderer* r, SDL_Texture* obverse, SDL_Texture* reverse,
                   SDL_Rect rect, double tangle_rad, SDL_Color color_edge);
SDL_Rect paintStereoCard(SDL_Renderer* r, SDL_Texture* obverse, SDL_Texture* reverse,
                         SDL_Rect rect, float x_rotate, float y_rotate, double focus_distance);

// 3D 纹理表面（透视四边形映射）
void paint3DSurface(SDL_Renderer* r, SDL_Texture* texture,
                    SDL_FPoint& perA, SDL_FPoint& perB, SDL_FPoint& perC, SDL_FPoint& perD,
                    float uv_x = 0, float uv_y = 0, float uv_w = 1, float uv_h = 1);
// paint3DSurfaceQ / paint3DSurfaceDQ / paint3DSurfaceI(...)  变体

// 注释标注框
void paintAnnotation(SDL_Renderer* ren, SDL_Rect rect);

// 色板 / 取值板（HSV 拾色器用）
void paintColorTable(SDL_Renderer* renderer, SDL_Rect rect);
void paintValueTable(SDL_Renderer* renderer, float H, float S, SDL_Rect rect);

// 透明棋盘格
void paintTransparency(SDL_Renderer* renderer, int x, int y, int side_length, int cw, int ch);
```

## 5. 弹窗工具类

```cpp
// 线程状态机弹窗（独立窗口 + 渲染器 + 线程自循环，推消息驱动）
class TooltipWindow {
    // 构造传入 std::function<void(SDL_Window*, SDL_Renderer*, int)>
    // PushMsg 发消息；#define TOOLTIP_CALLBACK_INIT/LOOP/QUIT/MSG 定义回调
};

// 自绘下拉/悬浮弹窗（Win32 子窗口 + 软件渲染器 + HWND 子类化）
class PopupWindow {
    void SetRenderCallback(std::function<void(SDL_Renderer*, const SDL_Rect&, const SDL_Point&)>);
    bool Open(SDL_Renderer*, int x, int y, int w, int h);
    void Close();
    void Render();                    // 每帧调用
    void PollInput();                 // 每帧轮询鼠标状态
    bool ConsumeClick(SDL_Point&);    // 消费一次点击
    int  ConsumeWheel();              // 消费滚轮
    bool IsMouseInside();  SDL_Point GetMousePos();  bool IsButtonDown();
    HWND GetHwnd();
    static float GetDpiScaleOf(SDL_Renderer*);
};
```

> `ComboBox` 的下拉列表即基于 `PopupWindow` 实现。若你要做自定义浮层，可参考它。

## 下一步

- 换主题：[配色与主题](07-配色与主题.md)
