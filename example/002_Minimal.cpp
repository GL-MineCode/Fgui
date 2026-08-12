/**
 * @brief 最小示例程序
 * @details 这是一个最小的标准Fgui程序示例，仅包含核心代码。
 */

#define NOMINMAX
#include "Fgui\Fgui.hpp"

int main(int,char**){
    //初始化SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("Minimal Example",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000,800,SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Rect top_relative_rect = {0,0,1000,800};
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

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
            if(eve.type == SDL_QUIT) running = false;
            if(SDL_GetWindowID(window) == eve.window.windowID) cb->MaintainEvent(&eve,{0,0,1000,800});
        }
        //调用Tick回调
        cb->MaintainTimer({0,0,1000,800});
        //读取脏区域
        SDL_Rect dirty_rect = cb->GetInvaildRect(top_relative_rect);
        //判断是否有脏区域
        if(dirty_rect.w != 0 && dirty_rect.h != 0){
            //脏区域重绘
            SDL_SetRenderDrawColor(renderer,
                cb->GetColorKit().BackgroundColorDarker.r,
                cb->GetColorKit().BackgroundColorDarker.g,
                cb->GetColorKit().BackgroundColorDarker.b, 255);
            SDL_RenderFillRect(renderer, &dirty_rect);
            cb->MaintainRender(renderer, top_relative_rect);
            SDL_RenderPresent(renderer);
        }
        fps_lim.Delay();
    }
    //释放SDL资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}