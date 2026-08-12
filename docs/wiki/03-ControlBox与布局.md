# 03 - ControlBox 与布局

`ControlBox` 是 Fgui 的容器控件，类似 Windows 的"子父窗口"概念：持有子控件、管理事件分发、自动计算内容范围并提供滚动能力。

## 构造

```cpp
ControlBox(const SDL_Rect& rect = {0,0,0,0}, int _inner_w = -1, int _inner_h = -1);
```

- `rect`：容器矩形（相对父容器）。
- `_inner_w / _inner_h`：容器内部逻辑宽/高。`-1` 表示**自动检测**（根据子控件矩形扩展）。显式指定可强制内容区域大小，内容超过时出现滚动条。

```cpp
// 自动内容区
auto cb = std::make_shared<ControlBox>(SDL_Rect{0,0,400,400});

// 固定内部 500x800，超出即可滚动
auto cb = std::make_shared<ControlBox>(SDL_Rect{500,400,400,400}, 500, 800);
```

## 子控件管理

```cpp
// 追加（返回 shared_ptr<T>，同时设置 parent）
auto btn = cb->PushbackControl<PushButton>("btn1", std::make_shared<PushButton>(...));
// 或（模板参数可推导）
auto btn = cb->PushbackControl("btn1", std::make_shared<PushButton>(...));

// 插到最前
cb->PushfrontControl("name", control);

// 查找（按名字，找不到抛 std::runtime_error）
auto c = cb->FindControl<PushButton>("btn1");

// 移除
cb->RemoveControl("btn1");

// 广播事件（排除某控件，relative_rect 传 {0,0,0,0}）
cb->Broadcast(&event, exclude);
```

子控件的 `parent` 指针自动指向容器，`RadioButton` 等控件会利用 `parent` 向同级广播（实现互斥）。

## 自动滚动与滚动条

`ControlBox` 内部自动持有 `ScrollBarV` / `ScrollBarH`：

- 内容矩形（子控件并集）超过容器大小时自动出现滚动条（右下角各 20px）。
- 滚动条默认显示，可隐藏（隐藏后仍可用滚轮/程序滚动）：

```cpp
cb->SetShowScrollBars(false);   // 隐藏滚动条
bool show = cb->IsShowScrollBars();
```

- 程序化滚动：

```cpp
cb->SetScrollPosX(int);   // 设置横向滚动位置（自动钳位）
cb->SetScrollPosY(int);   // 设置纵向滚动位置（自动钳位）
int x = cb->GetScrollPosX();
int y = cb->GetScrollPosY();
```

- 滚轮在容器上方时按每格 45px 滚动；若子控件（如 InputBox）消费了滚轮，容器自身不再滚动（滚轮消费机制）。

## 平滑滚动

`enable_animation == true` 时，滚动位置会平滑过渡（而非瞬间跳变）：

```cpp
cb->SetScrollSmoothSpeed(0.2f);   // 0.01~1，越大越快
float s = cb->GetScrollSmoothSpeed();
```

拖动滚动条时内容即时跟随，避免滑块与内容脱节。

## 坐标与命中（布局要点）

Fgui 没有排版引擎（未来计划），布局需手动计算每个控件的 `SDL_Rect`。要点：

1. **子控件的 `default_rect` 是相对父容器的坐标**。
2. 贴边的子控件要留余量或钳制，否则 `y+h` 超出 1px 就会触发根容器出现滚动条。
3. 窗口级根容器一般设置为 `SetShowScrollBars(false)`，避免无意义滚动条。

## 子控件接收事件的坐标语义

`ControlBox` 转发事件时：

```cpp
// 事件路径：relative_rect = 父原点 - 滚动偏移（不含 child.default_rect.xy）
child->MaintainEvent(event, {new_rel_rect.x - GetDisplayXPos(), new_rel_rect.y - GetDisplayYPos(), ...});
```

因此子控件在 `ActionOnEvent` 中计算自己的屏幕矩形必须：

```cpp
SDL_Rect draw_area = {relative_rect.x + default_rect.x,
                      relative_rect.y + default_rect.y,
                      default_rect.w, default_rect.h};
```

而 `ActionOnPaint` 中 **不能** 再加 `default_rect.xy`（绘制路径已包含偏移）。

## 示例

```cpp
// 根容器
auto root = std::make_shared<ControlBox>(SDL_Rect{0,0,1000,800});

// 嵌套容器（500x800 固定内容区，可滚动）
auto sub = root->PushbackControl("sub", std::make_shared<ControlBox>(SDL_Rect{500,400,400,400}, 500, 800));
sub->PushbackControl("t1", std::make_shared<StaticText>("文本",&font,46,SDL_Rect{100,100}));
sub->PushbackControl("b1", std::make_shared<PushButton>("按钮",&font,46,SDL_Rect{200,100}));
```

## 下一步

- [控件参考（上）：基础控件](04-控件参考上-基础控件.md)
