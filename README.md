# 本项目已迁移至 SDL3

# Fgui --- Fair GUI Library for SDL3

## 这是一个基于 SDL3 的 GUI 库，用于快速开发基于 SDL3 的游戏或应用。  
本项目暂时没有充分利用SDL3的优化功能，这将会在后续版本中优化。

## 优点
- 易于做跨平台兼容
- 支持脏区重绘优化，性能比每帧全部重画要好一点
- 支持自定义控件
- 支持更换配色方案
- 内置 15+ 种常用控件，开箱即用
- 依赖极简：只需 SDL3 / SDL3_ttf

## 缺点
- 没排版引擎（这将是硬伤，等我有空和GenFromXML功能一起出）
- 目前仅面向 Windows（内部使用了 Win32 API 实现下拉弹窗等特性）

## 支持的控件

| 控件 | 类名 | 说明 |
| --- | --- | --- |
| 垂直滚动条 | `ScrollBarV` | 上下页面翻动 |
| 水平滚动条 | `ScrollBarH` | 左右页面翻动 |
| 控件容器 | `ControlBox` | 类似 Windows 子父窗口的概念，自动滚动 + 滚动条 |
| 按钮 | `PushButton` | 最基础的控件之一 |
| 静态文本 | `StaticText` | 虽然叫 Static，但可以修改显示文本 |
| 链接文本 | `LinkText` | 带下划线的可点击文本 |
| 输入框 | `InputBox` | 单行/多行文本输入（整个库最复杂的控件） |
| 复选框 | `CheckBox` | 三态：勾选 / 不勾选 / 部分勾选 |
| 开关 | `ToggleButton` | 本质上是更好看的复选框 |
| 单选按钮 | `RadioButton` | 与同 ControlBox 内其他单选按钮互斥 |
| 图片框 | `PictureBox` | 支持 5 种图片填充方式 |
| 滑动条 | `Slider` | 数值调节 |
| 下拉列表框 | `ComboBox` | 自绘 PopupWindow 弹窗下拉 |
| 进度条 | `ProgressBar` | 三态：正常 / 处理中 / 暂停 |
| 选项卡 | `TabControl` | 多标签页，支持关闭、拖动重排、四个方向 |

### 可选编译控件
- `QRCodeBox` --- 二维码控件，基于 QR-Code-generator 库实现（需定义 `ENABLE_QRCODE_MODULE` 宏）

## 快速开始

```cpp
#define NOMINMAX
#include "Fgui\Fgui.hpp"

int main(int,char**){
    //初始化SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("Minimal Example",1000,800,0);
    SDL_Rect top_relative_rect = {0,0,1000,800};
    SDL_Renderer* renderer = SDL_CreateRenderer(window,NULL);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    Fgui_SetWindow(window);

    // 持久帧缓冲纹理：脏区重绘在离屏纹理上局部进行，再整体拷贝到 backbuffer 呈现，
    // 避免 SDL3 双缓冲交换机制导致脏区重绘时旧画面丢失/闪烁。
    SDL_Texture* frame_buf = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,1000,800);

    //准备GUI
    FontEx font({"font.ttf"});
    //创建根容器
    std::shared_ptr<ControlBox> cb = std::make_shared<ControlBox>(SDL_Rect{0,0,1000,800});
    //创建一个按钮
    auto button = cb->PushbackControl("hello", std::make_shared<PushButton>("Hello Fgui!",&font,128,SDL_Rect{0,0,0,0}));
    //居中显示
    button->default_rect.x = 500 - button->default_rect.w / 2;
    button->default_rect.y = 400 - button->default_rect.h / 2;
    //设定点击回调
    button->actions.OnClick = [](PushButton *){printf("Hello Fgui!\n");};

    //SDL事件循环必须事项
    SDL_Event eve;
    Timer fps_lim(60);
    bool running = true;

    //SDL事件循环
    while(running){
        //处理SDL事件
        while(SDL_PollEvent(&eve)){
            if(eve.type == SDL_EVENT_QUIT) running = false;
            if(SDL_GetWindowID(window) == eve.window.windowID) cb->MaintainEvent(&eve,{0,0,1000,800});
        }
        //调用Tick回调
        cb->MaintainTimer({0,0,1000,800});
        //读取脏区域
        SDL_Rect dirty_rect = cb->GetInvaildRect(top_relative_rect);
        //判断是否有脏区域
        if(dirty_rect.w != 0 && dirty_rect.h != 0){
            //在持久帧缓冲纹理上局部重绘脏区
            SDL_FRect fr_dirty = toFRect(dirty_rect);
            SDL_SetRenderTarget(renderer, frame_buf);
            SDL_SetRenderDrawColor(renderer,
                cb->GetColorKit().BackgroundColorDarker.r,
                cb->GetColorKit().BackgroundColorDarker.g,
                cb->GetColorKit().BackgroundColorDarker.b, 255);
            SDL_RenderFillRect(renderer, &fr_dirty);
            cb->MaintainRender(renderer, top_relative_rect);
            SDL_SetRenderTarget(renderer, NULL);
            //整体拷贝持久缓冲到 backbuffer 并呈现
            SDL_RenderTexture(renderer, frame_buf, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
        fps_lim.Delay();
    }
    //释放SDL资源
    SDL_DestroyTexture(frame_buf);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
```

## 构建

项目使用 CMake，提供 4 个预设（MinGW Makefiles）：

```bash
# 仅测试程序 main.exe（Debug / Release）
cmake --preset debug
cmake --build build/debug --target fast_gui2

# 测试程序 + 示例程序
cmake --preset debug-examples
cmake --build build/debug-examples --target examples
```

可执行文件统一输出到 `bin/` 目录，注意启动程序时指定工作目录为 `res/` 目录，否则会缺DLL缺资源。

## 文档（Wiki）

详细的库文档请见 [`docs/wiki/`](docs/wiki/README.md)：

- [快速开始](docs/wiki/00-快速开始.md)
- [构建与项目结构](docs/wiki/01-构建与项目结构.md)
- [核心架构（控件基类 / 脏区重绘 / 事件系统）](docs/wiki/02-核心架构.md)
- [ControlBox 与布局](docs/wiki/03-ControlBox与布局.md)
- [控件参考（上）：基础控件](docs/wiki/04-控件参考上-基础控件.md)
- [控件参考（下）：复杂控件](docs/wiki/05-控件参考下-复杂控件.md)
- [图形绘制 API](docs/wiki/06-图形绘制API.md)
- [配色与主题](docs/wiki/07-配色与主题.md)
- [自定义控件](docs/wiki/08-自定义控件.md)
- [示例教程：文本编辑器](docs/wiki/09-示例教程-文本编辑器.md)
- [常见问题与陷阱](docs/wiki/10-常见问题与陷阱.md)

## 未来计划
- 支持排版引擎，用于自动布局控件，属性类似CSS
- 支持GenFromXML功能，用于从XML文件加载并生成控件(It saves your hands!   -   指可以AI生成XML文件，而不是手动写)
- 支持跨平台（Linux平台）

## Contributor

### 我(GL)
### D指导(DeepSeek，后来加入的，他修复了我2年来写的屎山代码，他比我更懂写GUI，我是完全没经验纯摸索的)

## 后记

真的有人试吃我的屎山代码吗？这玩意就是我周末抽空写的一个学习项目而已。但是我会试图让它更好的。

:) GL 2026-08-15