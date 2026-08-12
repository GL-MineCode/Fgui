# Fgui --- Fair GUI Library for SDL2

## 这是一个基于 SDL2 的 GUI 库，用于快速开发基于 SDL2 的游戏或应用。  
噢等等，那个是脏区重绘机制吗？没错这款库竟然支持脏区重绘，这使得它在处理复杂场景时能够保持良好的性能（其实由于SDL2每次Render Present 都需要交换整个缓冲区，所以性能会受到影响，导致其可能没有其他GUI库比如WinUI占用少。但这起码是SDL2这种蹩脚轮子能写出来的最好的带脏区重绘优化的GUI库了）

## 优点
- 易于做跨平台兼容
- 支持脏区重绘优化，性能比每帧全部重画要好一点（SDL2对性能优化一点也不友好，我已经尽力了）
- 支持自定义控件
- 支持更换配色方案（？我不知道这算不算值得说的优点，但还是加上吧）
- 内置 15+ 种常用控件，开箱即用
- 依赖极简：只需 SDL2 / SDL2_ttf

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
#include "decode.hpp"               // 必须先于 Fgui.hpp 包含，避免符号重定义
#include "Fgui\Fgui.hpp"

int main(int,char**){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();
    SDLWindowStuff* sws = SDLWindowStuff::Create("程序模板",1000,800,SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = sws->CreateRenderer();
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

    FontEx font({"font.ttf"});

    std::shared_ptr<ControlBox> cb = std::make_shared<ControlBox>(SDL_Rect{0,0,1000,800});
    cb->PushbackControl("hello", std::make_shared<StaticText>("Hello Fgui!",&font,32,SDL_Rect{400,350}));

    SDL_Event eve;
    EventPlus evp(&eve);
    Timer fps_lim(60);
    while(true){
        while(evp.PollEvent()){
            if(eve.type == SDL_QUIT) return 0;
            cb->MaintainEvent(&eve,{0,0,1000,800});
        }
        cb->MaintainTimer({0,0,1000,800});
        cb->MaintainRender(renderer,{0,0,1000,800});
        SDL_RenderPresent(renderer);
        fps_lim.Delay();
    }
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

:) GL 2026-08-12