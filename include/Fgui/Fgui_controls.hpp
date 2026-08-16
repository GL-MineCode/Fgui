#ifndef __INC_FGUI_CONTROLS_
#define __INC_FGUI_CONTROLS_

#include "Fgui_includes.hpp"
#include "Fgui_math.hpp"
#include "Fgui_defs.hpp"
#include "Fgui_graphic.hpp"

/**
 * @brief 按需选择拓展模块
 * 
 */

//#define ENABLE_QRCODE_MODULE

#ifdef ENABLE_QRCODE_MODULE

#include "QR-Code-generator/qrcodegen.hpp"

#endif

/**
 * @brief 配色结构体
 * 
 */
struct Fgui_ColorKit
{
    //前景色
	SDL_Color ForegroundColor;
    //背景色
	SDL_Color BackgroundColor;
    //更深的前景色
	SDL_Color ForegroundColorDarker;
    //更深的背景色
	SDL_Color BackgroundColorDarker;
    //更亮的前景色
	SDL_Color ForegroundColorLighter;
    //更亮的背景色
	SDL_Color BackgroundColorLighter;
    //边框颜色
	SDL_Color BorderColor;
    //文本颜色
	SDL_Color TextColor;
};

#ifndef DefaultColorKit
#define DefaultColorKit ColorKitPresets::Default
#endif

namespace ColorKitPresets{

    //纯深色主题
    const static constexpr Fgui_ColorKit Dark = Fgui_ColorKit{
        {150, 150, 150, 255},   // 前景色
        {30, 30, 30, 255},      // 背景色
        {60, 60, 60, 255},      // 更深前景色
        {20, 20, 20, 255},      // 更深背景色
        {190, 190, 190, 255},   // 更亮前景色
        {50, 50, 50, 255},      // 更亮背景色
        {0, 0, 0, 255},         // 边框色
        {235, 235, 235, 255}    // 文本色
    };

    //浅色主题
    const static constexpr Fgui_ColorKit Light = Fgui_ColorKit{
        {120, 120, 120, 255},
        {245, 245, 245, 255},
        {200, 200, 200, 255},
        {220, 220, 220, 255},
        {90, 90, 90, 255},
        {255, 255, 255, 255},
        {160, 160, 160, 255},
        {40, 40, 40, 255}
    };

    //蓝色主题
    const static constexpr Fgui_ColorKit Blue = Fgui_ColorKit{
        {59, 130, 246, 255},
        {30, 41, 59, 255},
        {37, 99, 235, 255},
        {15, 23, 42, 255},
        {96, 165, 250, 255},
        {51, 65, 85, 255},
        {0, 0, 0, 255},
        {255, 255, 255, 255}
    };

    //绿色主题
    const static constexpr Fgui_ColorKit Green = Fgui_ColorKit{
        {34, 197, 94, 255},
        {20, 30, 24, 255},
        {22, 163, 74, 255},
        {10, 20, 14, 255},
        {74, 222, 128, 255},
        {30, 45, 34, 255},
        {0, 0, 0, 255},
        {240, 255, 245, 255}
    };

    //红色主题
    const static constexpr Fgui_ColorKit Red = Fgui_ColorKit{
        {239, 68, 68, 255},
        {40, 25, 25, 255},
        {220, 38, 38, 255},
        {25, 15, 15, 255},
        {248, 113, 113, 255},
        {55, 35, 35, 255},
        {0, 0, 0, 255},
        {255, 245, 245, 255}
    };

    //橙色主题
    const static constexpr Fgui_ColorKit Orange = Fgui_ColorKit{
        {249, 115, 22, 255},
        {40, 28, 18, 255},
        {234, 88, 12, 255},
        {25, 18, 12, 255},
        {251, 146, 60, 255},
        {55, 38, 24, 255},
        {0, 0, 0, 255},
        {255, 250, 245, 255}
    };

    //紫色主题
    const static constexpr Fgui_ColorKit Purple = Fgui_ColorKit{
        {168, 85, 247, 255},
        {34, 25, 45, 255},
        {147, 51, 234, 255},
        {22, 16, 30, 255},
        {192, 132, 252, 255},
        {48, 35, 60, 255},
        {0, 0, 0, 255},
        {250, 245, 255, 255}
    };

    //终端风格（黑底绿字）
    const static constexpr Fgui_ColorKit Terminal = Fgui_ColorKit{
        {0, 255, 65, 255},
        {0, 0, 0, 255},
        {0, 180, 45, 255},
        {10, 10, 10, 255},
        {120, 255, 150, 255},
        {30, 30, 30, 255},
        {0, 200, 80, 255},
        {0, 255, 65, 255}
    };

    //海洋主题
    const static constexpr Fgui_ColorKit Ocean = Fgui_ColorKit{
        {14, 165, 233, 255},
        {8, 47, 73, 255},
        {3, 105, 161, 255},
        {4, 30, 50, 255},
        {56, 189, 248, 255},
        {15, 70, 100, 255},
        {0, 0, 0, 255},
        {240, 250, 255, 255}
    };

    //日落主题
    const static constexpr Fgui_ColorKit Sunset = Fgui_ColorKit{
        {244, 114, 182, 255},
        {45, 24, 50, 255},
        {236, 72, 153, 255},
        {30, 15, 35, 255},
        {251, 146, 60, 255},
        {60, 35, 60, 255},
        {0, 0, 0, 255},
        {255, 250, 250, 255}
    };

    const static constexpr Fgui_ColorKit Default = Dark;
}

static bool enable_animation = false;

/**
 * @brief 设置控件动画是否启用，启用可能会显著增加性能消耗
 * 
 * @param enable 
 */
void Fgui_ConfigControlAnimation(bool enable){
    enable_animation = enable;
}

// 全局主窗口句柄（SDL3 的文本输入/IME 相关 API 需要 SDL_Window*）
static SDL_Window* g_fgui_window = nullptr;

/**
 * @brief 设置 Fgui 关联的主窗口（用于 SDL3 文本输入/IME API）
 * 
 * @param win 主 SDL_Window
 */
void Fgui_SetWindow(SDL_Window* win){
    g_fgui_window = win;
}

class ControlBox;

/**
 * @brief 所有控件的父类
 * 
 */
class Fgui_Control
{
private:

    //定时器的临时数值，不应该被更改，该成员由MaintainTimer函数管理
    uint64_t timer_temp = 0;

protected:

    //将要重绘的脏区范围，不应该被更改，除非你想重载InvalidateRect
    SDL_Rect dirty_region = {0,0,0,0};

    //控件配色
	Fgui_ColorKit color_kit = DefaultColorKit;

    //ActionOnTick被调用的时间间隔，0表示每帧调用
    uint64_t timer_time = 0;

    //控件的可见性
    bool visibility = true;

public:	

    //父控件
    ControlBox* parent = nullptr;

    //控件矩形范围
    SDL_Rect default_rect = {0,0,0,0};

    // 滚轮事件消费标记：本控件（或其子树）实际滚动滚轮后置 true，父容器据此跳过自身滚动
    bool wheel_consumed = false;
    void ResetWheelConsumed(){ wheel_consumed = false; }
    bool IsWheelConsumed() const{ return wheel_consumed; }

	virtual ~Fgui_Control(){};

    /**
     * @brief 通用控件重绘回调，调用该函数时，渲染器的视口会被设置为default_rect，禁止在该函数内调用延时函数
     * 
     * @param renderer 渲染器
     * @param dirty_region 脏区范围
     */
	virtual void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) = 0;

    /**
     * @brief 通用控件事件回调，禁止在该函数内调用延时函数
     * 
     * @param event SDL事件
     */
	virtual void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) = 0;

    /**
     * @brief 通用控件滴答计时器回调，禁止在该函数内调用延时函数
     * 
     */
	virtual void ActionOnTimer(const SDL_Rect& relative_rect) = 0;

    /**
     * @brief 获取控件类型名称，遵循"[控件包名].[控件名称]"的命名规范
     * 
     */
    virtual std::string ActionOnGetTypeName() const = 0;

    virtual void ActionOnColorKitChange(const Fgui_ColorKit& ck){
        color_kit = ck;
        this->InvalidateRect();
    };

    /**
     * @brief 指定控件脏区以强制渲染
     * 
     * @param rect 脏区范围
     */
    virtual void InvalidateRect(const SDL_Rect& rect){
        dirty_region = ExtendRect(dirty_region,rect);
    }

    /**
     * @brief 强制渲染整个控件
     * 
     */
    virtual void InvalidateRect(){
        this->InvalidateRect({0,0,default_rect.w,default_rect.h});
    }

    /**
     * @brief 获得脏区绝对范围
     * 
     * @return 脏区的绝对范围 
     */
    SDL_Rect GetInvaildRect(const SDL_Rect& relative_rect) const{
        SDL_Rect temp = relative_rect;
        temp.x += dirty_region.x;
        temp.y += dirty_region.y;
        temp.w = dirty_region.w;
        temp.h = dirty_region.h;
        //SDL_Rect dr = ClipRect(temp,dirty_region);
        return temp;
    }

    /**
     * @brief 获得脏区相对范围
     * 
     * @return 脏区的相对范围 
     */
    SDL_Rect GetRelativeInvaildRect() const{
        return dirty_region;
    }

    //重置脏区范围
    void ClearInvaildRect(){
        dirty_region = {0,0,0,0};
    }

    /**
     * @brief 维护渲染
     * 
     * @param renderer 渲染器
     * @return 完成渲染的脏区
     */
    SDL_Rect MaintainRender(SDL_Renderer* renderer,const SDL_Rect& relative_rect){
        if(!visibility) return {0,0,0,0};
        if(dirty_region.w == 0 || dirty_region.h == 0) return {0,0,0,0};
        SDL_Rect dr = ClipRect(relative_rect,default_rect);

        SDL_Rect dr2 = ClipRect(dr,dirty_region);

        SDL_Rect r;
        SDL_GetRenderClipRect(renderer, &r);
        SDL_Rect dr3 = SDL_RectEmpty(&r) ? dr2 : RectIntersection(r,dr2);
        SDL_SetRenderClipRect(renderer, &dr3);
        ActionOnPaint(renderer,dr3,dr);
        if(SDL_RectEmpty(&r)) SDL_SetRenderClipRect(renderer, NULL);
        else SDL_SetRenderClipRect(renderer, &r);
        //SDL_SetRenderClipRect(renderer, NULL);

        dirty_region = {0,0,0,0};
        return dr;
    }

    /**
     * @brief 维护定时器
     * 
     */
    void MaintainTimer(const SDL_Rect& relative_rect){
        if(!visibility) return;
        if(timer_time == 0){
            this->ActionOnTimer(relative_rect);
        }else{
            uint64_t current_temp = SDL_GetTicks() / timer_time;
            if(timer_temp != current_temp){
                this->ActionOnTimer(relative_rect);
                timer_temp = current_temp;
            }
        }
    }

    /**
     * @brief 维护事件处理
     * 
     * @param eve SDL事件
     */
    void MaintainEvent(const SDL_Event* eve,const SDL_Rect& relative_rect){
        // 每轮滚轮事件重置消费标记（放在可见性判断前，避免隐藏子控件残留旧值被误判为已消费）
        if(eve->type == SDL_EVENT_MOUSE_WHEEL) wheel_consumed = false;
        if(!visibility) return;
        this->ActionOnEvent(eve,relative_rect);
    }


    const Fgui_ColorKit& GetColorKit() const{
        return color_kit;
    }

    /**
     * @brief 设置控件可见性（隐藏后不绘制、不处理事件与定时器）
     */
    void SetVisibility(bool v){
        if(visibility != v){
            visibility = v;
            if(visibility) this->InvalidateRect();
        }
    }
    bool IsVisibility() const{ return visibility; }

};

#define CallIfNotNull(func, ... ) if(func) func(__VA_ARGS__)
#define SetValueIfNotNull(func,value, ... ) if(func) value = func(__VA_ARGS__)

/**
 * @brief 垂直滑条，用于上下的页面翻动
 * 
 */
class ScrollBarV : public Fgui_Control
{

    int total_height;
    int window_height;
    int ypos;

    bool hovering;
    bool dragging;
    int drag_y_rel_bar;

    // 滚动范围两端额外 padding（px），使滚动位置可进入负值 / 超出内容末尾
    int pad_top = 0;
    int pad_bottom = 0;

    SDL_Rect ClacBarRect(){
        SDL_Rect draw_area = this->default_rect;
        draw_area.x = 0;
        draw_area.y = 0;

        int bar_height = ((double)window_height / (double)total_height) * (double)draw_area.h;
        // 滚动范围：[-pad_top, total_height - window_height + pad_bottom]
        int range = (total_height - window_height) + pad_top + pad_bottom;
        int bar_y;
        if(range <= 0){
            bar_y = draw_area.y;
        }else{
            bar_y = (int)(((double)(ypos + pad_top) / (double)range) * (double)(draw_area.h - bar_height)) + draw_area.y;
        }

        return {draw_area.x,bar_y,draw_area.w,bar_height};
    }

public:

    struct Actions{
        //std::function<int(int ypos, int window_height, int total_height)> OnBeginScrolling;
        std::function<int(int ypos, int window_height, int total_height)> OnScrolling;
        std::function<int(int ypos, int window_height, int total_height)> OnStopScrolling;
        std::function<void(int ypos, int window_height, int total_height)> OnHeightChanged;
    } actions;

    void SetHeightInfo(int _total_height,int _window_height){
        if(_total_height < _window_height) _total_height = _window_height;
        if(_total_height != total_height || _window_height != window_height){
            total_height = _total_height;
            window_height = _window_height;
            ypos = std::clamp(ypos, -pad_top, (total_height - window_height) + pad_bottom);
            CallIfNotNull(actions.OnHeightChanged, ypos, window_height, total_height);
            this->InvalidateRect();
        }
    }

    void SetScrollPos(int pos) {
        int new_pos = std::clamp(pos, -pad_top, (total_height - window_height) + pad_bottom);
        if (new_pos != ypos) {
            ypos = new_pos;
            this->InvalidateRect();
        }
    }

    /**
     * @brief 设置滚动范围两端额外 padding（px），使滚动条位置可进入负值/超出内容末尾
     * 
     * @param _pad_top 起始端 padding（>=0）
     * @param _pad_bottom 结束端 padding（>=0）
     */
    void SetPadding(int _pad_top,int _pad_bottom){
        _pad_top = std::max(0,_pad_top);
        _pad_bottom = std::max(0,_pad_bottom);
        if(pad_top != _pad_top || pad_bottom != _pad_bottom){
            pad_top = _pad_top;
            pad_bottom = _pad_bottom;
            ypos = std::clamp(ypos, -pad_top, (total_height - window_height) + pad_bottom);
            this->InvalidateRect();
        }
    }

    void GetHeightInfo(int& _total_height,int& _window_height) const{
        _total_height = total_height;
        _window_height = window_height;
    }

    int GetScrollPos() const{
        return ypos;
    }

    // 滚动条是否正在被拖动（ControlBox 平滑滚动时用于判断是否即时跟随）
    bool IsDragging() const{
        return dragging;
    }

    /**
     * @brief 构造ScrollBar控件
     * 
     * @param _total_height 页面总高
     * @param _window_height 窗口高度
     * @param rect 
     */
	ScrollBarV(int _total_height,int _window_height,const SDL_Rect& rect = {0,0,0,0},int _pad_top = 0,int _pad_bottom = 0)
	{
		default_rect = rect;

        if(_total_height < _window_height) _total_height = _window_height;

        total_height = _total_height;
        window_height = _window_height;

        pad_top = std::max(0,_pad_top);
        pad_bottom = std::max(0,_pad_bottom);

        hovering = false;
        dragging = false;
        drag_y_rel_bar = 0;

        ypos = 0;

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{
        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColor));
    SDL_FRect __fr_1 = toFRect(draw_area);
		SDL_RenderFillRect(renderer, &__fr_1);

        SDL_Rect bar = ClacBarRect();
        bar.x += draw_area.x;
        bar.y += draw_area.y;

        if(hovering || dragging){
            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.ForegroundColorLighter));
        }else{
            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.ForegroundColor));
        }
    SDL_FRect __fr_2 = toFRect(bar);
		SDL_RenderFillRect(renderer, &__fr_2);

		SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_3 = toFRect(draw_area);
		SDL_RenderRect(renderer, &__fr_3);
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        SDL_Rect bar = ClacBarRect();
        bar.x += relative_rect.x + default_rect.x;
        bar.y += relative_rect.y + default_rect.y;
        if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    if (isPointInsideRect(pt, bar)){
                        drag_y_rel_bar = pt.y - bar.y;
                    }else{
                        drag_y_rel_bar = bar.h / 2;
                    }
                    dragging = true;
                    
                }
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_BUTTON_UP){
            //SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(dragging){
                SetValueIfNotNull(actions.OnStopScrolling,ypos,ypos,window_height,total_height);
                this->InvalidateRect();
            }
            dragging = false;
        }
        else if(event->type == SDL_EVENT_MOUSE_MOTION){
            SDL_Point pt = RelativizePoint({(int)event->motion.x,(int)event->motion.y},relative_rect);
            if(dragging){
                // 按滚动范围（含两端 padding）反算滚动位置：轨道像素 -> [-pad_top, total-window+pad_bottom]
                int track_h = default_rect.h - bar.h;
                if(track_h > 0){
                    int bar_y = std::clamp(pt.y - drag_y_rel_bar, default_rect.y, default_rect.y + track_h);
                    int range = (total_height - window_height) + pad_top + pad_bottom;
                    SetScrollPos( -pad_top + (bar_y - default_rect.y) * range / track_h );
                }
                SetValueIfNotNull(actions.OnScrolling,ypos,ypos,window_height,total_height);
            }
            if (isPointInsideRect(pt, bar))
            {
                if (!hovering)
                {
                    hovering = true;
                    this->InvalidateRect();
                }
            }
            else
            {
                if (hovering)
                {
                    hovering = false;
                    this->InvalidateRect();
                }
            }
        }
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{
        
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.ScrollBarV";
    }
};

/**
 * @brief 水平滑条，用于左右的页面翻动
 * 
 */
class ScrollBarH : public Fgui_Control
{
    int total_width;
    int window_width;
    int xpos;

    bool hovering;
    bool dragging;
    int drag_x_rel_bar;

    // 滚动范围两端额外 padding（px），使滚动位置可进入负值 / 超出内容末尾
    int pad_left = 0;
    int pad_right = 0;

    SDL_Rect CalcBarRect() {
        SDL_Rect draw_area = this->default_rect;
        draw_area.x = 0;
        draw_area.y = 0;

        int bar_width = ((double)window_width / (double)total_width) * (double)draw_area.w;
        // 滚动范围：[-pad_left, total_width - window_width + pad_right]
        int range = (total_width - window_width) + pad_left + pad_right;
        int bar_x;
        if(range <= 0){
            bar_x = draw_area.x;
        }else{
            bar_x = (int)(((double)(xpos + pad_left) / (double)range) * (double)(draw_area.w - bar_width)) + draw_area.x;
        }

        return {bar_x, draw_area.y, bar_width, draw_area.h};
    }

public:

    struct Actions{
        //std::function<int(int ypos, int window_height, int total_height)> OnBeginScrolling;
        std::function<int(int xpos, int window_width, int total_width)> OnScrolling;
        std::function<int(int xpos, int window_width, int total_width)> OnStopScrolling;
        std::function<void(int xpos, int window_width, int total_width)> OnWidthChanged;
    } actions;

    void SetWidthInfo(int _total_width, int _window_width) {
        if (_total_width < _window_width) _total_width = _window_width;
        if (_total_width != total_width || _window_width != window_width) {
            total_width = _total_width;
            window_width = _window_width;
            xpos = std::clamp(xpos, -pad_left, (total_width - window_width) + pad_right);
            CallIfNotNull(actions.OnWidthChanged,xpos,window_width,total_width);
            this->InvalidateRect();
        }
    }

    void SetScrollPos(int pos) {
        int new_pos = std::clamp(pos, -pad_left, (total_width - window_width) + pad_right);
        if (new_pos != xpos) {
            xpos = new_pos;
            this->InvalidateRect();
        }
    }

    /**
     * @brief 设置滚动范围两端额外 padding（px），使滚动条位置可进入负值/超出内容末尾
     * 
     * @param _pad_left 起始端 padding（>=0）
     * @param _pad_right 结束端 padding（>=0）
     */
    void SetPadding(int _pad_left, int _pad_right) {
        _pad_left = std::max(0,_pad_left);
        _pad_right = std::max(0,_pad_right);
        if(pad_left != _pad_left || pad_right != _pad_right){
            pad_left = _pad_left;
            pad_right = _pad_right;
            xpos = std::clamp(xpos, -pad_left, (total_width - window_width) + pad_right);
            this->InvalidateRect();
        }
    }

    void GetWidthInfo(int& _total_width, int& _window_width) const {
        _total_width = total_width;
        _window_width = window_width;
    }

    int GetScrollPos() const {
        return xpos;
    }

    // 滚动条是否正在被拖动（ControlBox 平滑滚动时用于判断是否即时跟随）
    bool IsDragging() const {
        return dragging;
    }

    /**
     * @brief 构造 ScrollBarH 控件
     * 
     * @param _total_width 页面总宽
     * @param _window_width 窗口宽度
     * @param rect 控件区域（相对于父容器）
     */
    ScrollBarH(int _total_width, int _window_width, const SDL_Rect& rect = {0,0,0,0}, int _pad_left = 0, int _pad_right = 0) {
        default_rect = rect;

        if (_total_width < _window_width) _total_width = _window_width;

        total_width = _total_width;
        window_width = _window_width;

        pad_left = std::max(0,_pad_left);
        pad_right = std::max(0,_pad_right);

        hovering = false;
        dragging = false;
        drag_x_rel_bar = 0;

        xpos = 0;

        this->InvalidateRect();
    }

    void ActionOnPaint(SDL_Renderer* renderer, const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override {
        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColor));
    SDL_FRect __fr_4 = toFRect(draw_area);
        SDL_RenderFillRect(renderer, &__fr_4);

        SDL_Rect bar = CalcBarRect();
        bar.x += draw_area.x;
        bar.y += draw_area.y;

        if (hovering || dragging) {
            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.ForegroundColorLighter));
        } else {
            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.ForegroundColor));
        }
    SDL_FRect __fr_5 = toFRect(bar);
        SDL_RenderFillRect(renderer, &__fr_5);

        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_6 = toFRect(draw_area);
        SDL_RenderRect(renderer, &__fr_6);
    }

    void ActionOnEvent(const SDL_Event* event, const SDL_Rect& relative_rect) override {
        SDL_Rect bar = CalcBarRect();
        bar.x += relative_rect.x + default_rect.x;
        bar.y += relative_rect.y + default_rect.y;

        if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            SDL_Point pt = RelativizePoint({(int)event->button.x, (int)event->button.y}, relative_rect);
            if (event->button.button == SDL_BUTTON_LEFT) {
                if (isPointInsideRect(pt, default_rect)) {
                    if (isPointInsideRect(pt, bar)) {
                        drag_x_rel_bar = pt.x - bar.x;
                    } else {
                        drag_x_rel_bar = bar.w / 2;
                    }
                    dragging = true;
                }
            }
        }
        else if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
            //SDL_Point pt = RelativizePoint({(int)event->button.x, (int)event->button.y}, relative_rect);
            if(dragging){
                SetValueIfNotNull(actions.OnStopScrolling,xpos,xpos,window_width,total_width);
                this->InvalidateRect();
            }
            dragging = false;
        }
        else if (event->type == SDL_EVENT_MOUSE_MOTION) {
            SDL_Point pt = RelativizePoint({(int)event->motion.x, (int)event->motion.y}, relative_rect);
            if (dragging) {
                // 按滚动范围（含两端 padding）反算滚动位置：轨道像素 -> [-pad_left, total-window+pad_right]
                int track_w = default_rect.w - bar.w;
                if(track_w > 0){
                    int bar_x = std::clamp(pt.x - drag_x_rel_bar, default_rect.x, default_rect.x + track_w);
                    int range = (total_width - window_width) + pad_left + pad_right;
                    SetScrollPos(-pad_left + (bar_x - default_rect.x) * range / track_w);
                }
                SetValueIfNotNull(actions.OnScrolling,xpos,xpos,window_width,total_width);
            }
            if (isPointInsideRect(pt, bar)) {
                if (!hovering) {
                    hovering = true;
                    this->InvalidateRect();
                }
            } else {
                if (hovering) {
                    hovering = false;
                    this->InvalidateRect();
                }
            }
        }
    }

    void ActionOnTimer(const SDL_Rect& relative_rect) override {
        
    }

    std::string ActionOnGetTypeName() const override {
        return "Fgui.ScrollBarH";
    }
};

/**
 * @brief 控件容器，类似Windows中的子父窗口的概念
 * 
 */
class ControlBox : public Fgui_Control{

    protected:
    
    struct ControlData{
        std::string name;
        std::shared_ptr<Fgui_Control> control_ptr;

        ControlData(const std::string& _name,
        std::shared_ptr<Fgui_Control> _control_ptr)
        :name(_name),control_ptr(_control_ptr)
        {

        }
    };
    std::vector<ControlData> controls;

    ControlData& _FindControl_refControlData(const std::string& name){
        for(auto& i : controls){
            if(i.name == name){
                return i;
            }
        }
        throw std::runtime_error(cpp_sformat("未找到控件:\"%s\"",name.c_str()));
    }

    std::vector<ControlData>::iterator _FindControl_iterator(const std::string& name){
        for(auto i = controls.begin();i < controls.end();i++){
            if(i->name == name){
                return i;
            }
        }
        throw std::runtime_error(cpp_sformat("未找到控件:\"%s\"",name.c_str()));
    }

    void TestInvalidateRect(const SDL_Rect& relative_rect){
        SDL_Rect temp = {0,0,0,0};

        SDL_Rect viewport = {relative_rect.x, relative_rect.y, default_rect.w, default_rect.h};

        for(auto& i : controls){
            if(!i.control_ptr->IsVisibility()) continue;
            SDL_Rect child_dirty_local = i.control_ptr->GetRelativeInvaildRect();
            if(child_dirty_local.w <= 0 || child_dirty_local.h <= 0) continue;

            SDL_Rect child_abs = {
                relative_rect.x + i.control_ptr->default_rect.x - GetDisplayXPos(),
                relative_rect.y + i.control_ptr->default_rect.y - GetDisplayYPos(),
                i.control_ptr->default_rect.w,
                i.control_ptr->default_rect.h
            };

            SDL_Rect child_dirty_screen = i.control_ptr->GetInvaildRect(child_abs);
            SDL_Rect visible = RectIntersection(child_dirty_screen, viewport);
            if(visible.w <= 0 || visible.h <= 0) continue;

            temp = ExtendRect(visible, temp);
        }

        SDL_Rect rect = GetInnerRect();
        if(rect.h != default_rect.h){
            sb_v->default_rect = {default_rect.w - 20,0,20,default_rect.h - 20};
            sb_v->SetHeightInfo(rect.h,default_rect.h);
            if(show_scrollbars){
                SDL_Rect dr = ClipRect(relative_rect,sb_v->default_rect);
                temp = ExtendRect(sb_v->GetInvaildRect(dr),temp);
            }
        }
        if(rect.w != default_rect.w){
            sb_h->default_rect = {0,default_rect.h - 20,default_rect.w - 20,20};
            sb_h->SetWidthInfo(rect.w,default_rect.w);
            if(show_scrollbars){
                SDL_Rect dr = ClipRect(relative_rect,sb_h->default_rect);
                temp = ExtendRect(sb_h->GetInvaildRect(dr),temp);
            }
        }

        this->InvalidateRect(temp);
    }

    SDL_Rect ClacControlsRect(){
        SDL_Rect rect = default_rect;
        for(auto& i : controls){
            rect = ExtendRect(rect,i.control_ptr->default_rect);
        }
        return rect;
    }

    int inner_w = -1, inner_h = -1;
    bool auto_scroll = true;
    bool show_scrollbars = true;   // 是否显示滚动条（隐藏后仍支持滚轮/程序滚动）

    // 滚动目标位置（由滚动条/滚轮驱动）
    int xpos = 0, ypos = 0;

    // 平滑滚动：display_* 为当前实际渲染偏移（平滑逼近 xpos/ypos），
    // enable_animation 为 true 时启用平滑，否则立即同步
    double display_xpos = 0, display_ypos = 0;
    // 平滑速度（每帧逼近目标的比例，0~1，越大越快）
    float scroll_smooth_alpha = 0.2f;

    SDL_Point mouse_pos = {-1,-1};

    SDL_Rect GetInnerRect(){
        SDL_Rect rect = default_rect;
        SDL_Rect inner_rect = ClacControlsRect();
        if(inner_w == -1){
            if(auto_scroll) rect.w = inner_rect.w;
        }
        else{
            rect.w = std::max(inner_w,rect.w);
        }
        if(inner_h == -1){
            if(auto_scroll) rect.h = inner_rect.h;
        }
        else{
            rect.h = std::max(inner_h,rect.h);
        }
        return rect;
    }

    ScrollBarV* sb_v = nullptr;
    ScrollBarH* sb_h = nullptr;

    // 当前渲染使用的滚动偏移（平滑滚动时用平滑值）
    int GetDisplayXPos() const{
        return (int)std::lround(display_xpos);
    }

    int GetDisplayYPos() const{
        return (int)std::lround(display_ypos);
    }

    // 推进平滑滚动：enable_animation 时平滑逼近 xpos/ypos，否则立即同步
    void AdvanceSmoothScroll(){
        if(enable_animation){
            // 拖动滚动条时内容即时跟随，避免滑块与内容脱节
            if((sb_v && sb_v->IsDragging()) || (sb_h && sb_h->IsDragging())){
                if(display_xpos != xpos || display_ypos != ypos){
                    display_xpos = xpos;
                    display_ypos = ypos;
                    this->InvalidateRect();
                }
                return;
            }
            double ndx = display_xpos + (xpos - display_xpos) * scroll_smooth_alpha;
            double ndy = display_ypos + (ypos - display_ypos) * scroll_smooth_alpha;
            // 足够接近时吸附到目标，避免无限逼近
            if(std::abs(ndx - xpos) < 0.5) ndx = xpos;
            if(std::abs(ndy - ypos) < 0.5) ndy = ypos;
            if(ndx != display_xpos || ndy != display_ypos){
                display_xpos = ndx;
                display_ypos = ndy;
                this->InvalidateRect();
            }
        }
        else{
            if(display_xpos != xpos || display_ypos != ypos){
                display_xpos = xpos;
                display_ypos = ypos;
                this->InvalidateRect();
            }
        }
    }

    bool ShallEventPenetrate(const SDL_Event* eve,const SDL_Rect& rect){
        if(eve->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
            if(!isPointInsideRect({eve->button.x,eve->button.y},rect)){
                return false;
            }
        }
        else if(eve->type == SDL_EVENT_MOUSE_BUTTON_UP){
            return true;
        }
        else if(eve->type == SDL_EVENT_MOUSE_WHEEL){
            // SDL_Event 为 union，滚轮事件里 button.x/y 与 wheel 结构偏移错位（读到的是 wheel.y 和 direction），
            // 必须用 SDL_GetMouseState 取真实鼠标坐标做命中测试，否则嵌套容器的滚轮永不透传给内层控件
            float mxf, myf;
            SDL_GetMouseState(&mxf, &myf);
            if(!isPointInsideRect({(int)mxf,(int)myf},rect)){
                return false;
            }
        }
        return true;
    }

    public:

    void InvalidateRect(const SDL_Rect& _rect) override{
        if(_rect.w == 0 || _rect.h == 0) return;
        dirty_region = ExtendRect(dirty_region,_rect);
        for(auto& i : controls){
           if(!i.control_ptr->IsVisibility()) continue;
           SDL_Rect real_control_rect = {i.control_ptr->default_rect.x - GetDisplayXPos(),i.control_ptr->default_rect.y - GetDisplayYPos(),i.control_ptr->default_rect.w,i.control_ptr->default_rect.h};
           SDL_Rect drect = RectIntersection(dirty_region,real_control_rect);
           SDL_Rect nd = {drect.x - real_control_rect.x,drect.y - real_control_rect.y,drect.w,drect.h};
           i.control_ptr->InvalidateRect(nd);
        }

        SDL_Rect rect = GetInnerRect();
        if(rect.h != default_rect.h){
            sb_v->default_rect = {default_rect.w - 20,0,20,default_rect.h};
            sb_v->SetHeightInfo(rect.h,default_rect.h);
            if(show_scrollbars){
                SDL_Rect drect = RectIntersection(dirty_region,sb_v->default_rect);
                SDL_Rect nd = {drect.x - sb_v->default_rect.x,drect.y - sb_v->default_rect.y,drect.w,drect.h};
                sb_v->InvalidateRect(nd);
            }
        }
        if(rect.w != default_rect.w){
            sb_h->default_rect = {0,default_rect.h - 20,default_rect.w - 20,20};
            sb_h->SetWidthInfo(rect.w,default_rect.w);
            if(show_scrollbars){
                SDL_Rect drect = RectIntersection(dirty_region,sb_h->default_rect);
                SDL_Rect nd = {drect.x - sb_h->default_rect.x,drect.y - sb_h->default_rect.y,drect.w,drect.h};
                sb_h->InvalidateRect(nd);
            }
        }

        dirty_region = RectIntersection(dirty_region,{0,0,default_rect.w,default_rect.h});
    }

    void InvalidateRect() override{
        this->InvalidateRect({0,0,default_rect.w,default_rect.h});
    }

    /**
     * @brief 设置平滑滚动速度（仅 enable_animation 为 true 时生效）
     * 
     * @param alpha 每帧逼近目标滚动的比例，0~1，越大滚动越快
     */
    void SetScrollSmoothSpeed(float alpha){
        scroll_smooth_alpha = std::clamp(alpha, 0.01f, 1.0f);
    }

    /**
     * @brief 获取当前平滑滚动速度
     */
    float GetScrollSmoothSpeed() const{
        return scroll_smooth_alpha;
    }

    /**
     * @brief 获取当前纵向滚动位置（像素）
     */
    int GetScrollPosY() const{ return ypos; }

    /**
     * @brief 获取当前横向滚动位置（像素）
     */
    int GetScrollPosX() const{ return xpos; }

    /**
     * @brief 设置纵向滚动位置（自动钳位到合法范围）
     */
    void SetScrollPosY(int pos){
        if(sb_v){
            sb_v->SetScrollPos(pos);
            if(sb_v->GetScrollPos() != ypos){
                ypos = sb_v->GetScrollPos();
                this->InvalidateRect();
            }
        }
    }

    /**
     * @brief 设置横向滚动位置（自动钳位到合法范围）
     */
    void SetScrollPosX(int pos){
        if(sb_h){
            sb_h->SetScrollPos(pos);
            if(sb_h->GetScrollPos() != xpos){
                xpos = sb_h->GetScrollPos();
                this->InvalidateRect();
            }
        }
    }

    /**
     * @brief 设置是否显示滚动条（隐藏后仍可用滚轮/程序滚动，仅不绘制且不响应拖动）
     */
    void SetShowScrollBars(bool show){
        if(show_scrollbars != show){
            show_scrollbars = show;
            this->InvalidateRect();
        }
    }
    bool IsShowScrollBars() const{ return show_scrollbars; }

    /**
     * @brief 构造ControlBox控件
     * 
     * @param rect 范围
     * @param _inner_w 容器内部宽度，若为-1则为自动检测
     * @param _inner_h 容器内部高度，若为-1则为自动检测
     */
    ControlBox(const SDL_Rect& rect = {0,0,0,0},int _inner_w = -1,int _inner_h = -1){
        default_rect = rect;
        timer_time = 0;

        inner_w = _inner_w;
        inner_h = _inner_h;
        auto_scroll = true;

        sb_v = new ScrollBarV(rect.h,rect.h,{rect.w - 20,0,20,rect.h});
        sb_h = new ScrollBarH(rect.w,rect.w,{0,rect.h - 20,rect.w,20});

        xpos = 0;
        ypos = 0;
        display_xpos = 0;
        display_ypos = 0;

        mouse_pos = {-1,-1};

        // 必须在所有成员（尤其 sb_v/sb_h）初始化之后调用，否则会解引用未初始化指针
        this->InvalidateRect();
    }

    ~ControlBox(){
        delete sb_v;
        delete sb_h;
    }

	void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{
        // 可见视口（ControlBox在父坐标中的可见区域）
        SDL_Rect viewport = {relative_rect.x, relative_rect.y, default_rect.w, default_rect.h};

        for(auto& i : controls){
            // 跳过不可见或没有脏区的子控件
            if(!i.control_ptr->IsVisibility()) continue;
            SDL_Rect child_dirty_local = i.control_ptr->GetRelativeInvaildRect();
            if(child_dirty_local.w <= 0 || child_dirty_local.h <= 0) continue;

            // 子控件的屏幕绝对位置（考虑滚动偏移）
            SDL_Rect child_abs = {
                relative_rect.x + i.control_ptr->default_rect.x - GetDisplayXPos(),
                relative_rect.y + i.control_ptr->default_rect.y - GetDisplayYPos(),
                i.control_ptr->default_rect.w,
                i.control_ptr->default_rect.h
            };

            // 子控件的脏区在屏幕坐标中的范围
            SDL_Rect child_dirty_screen = i.control_ptr->GetInvaildRect(child_abs);

            // 脏区与视口的交集 = 实际需要绘制且可见的部分
            SDL_Rect visible = RectIntersection(child_dirty_screen, viewport);
            if(visible.w <= 0 || visible.h <= 0){
                i.control_ptr->ClearInvaildRect();
                continue;
            }

            // 保存原有的裁剪区域，设置新的裁剪区域为可见部分
            SDL_Rect old_clip;
            SDL_GetRenderClipRect(renderer, &old_clip);
            SDL_Rect new_clip = SDL_RectEmpty(&old_clip) ? visible : RectIntersection(old_clip, visible);
            SDL_SetRenderClipRect(renderer, &new_clip);

            // 将可见区域转换到子控件的局部坐标系，作为dirty_region传递
            SDL_Rect dirty_in_child = {
                visible.x - child_abs.x,
                visible.y - child_abs.y,
                visible.w,
                visible.h
            };
            i.control_ptr->ActionOnPaint(renderer, dirty_in_child, child_abs);
            i.control_ptr->ClearInvaildRect();

            // 恢复原有的裁剪区域
            if(SDL_RectEmpty(&old_clip)) SDL_SetRenderClipRect(renderer, NULL);
            else SDL_SetRenderClipRect(renderer, &old_clip);
        }

        SDL_Rect rect = GetInnerRect();
        if(rect.h != default_rect.h){
            sb_v->default_rect = {default_rect.w - 20,0,20,default_rect.h};
            sb_v->SetHeightInfo(rect.h,default_rect.h);
            if(show_scrollbars) sb_v->MaintainRender(renderer,relative_rect);
        }
        if(rect.w != default_rect.w){
            sb_h->default_rect = {0,default_rect.h - 20,default_rect.w - 20,20};
            sb_h->SetWidthInfo(rect.w,default_rect.w);
            if(show_scrollbars) sb_h->MaintainRender(renderer,relative_rect);
        }
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        SDL_Rect new_rel_rect = ClipRect(relative_rect,default_rect);
        if(ShallEventPenetrate(event,new_rel_rect)){
            for(auto& i : controls){
                i.control_ptr->MaintainEvent(event,{new_rel_rect.x - GetDisplayXPos(),new_rel_rect.y - GetDisplayYPos(),new_rel_rect.w,new_rel_rect.h});
            }
        }

        if(event->type == SDL_EVENT_MOUSE_WHEEL){
            // 滚轮消费机制：任一子控件（或其子树）已消费滚轮则本容器不再滚动，
            // 并把消费结果继续向上层报告（避免根容器与嵌套容器同时滚动）
            bool child_consumed = false;
            for(auto& i : controls){
                if(i.control_ptr->IsWheelConsumed()){
                    child_consumed = true;
                    break;
                }
            }
            if(child_consumed){
                wheel_consumed = true;
            }
            else{
                float mxf, myf;
                SDL_GetMouseState(&mxf, &myf);
                if(isPointInsideRect({(int)mxf,(int)myf},new_rel_rect)){
                    SDL_Rect rect = GetInnerRect();
                    // 仅当本容器该方向确有溢出（可滚动）时才滚动并消费；无溢出则不消费（透传给父容器）
                    bool can_v = (rect.h != default_rect.h);
                    bool can_h = (rect.w != default_rect.w);
                    bool scrolled = false;
                    if(can_v && event->wheel.y != 0){
                        sb_v->SetScrollPos(sb_v->GetScrollPos() - 45 * (int)event->wheel.y);
                        scrolled = true;
                    }
                    if(can_h && event->wheel.x != 0){
                        sb_h->SetScrollPos(sb_h->GetScrollPos() + 45 * (int)event->wheel.x);
                        scrolled = true;
                    }
                    if(scrolled) wheel_consumed = true;
                }
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_MOTION){
            mouse_pos = {(int)event->motion.x,(int)event->motion.y};
        }

        SDL_Rect rect = GetInnerRect();
        if(rect.h != default_rect.h){
            sb_v->default_rect = {default_rect.w - 20,0,20,default_rect.h};
            sb_v->SetHeightInfo(rect.h,default_rect.h);
            if(show_scrollbars) sb_v->MaintainEvent(event,new_rel_rect);
            if(sb_v->GetScrollPos() != ypos){
                ypos = sb_v->GetScrollPos();
                this->InvalidateRect();
            }
        }
        if(rect.w != default_rect.w){
            sb_h->default_rect = {0,default_rect.h - 20,default_rect.w - 20,20};
            sb_h->SetWidthInfo(rect.w,default_rect.w);
            if(show_scrollbars) sb_h->MaintainEvent(event,new_rel_rect);
            if(sb_h->GetScrollPos() != xpos){
                xpos = sb_h->GetScrollPos();
                this->InvalidateRect();
            }
        }
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{
        SDL_Rect new_rel_rect = ClipRect(relative_rect,default_rect);
        for(auto& i : controls){
            i.control_ptr->MaintainTimer({new_rel_rect.x,new_rel_rect.y,new_rel_rect.w,new_rel_rect.h});
        }
        // 推进平滑滚动（滚动产生的脏区由 InvalidateRect 传播给子控件）
        AdvanceSmoothScroll();
        TestInvalidateRect(relative_rect);
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.ControlBox";
    }

    void ActionOnColorKitChange(const Fgui_ColorKit& ck) override{
        color_kit = ck;
        for(auto& i : controls){
            i.control_ptr->ActionOnColorKitChange(ck);
        }
        if(this->sb_h) this->sb_h->ActionOnColorKitChange(ck);
        if(this->sb_v) this->sb_v->ActionOnColorKitChange(ck);
        this->InvalidateRect();
    }

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Fgui_Control, T>>>
    std::shared_ptr<T> PushbackControl(const std::string &name, std::shared_ptr<T> control_ptr)
    {
        controls.emplace_back(name, control_ptr);
        control_ptr->parent = this;
        return control_ptr;
    }

    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Fgui_Control, T>>>
    std::shared_ptr<T> PushfrontControl(const std::string &name, std::shared_ptr<T> control_ptr)
    {
        auto& item = *controls.emplace(controls.begin(), name, control_ptr);
        control_ptr->parent = this;
        return control_ptr;
    }

    template<typename _T> std::shared_ptr<_T> FindControl(const std::string& name){
        return std::dynamic_pointer_cast<_T>(_FindControl_refControlData(name).control_ptr);
    }

    void RemoveControl(const std::string& name){
        auto i = _FindControl_iterator(name);
        i->control_ptr->parent = nullptr;
        controls.erase(i);
    }

    /**
     * @brief 广播事件到所有子控件
     * 
     * @param event 广播的事件
     * @param exclude 排除的控件，可以为NULL
     * 
     * @note 函数遍历调用所有除指针为exclude的控件的MaintainEvent。注意，此时relative_rect参数为SDL_Rect{0,0,0,0}
     */
    virtual void Broadcast(const SDL_Event* event,const Fgui_Control* exclude){
        for(auto& i : controls){
            if(i.control_ptr.get() == exclude) continue;
            if(!i.control_ptr->IsVisibility()) continue;
            i.control_ptr->MaintainEvent(event,SDL_Rect{0,0,0,0});
        }
    }
};

/**
 * @brief 按钮，最基础的控件之一
 * 
 */
class PushButton : public Fgui_Control
{
    FontEx* font;
    int ptsize;
	std::string text;

    bool _pressing;
    bool _hovered;

    //For animation
    EMAColor bg_color;

    bool enable;

public:

    struct Actions{
        std::function<void(PushButton *)> OnClick;
    } actions;

    void SetText(const std::string& str){
        this->InvalidateRect();
        text = str;
        font->SizeText(ptsize,text,&default_rect.w,&default_rect.h);
        default_rect.w = (float)default_rect.w * 1.2f;
        this->InvalidateRect();
    } 

    void SetFont(FontEx* _font,int _ptsize){
        this->InvalidateRect();
        font = _font;
        ptsize = _ptsize;
        font->SizeText(ptsize,text,&default_rect.w,&default_rect.h);
        default_rect.w = (float)default_rect.w * 1.2f;
        this->InvalidateRect();
    } 

    std::string GetText() const{
        return text;
    }

    bool IsEnabled() const{
        return enable;
    }

    void Enable(){
        enable = true;
    }

    void Disable(){
        enable = false;
    }

    /**
     * @brief 构造PushButton控件
     * 
     * @param str 文本
     * @param _font 字体
     * @param _ptsize 字体大小
     * @param fun_onClick 被按下时调用的回调
     * @param rect 范围
     */
	PushButton(const std::string& str, FontEx* _font,int _ptsize,const SDL_Rect& rect = {0,0,0,0})
	{
		text = str;
        font = _font;
        ptsize = _ptsize;
        default_rect = rect;
        enable = true;
        font->SizeText(ptsize,text,&default_rect.w,&default_rect.h);

        default_rect.w = (float)default_rect.w * 1.2f;

        _pressing = false;

        bg_color.setLastValue(color_kit.BackgroundColor);

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{

        if(enable_animation){
            SDL_Rect draw_area = this->default_rect;
            draw_area.x = relative_rect.x;
            draw_area.y = relative_rect.y;
            SDL_Color bg_c = bg_color.getLastValue(); 
            SDL_SetRenderDrawColor(renderer, ColorArg(bg_c));
    SDL_FRect __fr_7 = toFRect(draw_area);
            SDL_RenderFillRect(renderer, &__fr_7);
            font->paintText_Blended(renderer,draw_area.x + draw_area.w / 2,draw_area.y + draw_area.h / 2,ptsize,text,enable ? color_kit.TextColor : color_kit.ForegroundColor,{0.5f,0.5f});
            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_8 = toFRect(draw_area);
            SDL_RenderRect(renderer, &__fr_8);
            return;
        }

        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        if (_pressing)
		{
			SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColorDarker));
		}
        else if(_hovered)
		{
			SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.ForegroundColor));
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColor));
		}
    SDL_FRect __fr_9 = toFRect(draw_area);
		SDL_RenderFillRect(renderer, &__fr_9);
		
        font->paintText_Blended(renderer,draw_area.x + draw_area.w / 2,draw_area.y + draw_area.h / 2,ptsize,text,enable ? color_kit.TextColor : color_kit.ForegroundColor,{0.5f,0.5f});

		SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_10 = toFRect(draw_area);
		SDL_RenderRect(renderer, &__fr_10);
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    _pressing = true;
                    this->InvalidateRect();
                }
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_BUTTON_UP){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    if(enable) CallIfNotNull(actions.OnClick,this);
                    _pressing = false;
                    this->InvalidateRect();
                }
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_MOTION){
            SDL_Point pt = RelativizePoint({(int)event->motion.x,(int)event->motion.y},relative_rect);
            if (isPointInsideRect(pt, default_rect))
            {
                if (!_hovered)
                {
                    _hovered = true;
                    this->InvalidateRect();
                }
            }
            else
            {
                if (_pressing)
                {
                    _pressing = false;
                    this->InvalidateRect();
                }
                if (_hovered)
                {
                    _hovered = false;
                    this->InvalidateRect();
                }
            }
        }
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{
        if(enable_animation){
            SDL_Color target_c;
            if(enable){
                if (_pressing)
                {
                    target_c = color_kit.BackgroundColorDarker;
                }
                else if(_hovered)
                {
                    target_c = color_kit.ForegroundColor;
                }
                else
                {
                    target_c = color_kit.BackgroundColor;
                }
            }
            else{
                target_c = color_kit.ForegroundColorDarker;
            }
            
            bool changed;
            bg_color.process(target_c,changed);
            if(changed){
                this->InvalidateRect();
            }
        }
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.PushButton";
    }
};

/**
 * @brief 文本，虽然名字叫做StaticText，但是可以修改显示的文本
 * 
 */
class StaticText : public Fgui_Control{
    FontEx* font;
    int ptsize;
	std::string text;

public:

    void SetText(const std::string& str){
        this->InvalidateRect();
        text = str;
        font->SizeText(ptsize,text,&default_rect.w,&default_rect.h);
        this->InvalidateRect();
    } 

    void SetFont(FontEx* _font,int _ptsize){
        this->InvalidateRect();
        font = _font;
        ptsize = _ptsize;
        font->SizeText(ptsize,text,&default_rect.w,&default_rect.h);
        this->InvalidateRect();
    } 

    std::string GetText() const{
        return text;
    }

    /**
     * @brief 构造StaticText控件
     * 
     * @param str 文本
     * @param _font 字体
     * @param _ptsize 字体大小
     * @param rect 范围
     */
	StaticText(const std::string& str, FontEx* _font,int _ptsize,const SDL_Rect& rect = {0,0,0,0})
	{
		text = str;
        font = _font;
        ptsize = _ptsize;
        default_rect = rect;
        font->SizeText(ptsize,text,&default_rect.w,&default_rect.h);

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{
        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        font->paintText_Blended(renderer,draw_area.x + draw_area.w / 2,draw_area.y + draw_area.h / 2,ptsize,text,color_kit.TextColor,{0.5f,0.5f});
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect&) override{

    }

	void ActionOnTimer(const SDL_Rect&) override{
        
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.StaticText";
    }
};

/**
 * @brief 链接文本，可点击
 * 
 */
class LinkText : public Fgui_Control{
    FontEx* font;
    int ptsize;
	std::string text;

    bool _pressing;

public:

    struct Actions{
        std::function<void(LinkText *)> OnClick;
    } actions;

    void SetText(const std::string& str){
        this->InvalidateRect();
        text = str;
        font->SizeText(ptsize,text,&default_rect.w,&default_rect.h);
        this->InvalidateRect();
    } 

    void SetFont(FontEx* _font,int _ptsize){
        this->InvalidateRect();
        font = _font;
        ptsize = _ptsize;
        font->SizeText(ptsize,text,&default_rect.w,&default_rect.h);
        this->InvalidateRect();
    } 

    std::string GetText() const{
        return text;
    }

    /**
     * @brief 构造LinkText控件
     * 
     * @param str 文本
     * @param _font 字体
     * @param _ptsize 字体大小
     * @param fun_onClick 被点击时调用的回调
     * @param rect 范围
     */
	LinkText(const std::string& str, FontEx* _font,int _ptsize,const SDL_Rect& rect = {0,0,0,0})
	{
		text = str;
        font = _font;
        ptsize = _ptsize;
        default_rect = rect;
        font->SizeText(ptsize,text,&default_rect.w,&default_rect.h);

        _pressing = false;

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{

        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

		SDL_Color link_color = _pressing ? SDL_Color{72,60,207,255} : SDL_Color{42,30,255,255};
        font->paintStyledText_Blended(renderer,draw_area.x + draw_area.w / 2,draw_area.y + draw_area.h / 2,ptsize,text,link_color,TTF_STYLE_UNDERLINE,{0.5f,0.5f});
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    _pressing = true;
                    this->InvalidateRect();
                }
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_BUTTON_UP){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    CallIfNotNull(actions.OnClick,this);
                    _pressing = false;
                    this->InvalidateRect();
                }
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_MOTION){
            SDL_Point pt = RelativizePoint({(int)event->motion.x,(int)event->motion.y},relative_rect);
            if(_pressing){
                if(!isPointInsideRect(pt,default_rect)){
                    _pressing = false;
                    this->InvalidateRect();
                }
            }
        }
    }

	void ActionOnTimer(const SDL_Rect&) override{
        
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.LinkText";
    }
};

/**
 * @brief 输入框，目前只能单行输入
 * @note 整个库最复杂的控件，修BUG累死了，还好有D指导
 */
class InputBox : public Fgui_Control{
    FontEx* font;
    int ptsize;
    // 多行字符缓存，每行是一个Codepoint列表，行与行之间隐含一个换行位置
	std::vector<std::vector<uint32_t>> text;

    // 基于FontEx::TextLayout生成的布局
    FontEx::LayoutInfo layout;
    int buff_w,buff_h;

    int cam_x,cam_y;

    // 光标/选区使用全局索引（各行的字符数 + 行间的换行位置）
    size_t selection_primary,selection_secondary;

    bool focused;

    uint64_t cursor_blink_time_temp;

    bool dragging;

    #define FGUI_INPUTBOX_FLAG_ALLOW_MULTILINES 1
    #define FGUI_INPUTBOX_FLAG_PASSWORD 2
    #define FGUI_INPUTBOX_FLAG_NUMBER_ONLY 4
    #define FGUI_INPUTBOX_FLAG_READ_ONLY 8
    #define FGUI_INPUTBOX_FLAG_VERTICAL_SCROLL 16
    #define FGUI_INPUTBOX_FLAG_HORIZONTAL_SCROLL 32
    int flags;

    SDL_Surface* sur_text_buff;

    //---------- 滚动条 ----------
    ScrollBarV* sb_v;
    ScrollBarH* sb_h;
    // 当前是否显示对应滚动条（内容溢出时显示）
    bool show_sb_v;
    bool show_sb_h;

    //---------- 多行索引辅助 ----------

    // 第line行的起始全局索引（前面所有行的字符数 + 换行数）
    size_t GetLineStartIndex(size_t line) const{
        size_t idx = 0;
        for(size_t l = 0; l < line && l < text.size(); l++){
            idx += text[l].size() + 1;
        }
        return idx;
    }

    // 全局总长度（所有字符 + 行间换行位置）
    size_t GetTotalLength() const{
        if(text.empty()) return 0;
        size_t sum = 0;
        for(auto& l : text) sum += l.size();
        return sum + text.size() - 1;
    }

    // (行,列) -> 全局索引
    size_t PosToIndex(size_t line,size_t col) const{
        if(text.empty()) return 0;
        if(line >= text.size()) line = text.size() - 1;
        return GetLineStartIndex(line) + std::min(col,text[line].size());
    }

    // 全局索引 -> (行,列)
    void IndexToPos(size_t idx,size_t& line,size_t& col) const{
        if(text.empty()){ line = 0; col = 0; return; }
        line = 0;
        while(line < text.size()){
            if(idx <= text[line].size()){
                col = idx;
                return;
            }
            idx -= text[line].size() + 1;
            line++;
        }
        line = text.size() - 1;
        col = text[line].size();
    }

    // 全局索引处的字符（行间位置返回换行符）
    uint32_t CharAt(size_t idx) const{
        size_t line,col;
        IndexToPos(idx,line,col);
        if(col < text[line].size()) return text[line][col];
        return '\n';
    }

    // (行,列)在布局中的X坐标
    int LayoutXOfPos(size_t line,size_t col) const{
        int x = 0;
        if(line < layout.line_infos.size()){
            auto& glyphs = layout.line_infos[line].glyphs;
            for(size_t gi = 0; gi < glyphs.size() && gi < col; gi++) x += glyphs[gi].rect.w;
        }
        return x;
    }

    // 布局中的X坐标 -> 该行的列位置
    size_t ColumnFromLayoutX(size_t line,int x) const{
        if(line >= layout.line_infos.size() || line >= text.size()) return 0;
        auto& glyphs = layout.line_infos[line].glyphs;
        if(glyphs.empty()) return 0;
        if(x <= glyphs.front().rect.x) return 0;
        int total = glyphs.back().rect.x + glyphs.back().rect.w;
        if(x >= total) return glyphs.size();
        size_t best = 0;
        int best_dist = INT_MAX;
        for(size_t gi = 0; gi < glyphs.size(); gi++){
            int cx = glyphs[gi].rect.x + glyphs[gi].rect.w / 2;
            int d = abs(cx - x);
            if(d < best_dist){ best_dist = d; best = gi; }
        }
        int center = glyphs[best].rect.x + glyphs[best].rect.w / 2;
        return (x < center) ? best : best + 1;
    }

    //---------- 缓存管理 ----------

    void ClearGlyphCache(){
        // 清空布局会触发Glyph析构，自动释放各Glyph持有的Surface
        layout.line_infos.clear();

        if(sur_text_buff) SDL_DestroySurface(sur_text_buff);
        sur_text_buff = NULL;
    }

    void UpdateGlyphs(){
        ClearGlyphCache();
        if((flags & FGUI_INPUTBOX_FLAG_PASSWORD) == 0){
            font->TextLayout(ptsize,text,layout,true,0,color_kit.TextColor);
        }
        else{
            std::vector<std::vector<uint32_t>> pwd = text;
            for(auto& line : pwd){
                for(auto& c : line){
                    if(!(c < 0x20 || c > 0x7E)) c = 0x25CF;
                }
            }
            font->TextLayout(ptsize,pwd,layout,true,0,color_kit.TextColor);
        }
        buff_w = layout.total_width;
        buff_h = layout.total_height;
        if(buff_w <= 0 || buff_h <= 0) return;
        sur_text_buff = SDL_CreateSurface(buff_w, buff_h, SDL_PIXELFORMAT_ARGB8888);
        SDL_FillSurfaceRect(sur_text_buff,NULL,SDL_MapSurfaceRGBA(sur_text_buff,0,0,0,0));
        int cur_y = 0;
        for(auto& line : layout.line_infos){
            for(auto& glyph : line.glyphs){
                if(glyph.surface){
                    SDL_SetSurfaceBlendMode(glyph.surface,SDL_BLENDMODE_NONE);
                    // 使用surface实际宽高，避免拉伸错位
                    SDL_Rect dst = { glyph.rect.x, cur_y + glyph.rect.y, glyph.surface->w, glyph.surface->h };
                    SDL_BlitSurface(glyph.surface,NULL,sur_text_buff,&dst);
                }
            }
            cur_y += line.rect.h;
        }

        UpdateScrollBarInfo();
    }

    void ResetCursorBlink(){
        cursor_blink_time_temp = SDL_GetTicks();
    }

    size_t PointToChar(const SDL_Point& mousepos,const SDL_Rect& rel_rect){
        // 将鼠标位置转换到缓冲区（布局）坐标
        int lx = mousepos.x - rel_rect.x + cam_x;
        int ly = mousepos.y - rel_rect.y + cam_y;

        if(layout.line_infos.empty()) return 0;

        // 1. 按行高（LineInfo.rect.h）找鼠标所在或最近的行
        size_t best_line = 0;
        int best_line_dist = INT_MAX;
        int line_y = 0;
        for(size_t li = 0; li < layout.line_infos.size(); li++){
            int top = line_y;
            int bottom = line_y + layout.line_infos[li].rect.h;
            int d = 0;
            if(ly < top)        d = top - ly;
            else if(ly >= bottom) d = ly - bottom;
            if(d < best_line_dist){
                best_line_dist = d;
                best_line = li;
                if(d == 0) break; // 精确命中该行
            }
            line_y += layout.line_infos[li].rect.h;
        }

        // 2. 在行内按字符占位中心找插入位置（0..行字符数）
        auto& glyphs = layout.line_infos[best_line].glyphs;
        if(glyphs.empty()) return PosToIndex(best_line,0);

        for(size_t gi = 0; gi < glyphs.size(); gi++){
            int gx = glyphs[gi].rect.x; // 缓冲区坐标
            int gw = glyphs[gi].rect.w; // advance占位宽
            int half = gw / 2;
            // 鼠标在字符左半（含中心）→ 该字符前；右半 → 继续到下一字符前
            if(lx <= gx + half){
                return PosToIndex(best_line,gi);
            }
        }
        return PosToIndex(best_line,glyphs.size()); // 行尾
    }

    /**
     * @brief 计算光标在视口（输入框本地）坐标中的渲染位置
     * @return SDL_Rect 光标的绘制范围（x,y 为视口坐标，w 为0用于画线，h 为高度）
     */
    SDL_Rect GetCursorRenderPos() const{
        if(text.empty() || layout.line_infos.empty()){
            return {0, 0, 0, font->GetPrimaryFontHeight(ptsize)};
        }

        // 光标全局索引 -> (行,列)
        size_t line,col;
        IndexToPos(selection_primary,line,col);

        // 计算该行在缓冲区中的起始Y
        int line_y = 0;
        size_t li = 0;
        for(; li < line && li < layout.line_infos.size(); li++){
            line_y += layout.line_infos[li].rect.h;
        }
        if(li >= layout.line_infos.size()) li = layout.line_infos.size() - 1;

        int cursor_layout_x = LayoutXOfPos(li,col);
        int cursor_layout_h = layout.line_infos[li].rect.h;

        // 转换到视口坐标
        return {cursor_layout_x - cam_x, line_y - cam_y, 0, cursor_layout_h};
    }

    void UpdateCamPos(){
        if(layout.line_infos.empty()){
            cam_x = -10;
            cam_y = 0;
            return;
        }

        // 获取光标在视口坐标中的位置
        SDL_Rect cursor = GetCursorRenderPos();

        // 文本可见区域（扣除滚动条占用空间）
        SDL_Rect inner = GetInnerTextRect();
        SDL_Rect lim_rect = {10, 0, inner.w - 20, inner.h};

        SDL_Rect new_cursor = LimitRectInsideRect(cursor, lim_rect);
        cam_x += cursor.x - new_cursor.x;
        cam_y += cursor.y - new_cursor.y;

        SyncScrollBarsFromCam();
    }

    // 文本可见区域（default_rect 扣除滚动条占用的空间）
    SDL_Rect GetInnerTextRect() const{
        int w = default_rect.w - (show_sb_v ? 20 : 0);
        int h = default_rect.h - (show_sb_h ? 20 : 0);
        return {0, 0, w, h};
    }

    // 垂直滚动条在父坐标中的绝对区域
    SDL_Rect GetScrollBarVAbsRect(const SDL_Rect& draw_area) const{
        return {draw_area.x + default_rect.w - 20, draw_area.y, 20, GetInnerTextRect().h};
    }

    // 水平滚动条在父坐标中的绝对区域
    SDL_Rect GetScrollBarHAbsRect(const SDL_Rect& draw_area) const{
        return {draw_area.x, draw_area.y + default_rect.h - 20, GetInnerTextRect().w, 20};
    }

    // 根据缓冲区大小与控件范围刷新滚动条信息，并把滚动条位置同步回cam
    void UpdateScrollBarInfo(){
        if(!sb_v && !sb_h) return;

        // 两轮迭代确定两个滚动条是否可见（二者相互影响）
        bool show_v = false, show_h = false;
        if(sb_v) show_v = buff_h > default_rect.h;
        if(sb_h) show_h = buff_w > default_rect.w;
        for(int i = 0; i < 2; i++){
            if(sb_v) show_v = buff_h > (default_rect.h - (show_h ? 20 : 0));
            if(sb_h) show_h = buff_w > (default_rect.w - (show_v ? 20 : 0));
        }

        show_sb_v = show_v;
        show_sb_h = show_h;

        int inner_w = default_rect.w - (show_sb_v ? 20 : 0);
        int inner_h = default_rect.h - (show_sb_h ? 20 : 0);

        if(sb_v){
            sb_v->default_rect = {0, 0, 20, inner_h};
            sb_v->SetHeightInfo(buff_h, inner_h);
            // 两端各留 10px padding（幂等，保证无论滚动条如何创建都生效）
            sb_v->SetPadding(10, 10);
            cam_y = sb_v->GetScrollPos();
        }
        if(sb_h){
            sb_h->default_rect = {0, 0, inner_w, 20};
            sb_h->SetWidthInfo(buff_w, inner_w);
            sb_h->SetPadding(10, 10);
            cam_x = sb_h->GetScrollPos();
        }
    }

    // 把cam同步到滚动条（带钳位），用于光标移动后滚动条跟随
    void SyncScrollBarsFromCam(){
        if(sb_v){
            sb_v->SetScrollPos(cam_y);
            cam_y = sb_v->GetScrollPos();
        }
        if(sb_h){
            sb_h->SetScrollPos(cam_x);
            cam_x = sb_h->GetScrollPos();
        }
    }

    // 根据当前flags创建/销毁滚动条（构造函数与SetFlags共用）
    void RecreateScrollBars(){
        if(flags & FGUI_INPUTBOX_FLAG_VERTICAL_SCROLL){
            if(!sb_v){
                sb_v = new ScrollBarV(default_rect.h, default_rect.h, {0, 0, 20, default_rect.h}, 10, 10);
                sb_v->actions.OnScrolling = [this](int pos,int window_height,int total_height){
                    cam_y = pos;
                    this->InvalidateRect();
                    return pos;
                };
                sb_v->actions.OnStopScrolling = [this](int pos,int window_height,int total_height){
                    cam_y = pos;
                    this->InvalidateRect();
                    return pos;
                };
                // 初始停在 -10（构造参数已设 padding 10，此时 SetScrollPos 才能钳到 -10）
                sb_v->SetScrollPos(-10);
            }
        }
        else{
            if(sb_v){
                delete sb_v;
                sb_v = nullptr;
                cam_y = 0;
            }
        }

        if(flags & FGUI_INPUTBOX_FLAG_HORIZONTAL_SCROLL){
            if(!sb_h){
                sb_h = new ScrollBarH(default_rect.w, default_rect.w, {0, 0, default_rect.w, 20}, 10, 10);
                sb_h->actions.OnScrolling = [this](int pos,int window_width,int total_width){
                    cam_x = pos;
                    this->InvalidateRect();
                    return pos;
                };
                sb_h->actions.OnStopScrolling = [this](int pos,int window_width,int total_width){
                    cam_x = pos;
                    this->InvalidateRect();
                    return pos;
                };
                // 初始停在 -10（同垂直滚动条）
                sb_h->SetScrollPos(-10);
            }
        }
        else{
            if(sb_h){
                delete sb_h;
                sb_h = nullptr;
                cam_x = 0;
            }
        }
    }

public:

    struct Actions{
        // 文本内容变更回调（InsertText/DeleteText 触发；SetBuffer/ClearBuffer 属程序化批量操作，不触发）
        std::function<void(InputBox*)> OnChange;
    } actions;

    /**
     * @brief 全选（等价于 Ctrl+A）
     */
    void SelectAll(){
        selection_secondary = 0;
        selection_primary = GetTotalLength();
        this->InvalidateRect();
    }

    /**
     * @brief 获取内容（缓冲）尺寸，供父容器按内容调整滚动区域
     */
    int GetContentWidth() const{ return buff_w; }
    int GetContentHeight() const{ return buff_h; }

    /**
     * @brief 获取光标在缓冲坐标中的纵向位置（供父容器滚动到光标处）
     */
    int GetCaretY() const{
        if(text.empty() || layout.line_infos.empty()) return 0;
        size_t line,col;
        IndexToPos(selection_primary,line,col);
        int line_y = 0;
        size_t li = 0;
        for(; li < line && li < layout.line_infos.size(); li++){
            line_y += layout.line_infos[li].rect.h;
        }
        return line_y;
    }

    /**
     * @brief 获取当前纵向滚动偏移（像素，跟随内部垂直滚动条/光标滚动）
     */
    int GetScrollY() const{ return cam_y; }

    ~InputBox(){
        ClearGlyphCache();
        // delete 空指针是安全的（no-op），这里判空便于阅读且防止重复释放
        if(sb_v) delete sb_v;
        if(sb_h) delete sb_h;
    }

    /**
     * @brief 构造InputBox控件
     * 
     * @param str 默认文本
     * @param _font 字体
     * @param _ptsize 字体大小
     * @param _flags 模式标志位
     * @param rect 范围
     */
	InputBox(const std::string& str, FontEx* _font,int _ptsize,int _flags,const SDL_Rect& rect = {0,0,0,0})
	{
		text = utf8_to_codepoints_lines(str);
        if(text.empty()) text.push_back({});
        font = _font;
        ptsize = _ptsize;
        default_rect = rect;

        buff_w = buff_h = 0;
        cam_x = -10;
        cam_y = 0;
        selection_primary = selection_secondary = 0;
        selection_primary = GetTotalLength();

        flags = _flags;

        focused = false;

        timer_time = 500;

        cursor_blink_time_temp = SDL_GetTicks();

        dragging = false;

        sur_text_buff = NULL;

        sb_v = nullptr;
        sb_h = nullptr;
        show_sb_v = false;
        show_sb_h = false;

        if(default_rect.h == 0){
            default_rect.h = font->GetPrimaryFontHeight(ptsize);
        }

        // 创建滚动条（按需启用）
        RecreateScrollBars();

        UpdateGlyphs();

        this->InvalidateRect();
	}

    /**
     * @brief 修改模式标志位（如滚动条开关），运行时动态创建/销毁滚动条并刷新布局
     * 
     * @param _flags 新的标志位组合
     */
    void SetFlags(int _flags){
        if(flags == _flags) return;
        flags = _flags;
        RecreateScrollBars();
        UpdateGlyphs();
        this->InvalidateRect();
    }

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{

        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        // 刷新滚动条信息（无副作用：仅在内部状态变化时更新）
        UpdateScrollBarInfo();

        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColor));
    SDL_FRect __fr_11 = toFRect(draw_area);
        SDL_RenderFillRect(renderer, &__fr_11);

        // 文本可见区域（扣除滚动条占用的空间）
        SDL_Rect inner = GetInnerTextRect();
        SDL_Rect inner_abs = {draw_area.x + inner.x, draw_area.y + inner.y, inner.w, inner.h};

        if(sur_text_buff){
            SDL_Rect dst = {draw_area.x - cam_x,draw_area.y - cam_y,buff_w,buff_h};
            SDL_Rect adst = RectIntersection(dst,inner_abs);
            if(adst.w > 0 && adst.h > 0){
                // 可见区域在缓冲 surface 中的像素范围
                SDL_Rect src = {adst.x - dst.x,adst.y - dst.y,adst.w,adst.h};

                // 视口（可见区域）远小于渲染器纹理尺寸上限（如 direct3d 为 8192x8192），
                // 因此只需裁剪出可见区域上传为一个小纹理即可。不要对整块缓冲调用
                // SDL_CreateTextureFromSurface——超大缓冲（如超长单行文本 > max_texture_width）
                // 会因超限失败返回 NULL，导致文本整段不显示。
                const int bpp = 4; // sur_text_buff 为 32bpp (ARGB8888)
                SDL_Texture* tex = SDL_CreateTexture(renderer, sur_text_buff->format,SDL_TEXTUREACCESS_STATIC, src.w, src.h);
                if(tex){
                    // SDL_CreateTexture 默认 blend mode 为 NONE，而 SDL_CreateTextureFromSurface
                    // 会自动设为 BLEND。若不显式设置，sur_text_buff 中透明黑色背景像素 (0,0,0,0)
                    // 会被当作不透明黑色直接写入，覆盖编辑区背景 → 文本区显示为纯黑。
                    // 必须设为 BLEND：透明处透出背景、字形不透明像素正常绘制。
                    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
                    Uint8* px = (Uint8*)sur_text_buff->pixels + (size_t)src.y * sur_text_buff->pitch + (size_t)src.x * bpp;
                    SDL_UpdateTexture(tex, NULL, px, sur_text_buff->pitch);
    SDL_FRect __fr_12 = toFRect(adst);
                    SDL_RenderTexture(renderer, tex, NULL, &__fr_12);
                    SDL_DestroyTexture(tex);
                }
            }
        }

        // 绘制选区高亮（覆盖整行高度，保证盖住字形，字形在surface内自带ascent偏移）
        size_t selection_begin,selection_end;
        GetSelectionArea(selection_begin,selection_end);
        int line_y = 0; // 各行在缓冲区中的Y偏移（LineInfo.rect.y均为0，需累加行高）
        for(size_t li = 0; li < layout.line_infos.size(); li++){
            auto& line = layout.line_infos[li];
            for(size_t gi = 0; gi < line.glyphs.size(); gi++){
                auto& g = line.glyphs[gi];
                size_t index = PosToIndex(li,gi);
                SDL_Rect dst = g.rect;
                dst.x += draw_area.x;
                dst.y += draw_area.y + line_y;
                dst.w = g.surface ? g.surface->w : g.rect.w; // 字形实际宽度
                dst.h = line.rect.h;                          // 覆盖整行高度
                dst.x -= cam_x;
                dst.y -= cam_y;
                if(!isRectInRect(dst,inner_abs)){
                    continue;
                }
                if(focused && index >= selection_begin && index < selection_end){
                    SDL_SetRenderDrawColor(renderer,255,255,255,122);
    SDL_FRect __fr_13 = toFRect(dst);
                    SDL_RenderFillRect(renderer, &__fr_13);
                }
            }

            // 特判：空行没有字形，若空行所在索引位置落在选区内，
            // 假装那里有一个空格被选中，让用户能直观看出该空行处于选中状态
            if(text[li].empty()){
                size_t line_pos = GetLineStartIndex(li);
                if(focused && line_pos >= selection_begin && line_pos < selection_end){
                    int space_w = 0, space_h = 0;
                    font->SizeText(ptsize, " ", &space_w, &space_h);
                    space_w *= 2; //加宽一点
                    if(space_w <= 0) space_w = line.rect.h / 2; // 兜底：退化为半个行高
                    SDL_Rect dst = {
                        draw_area.x - cam_x,
                        draw_area.y + line_y - cam_y,
                        space_w,
                        line.rect.h
                    };
                    if(isRectInRect(dst,inner_abs)){
                        SDL_SetRenderDrawColor(renderer,255,255,255,122);
    SDL_FRect __fr_14 = toFRect(dst);
                        SDL_RenderFillRect(renderer, &__fr_14);
                    }
                }
            }

            line_y += line.rect.h;
        }
        
        if(focused && ((SDL_GetTicks() - cursor_blink_time_temp) % 1000) < 500){
            SDL_Rect cursor = GetCursorRenderPos();
            cursor.x += draw_area.x;
            cursor.y += draw_area.y;
            SDL_SetRenderDrawColor(renderer,255,255,255,255);
            SDL_RenderLine(renderer,cursor.x,cursor.y,cursor.x,cursor.y + cursor.h);
        }

        // 绘制滚动条（内容溢出时显示）；InputBox直接渲染滚动条，渲染后清空其脏区
        if(show_sb_v && sb_v){
            sb_v->ActionOnPaint(renderer, dirty_region, GetScrollBarVAbsRect(draw_area));
            sb_v->ClearInvaildRect();
        }
        if(show_sb_h && sb_h){
            sb_h->ActionOnPaint(renderer, dirty_region, GetScrollBarHAbsRect(draw_area));
            sb_h->ClearInvaildRect();
        }

        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_15 = toFRect(draw_area);
        SDL_RenderRect(renderer, &__fr_15);
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        // 事件坐标基于父容器原点（不含子控件偏移），需加上 default_rect.xy 得到控件屏幕矩形
        SDL_Rect draw_area = {relative_rect.x + default_rect.x, relative_rect.y + default_rect.y, default_rect.w, default_rect.h};

        bool on_scrollbar = false;
        if(sb_v || sb_h){
            SDL_Rect svr = GetScrollBarVAbsRect(draw_area);
            SDL_Rect shr = GetScrollBarHAbsRect(draw_area);

            SDL_Point mpt = {-9999,-9999};
            if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN || event->type == SDL_EVENT_MOUSE_BUTTON_UP){
                mpt = {(int)event->button.x, (int)event->button.y};
            }
            else if(event->type == SDL_EVENT_MOUSE_MOTION){
                mpt = {(int)event->motion.x, (int)event->motion.y};
            }
            else if(event->type == SDL_EVENT_MOUSE_WHEEL){
                float mxf, myf;
                SDL_GetMouseState(&mxf, &myf);
                mpt = {(int)mxf, (int)myf};
            }

            if(sb_v && show_sb_v && isPointInsideRect(mpt,svr)) on_scrollbar = true;
            if(sb_h && show_sb_h && isPointInsideRect(mpt,shr)) on_scrollbar = true;

            if(sb_v && show_sb_v) sb_v->MaintainEvent(event, svr);
            if(sb_h && show_sb_h) sb_h->MaintainEvent(event, shr);
        }

        // 滚轮滚动（鼠标位于输入框内时生效）
        if(event->type == SDL_EVENT_MOUSE_WHEEL){
            SDL_Point mpos;
            float mxf, myf;
            SDL_GetMouseState(&mxf, &myf);
            mpos = {(int)mxf, (int)myf};
            if(isPointInsideRect(mpos,draw_area)){
                bool scrolled = false;
                if(sb_v && show_sb_v){
                    sb_v->SetScrollPos(sb_v->GetScrollPos() - 30 * (int)event->wheel.y);
                    cam_y = sb_v->GetScrollPos();
                    scrolled = true;
                }
                if(sb_h && show_sb_h){
                    sb_h->SetScrollPos(sb_h->GetScrollPos() - 30 * (int)event->wheel.x);
                    cam_x = sb_h->GetScrollPos();
                    scrolled = true;
                }
                // 实际滚动了就消费滚轮，阻止父容器同步滚动
                if(scrolled){
                    this->InvalidateRect();
                    wheel_consumed = true;
                }
            }
        }

        if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                SDL_Point pt = RelativizePoint({(int)event->button.x, (int)event->button.y}, relative_rect);
                if (isPointInsideRect(pt, default_rect))
                {
                    if(!on_scrollbar){
                        SetFocus(true);
                        SDL_Rect input_rect = default_rect;
                        input_rect.x += relative_rect.x;
                        input_rect.y += relative_rect.y;
                        if(g_fgui_window){
                            SDL_SetTextInputArea(g_fgui_window, &input_rect, 0);
                            SDL_StartTextInput(g_fgui_window);
                        }
                    }
                }
                else
                {
                    SetFocus(false);
                    //SDL_StopTextInput(g_fgui_window);
                }
            }
        }

        if(!focused) return;

        bool read_only = (flags & FGUI_INPUTBOX_FLAG_READ_ONLY) != 0;
        bool allow_multilines = (flags & FGUI_INPUTBOX_FLAG_ALLOW_MULTILINES) != 0;

        if(event->type == SDL_EVENT_KEY_DOWN){
            if(event->key.scancode == SDL_SCANCODE_LEFT){
                MoveCursorLeft(event->key.mod & SDL_KMOD_SHIFT);
            }
            else if(event->key.scancode == SDL_SCANCODE_RIGHT){
                MoveCursorRight(event->key.mod & SDL_KMOD_SHIFT);
            }
            else if(event->key.scancode == SDL_SCANCODE_UP){
                MoveCursorUp(event->key.mod & SDL_KMOD_SHIFT);
            }
            else if(event->key.scancode == SDL_SCANCODE_DOWN){
                MoveCursorDown(event->key.mod & SDL_KMOD_SHIFT);
            }
            else if(event->key.scancode == SDL_SCANCODE_BACKSPACE){
                if(!read_only) DeleteText();
            }
            else if(event->key.scancode == SDL_SCANCODE_RETURN || event->key.scancode == SDL_SCANCODE_RETURN2){
                if(allow_multilines && !read_only){
                    InsertText("\n");
                }
            }
            else if(event->key.scancode == SDL_SCANCODE_C){
                if(event->key.mod & SDL_KMOD_CTRL){
                    std::string str = GetSelection();
                    if(!str.empty()){
                        SDL_SetClipboardText(str.c_str());
                    }
                }
            }
            else if(event->key.scancode == SDL_SCANCODE_V){
                if((event->key.mod & SDL_KMOD_CTRL) && !read_only){
                    char* str = SDL_GetClipboardText();
                    if(str[0] != '\0'){
                        InsertText(str);
                    }
                    SDL_free(str);
                }
            }
            else if(event->key.scancode == SDL_SCANCODE_X){
                if((event->key.mod & SDL_KMOD_CTRL) && !read_only){
                    std::string str = GetSelection();
                    if(!str.empty()){
                        SDL_SetClipboardText(str.c_str());
                        DeleteText();
                    }
                }
            }
            else if(event->key.scancode == SDL_SCANCODE_A){
                if(event->key.mod & SDL_KMOD_CTRL){
                    selection_secondary = 0;
                    selection_primary = GetTotalLength();
                    this->InvalidateRect();
                }
            }
        }
        else if(event->type == SDL_EVENT_TEXT_INPUT){
            if(!read_only) InsertText(event->text.text);
        }
        else if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
            // 点击滚动条时不移动光标/开始拖选
            if(!on_scrollbar){
                SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
                selection_primary = selection_secondary = PointToChar(pt,default_rect);
                dragging = true;
                UpdateCamPos();
                ResetCursorBlink();
                this->InvalidateRect();
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_MOTION){
            float mxf, myf;
            if(!(SDL_GetMouseState(&mxf,&myf) & SDL_BUTTON_LMASK)){
                dragging = false;
            }
            if(dragging){
                SDL_Point pt = RelativizePoint({(int)event->motion.x,(int)event->motion.y},relative_rect);
                selection_primary = PointToChar(pt,default_rect);
                UpdateCamPos();
                ResetCursorBlink();
                this->InvalidateRect();
            }
        }
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{
        if(focused) this->InvalidateRect();

        // 滚动条悬停/状态变化产生的脏区（InputBox直接渲染滚动条，读取后必须清空其脏区，否则会持续触发重绘）
        if(sb_v && sb_v->GetRelativeInvaildRect().w > 0){
            this->InvalidateRect();
            sb_v->ClearInvaildRect();
        }
        if(sb_h && sb_h->GetRelativeInvaildRect().w > 0){
            this->InvalidateRect();
            sb_h->ClearInvaildRect();
        }

        int old_cx = cam_x, old_cy = cam_y;
        UpdateScrollBarInfo();
        if(cam_x != old_cx || cam_y != old_cy)
            this->InvalidateRect();
    }

    void ActionOnColorKitChange(const Fgui_ColorKit& ck) override{
        color_kit = ck;
        if(sb_v) sb_v->ActionOnColorKitChange(ck);
        if(sb_h) sb_h->ActionOnColorKitChange(ck);
        this->UpdateGlyphs();
        this->InvalidateRect();
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.InputBox";
    }

    void GetSelectionArea(size_t& selection_begin,size_t& selection_end) const{
        if(selection_primary > selection_secondary){
            selection_begin = selection_secondary;
            selection_end = selection_primary;
        }
        else{
            selection_begin = selection_primary;
            selection_end = selection_secondary;
        }
    }

    void InsertText(const std::string& str){
        // 将输入解析为多行（丢弃\r，按\n分行）
        std::vector<std::vector<uint32_t>> lines = utf8_to_codepoints_lines(str);
        if(lines.empty()) return;

        // 数字模式：过滤仅保留数字字符
        if(flags & FGUI_INPUTBOX_FLAG_NUMBER_ONLY){
            for(auto& line : lines){
                std::vector<uint32_t> filtered;
                for(auto cp : line){
                    if(cp >= '0' && cp <= '9'){
                        filtered.push_back(cp);
                    }
                }
                line = std::move(filtered);
            }
            // 移除过滤后为空的整行
            lines.erase(std::remove_if(lines.begin(),lines.end(),[](auto& l){ return l.empty(); }),lines.end());
            if(lines.empty()) return;
        }

        size_t selection_begin,selection_end;
        GetSelectionArea(selection_begin,selection_end);
        size_t sline,scol;
        IndexToPos(selection_begin,sline,scol);
        size_t eline,ecol;
        IndexToPos(selection_end,eline,ecol);

        // 计算插入内容占用的全局长度（行间换行位置 + 每行字符数）
        size_t inserted_len = 0;
        for(auto& l : lines) inserted_len += l.size();
        if(!lines.empty()) inserted_len += lines.size() - 1;

        if(lines.size() == 1){
            // 单行插入：合并到当前行
            std::vector<uint32_t> merged;
            merged.insert(merged.end(),text[sline].begin(),text[sline].begin() + scol);
            merged.insert(merged.end(),lines[0].begin(),lines[0].end());
            merged.insert(merged.end(),text[eline].begin() + ecol,text[eline].end());
            text[sline] = std::move(merged);
            text.erase(text.begin() + sline + 1,text.begin() + eline + 1);
        }
        else{
            // 多行插入：首行并入当前行前段，末行并入选区结束行后段
            std::vector<std::vector<uint32_t>> new_text;
            new_text.insert(new_text.end(),text.begin(),text.begin() + sline);

            std::vector<uint32_t> first;
            first.insert(first.end(),text[sline].begin(),text[sline].begin() + scol);
            first.insert(first.end(),lines[0].begin(),lines[0].end());
            new_text.push_back(std::move(first));

            new_text.insert(new_text.end(),lines.begin() + 1,lines.end() - 1);

            std::vector<uint32_t> last;
            last.insert(last.end(),lines.back().begin(),lines.back().end());
            last.insert(last.end(),text[eline].begin() + ecol,text[eline].end());
            new_text.push_back(std::move(last));

            new_text.insert(new_text.end(),text.begin() + eline + 1,text.end());
            text = std::move(new_text);
        }

        selection_primary = selection_secondary = selection_begin + inserted_len;

        UpdateGlyphs();
        UpdateCamPos();
        ResetCursorBlink();
        this->InvalidateRect();
        CallIfNotNull(actions.OnChange, this);
    }

    void DeleteText(){
        size_t selection_begin,selection_end;
        GetSelectionArea(selection_begin,selection_end);
        size_t sline,scol;
        IndexToPos(selection_begin,sline,scol);
        size_t eline,ecol;
        IndexToPos(selection_end,eline,ecol);

        if(sline == eline && scol == ecol){
            // 无选区：删除光标前一个字符（或前一个换行）
            if(selection_begin == 0){
                ResetCursorBlink();
                this->InvalidateRect();
                return;
            }
            if(scol > 0){
                // 删除本行前一个字符
                text[sline].erase(text[sline].begin() + scol - 1);
            }
            else{
                // 删除前一个换行：合并本行到上一行
                text[sline - 1].insert(text[sline - 1].end(),text[sline].begin(),text[sline].end());
                text.erase(text.begin() + sline);
            }
            selection_primary = selection_secondary = selection_begin - 1;
        }
        else{
            // 删除选区：将选区结束行剩余部分合并到选区开始行
            std::vector<uint32_t> merged;
            merged.insert(merged.end(),text[sline].begin(),text[sline].begin() + scol);
            merged.insert(merged.end(),text[eline].begin() + ecol,text[eline].end());
            text[sline] = std::move(merged);
            text.erase(text.begin() + sline + 1,text.begin() + eline + 1);
            selection_primary = selection_secondary = selection_begin;
        }
        UpdateGlyphs();
        UpdateCamPos();
        ResetCursorBlink();
        this->InvalidateRect();
        CallIfNotNull(actions.OnChange, this);
    }

    void MoveCursorRight(bool shift_pressed){
        if(shift_pressed){
            if(selection_primary < GetTotalLength()){
                selection_primary++;
            }
            UpdateCamPos();
            ResetCursorBlink();
            this->InvalidateRect();
            return;
        }

        size_t selection_begin,selection_end;
        GetSelectionArea(selection_begin,selection_end);
        if(selection_begin != selection_end){
            selection_primary = selection_end;
            selection_secondary = selection_end;
            UpdateCamPos();
            ResetCursorBlink();
            this->InvalidateRect();
            return;
        }

        if(selection_primary < GetTotalLength()){
            selection_primary++;
            selection_secondary++;
        }
        UpdateCamPos();
        ResetCursorBlink();
        this->InvalidateRect();
    }

    void MoveCursorLeft(bool shift_pressed){
        if(shift_pressed){
            if(selection_primary > 0){
                selection_primary--;
            }
            UpdateCamPos();
            ResetCursorBlink();
            this->InvalidateRect();
            return;
        }

        size_t selection_begin,selection_end;
        GetSelectionArea(selection_begin,selection_end);
        if(selection_begin != selection_end){
            selection_primary = selection_begin;
            selection_secondary = selection_begin;
            UpdateCamPos();
            ResetCursorBlink();
            this->InvalidateRect();
            return;
        }

        if(selection_primary > 0){
            selection_primary--;
            selection_secondary--;
        }
        UpdateCamPos();
        ResetCursorBlink();
        this->InvalidateRect();
    }

    void MoveCursorUp(bool shift_pressed){
        if(text.empty() || layout.line_infos.empty()) return;

        size_t cur_line,cur_col;
        IndexToPos(selection_primary,cur_line,cur_col);
        if(cur_line == 0) return; // 已在第一行

        int cursor_layout_x = LayoutXOfPos(cur_line,cur_col);
        size_t new_col = ColumnFromLayoutX(cur_line - 1,cursor_layout_x);
        size_t new_pos = PosToIndex(cur_line - 1,new_col);

        if(shift_pressed) selection_primary = new_pos;
        else selection_primary = selection_secondary = new_pos;
        UpdateCamPos();
        ResetCursorBlink();
        this->InvalidateRect();
    }

    void MoveCursorDown(bool shift_pressed){
        if(text.empty() || layout.line_infos.empty()) return;

        size_t cur_line,cur_col;
        IndexToPos(selection_primary,cur_line,cur_col);
        if(cur_line + 1 >= text.size()) return; // 已在最后一行

        int cursor_layout_x = LayoutXOfPos(cur_line,cur_col);
        size_t new_col = ColumnFromLayoutX(cur_line + 1,cursor_layout_x);
        size_t new_pos = PosToIndex(cur_line + 1,new_col);

        if(shift_pressed) selection_primary = new_pos;
        else selection_primary = selection_secondary = new_pos;
        UpdateCamPos();
        ResetCursorBlink();
        this->InvalidateRect();
    }

    void SetFocus(bool _focused){
        if(focused != _focused){
            focused = _focused;
            ResetCursorBlink();
            this->InvalidateRect();
        }
    }

    bool IsFocused() const{
        return focused;
    }

    /**
     * @brief 获取选区的文本
     * 
     * @param newline 选区跨行时的换行符，默认CRLF
     */
    std::string GetSelection(const std::string& newline = "\r\n") const{
        size_t selection_begin,selection_end;
        GetSelectionArea(selection_begin,selection_end);

        size_t sline,scol;
        IndexToPos(selection_begin,sline,scol);
        size_t eline,ecol;
        IndexToPos(selection_end,eline,ecol);

        if(sline == eline){
            return codepoints_to_utf8(std::vector<uint32_t>(text[sline].begin() + scol,text[sline].begin() + ecol));
        }

        std::string result;
        // 选区起始行剩余部分
        result += codepoints_to_utf8(std::vector<uint32_t>(text[sline].begin() + scol,text[sline].end()));
        // 中间整行
        for(size_t l = sline + 1; l < eline; l++){
            result += newline;
            result += codepoints_to_utf8(text[l]);
        }
        // 选区结束行前缀
        result += newline;
        result += codepoints_to_utf8(std::vector<uint32_t>(text[eline].begin(),text[eline].begin() + ecol));
        return result;
    }

    /**
     * @brief 获取整个输入框的文本
     * 
     * @param newline 行间换行符，默认CRLF
     */
    std::string GetBuffer(const std::string& newline = "\r\n") const{
        return codepoints_lines_to_utf8(text,newline);
    }

    void ClearBuffer(){
        text.clear();
        text.push_back({});
        ClearGlyphCache();
        buff_w = buff_h = 0;
        cam_x = -10;
        cam_y = 0;
        selection_primary = selection_secondary = 0;
        UpdateScrollBarInfo();
    }

    void SetBuffer(const std::string& str){
        ClearBuffer();
        text = utf8_to_codepoints_lines(str);
        if(text.empty()) text.push_back({});
        UpdateGlyphs();
        selection_primary = selection_secondary = GetTotalLength();
        // 重置视图到初始位置：有滚动条时停在 -10（两端各留 10px padding），无滚动条保持左上角
        if(sb_v) sb_v->SetScrollPos(-10);
        if(sb_h) sb_h->SetScrollPos(-10);
        cam_x = -10;
        cam_y = (sb_v || sb_h) ? -10 : 0;
        this->InvalidateRect();
    }
};

/**
 * @brief 复选框，拥有三态(勾选，不勾选，部分勾选)
 * 
 */
class CheckBox : public Fgui_Control
{
public:
    enum CheckBoxState{
        CHECKBOX_STATE_INDETERMINATE = 2,
        CHECKBOX_STATE_CHECKED = 1,
        CHECKBOX_STATE_UNCHECKED = 0,
    };

private:
    CheckBoxState state;

    CheckBoxState last_state;
    uint64_t time_checking;
    bool uflag;

public:

    struct Actions{
        std::function<void(CheckBox*,CheckBoxState)> OnChange;
    } actions;

    /**
     * @brief 构造CheckBox控件
     * 
     * @param rect 范围
     */
	CheckBox(const SDL_Rect& rect = {0,0,0,0})
	{
        default_rect = rect;

        state = CHECKBOX_STATE_UNCHECKED;
        last_state = CHECKBOX_STATE_UNCHECKED;

        time_checking = 0;
        uflag = true;

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{

        if(enable_animation){
            SDL_Rect draw_area = this->default_rect;
            draw_area.x = relative_rect.x;
            draw_area.y = relative_rect.y;

            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColor));
    SDL_FRect __fr_16 = toFRect(draw_area);
            SDL_RenderFillRect(renderer, &__fr_16);

            double process = 0;

            if(last_state == CHECKBOX_STATE_UNCHECKED){
                process = BezierCurve(beforeNumber(time_checking,SDL_GetTicks(),100),BC_EASE_IN_OUT);
            }
            else{
                process = BezierCurve(1.0 - beforeNumber(time_checking,SDL_GetTicks(),100),BC_EASE_IN_OUT);
            }

            paintFilledCircle(renderer,{color_kit.ForegroundColor.r,color_kit.ForegroundColor.g,color_kit.ForegroundColor.b,(uint8_t)(process * 255.0)},{draw_area.w / 2,draw_area.h / 2},process * hypot(double(draw_area.w / 2),double(draw_area.h / 2)));

            if(state == CHECKBOX_STATE_CHECKED || last_state == CHECKBOX_STATE_CHECKED){
                SDL_Rect _temp = ShrinkRect(draw_area,0.6);
                static const SDL_Point offsets[8] = {
                    {0,1},{0,-1},{1,0},{-1,0},
                    {1,1},{-1,-1},{1,-1},{-1,1},
                };
                for(int i = 0;i < 8;i++){
                    SDL_Rect temp = {_temp.x + offsets[i].x,_temp.y + offsets[i].y,_temp.w,_temp.h};
                    SDL_Point pts[3] = {
                        {temp.x,temp.y + temp.h / 2},
                        {temp.x + temp.w * 0.3f,temp.y + temp.h},
                        {temp.x + temp.w,temp.y}
                    };
                    SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.TextColor));
                    paintWritingLines(renderer,pts,3,process);
                }
            }
            else if(state == CHECKBOX_STATE_INDETERMINATE || last_state == CHECKBOX_STATE_INDETERMINATE){
                SDL_Rect _temp = ShrinkRect(draw_area,0.6);
                static const SDL_Point offsets[8] = {
                    {0,1},{0,-1},{1,0},{-1,0},
                    {1,1},{-1,-1},{1,-1},{-1,1},
                };
                for(int i = 0;i < 8;i++){
                    SDL_Rect temp = {_temp.x + offsets[i].x,_temp.y + offsets[i].y,_temp.w,_temp.h};
                    SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.TextColor));
                    SDL_Point pts[2] = {
                        {temp.x,temp.y + temp.h / 2},
                        {temp.x + temp.w,temp.y + temp.h / 2}
                    };
                    paintWritingLines(renderer,pts,2,process);
                }
            }

            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_17 = toFRect(draw_area);
            SDL_RenderRect(renderer, &__fr_17);
            return;
        }

        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        if(state == CHECKBOX_STATE_UNCHECKED){
            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColor));
        }
        else{
            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.ForegroundColor));
        }
    SDL_FRect __fr_18 = toFRect(draw_area);
		SDL_RenderFillRect(renderer, &__fr_18);

        if(state == CHECKBOX_STATE_CHECKED){
            SDL_Rect _temp = ShrinkRect(draw_area,0.6);
            static const SDL_Point offsets[8] = {
                {0,1},{0,-1},{1,0},{-1,0},
                {1,1},{-1,-1},{1,-1},{-1,1},
            };
            for(int i = 0;i < 8;i++){
                SDL_Rect temp = {_temp.x + offsets[i].x,_temp.y + offsets[i].y,_temp.w,_temp.h};
                SDL_FPoint pts[3] = {
                    {(float)temp.x,(float)(temp.y + temp.h / 2)},
                    {(float)(temp.x + temp.w * 0.3f),(float)(temp.y + temp.h)},
                    {(float)(temp.x + temp.w),(float)temp.y}
                };
                SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.TextColor));
                SDL_RenderLines(renderer,pts,3);
            }
        }
        else if(state == CHECKBOX_STATE_INDETERMINATE){
            SDL_Rect _temp = ShrinkRect(draw_area,0.6);
            static const SDL_Point offsets[8] = {
                {0,1},{0,-1},{1,0},{-1,0},
                {1,1},{-1,-1},{1,-1},{-1,1},
            };
            for(int i = 0;i < 8;i++){
                SDL_Rect temp = {_temp.x + offsets[i].x,_temp.y + offsets[i].y,_temp.w,_temp.h};
                SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.TextColor));
                SDL_RenderLine(renderer,temp.x,temp.y + temp.h / 2,temp.x + temp.w,temp.y + temp.h / 2);
            }
        }

		SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_19 = toFRect(draw_area);
		SDL_RenderRect(renderer, &__fr_19);
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        if(event->type == SDL_EVENT_MOUSE_BUTTON_UP){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    last_state = state;
                    if(state == CHECKBOX_STATE_CHECKED){
                        state = CHECKBOX_STATE_UNCHECKED;
                    }
                    else if(state == CHECKBOX_STATE_UNCHECKED){
                        state = CHECKBOX_STATE_CHECKED;
                    }
                    else if(state == CHECKBOX_STATE_INDETERMINATE){
                        state = CHECKBOX_STATE_UNCHECKED;
                    }
                    CallIfNotNull(actions.OnChange,this,state);
                    time_checking = SDL_GetTicks();
                    uflag = false;
                    this->InvalidateRect();
                }
            }
        }
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{
        if(enable_animation){
            if(SDL_GetTicks() - time_checking <= 100){
                if(!uflag){
                    this->InvalidateRect();
                }
            }
            else{
                if(!uflag){
                    uflag = true;
                    this->InvalidateRect();
                }
            }
        }
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.CheckBox";
    }

    //使用成员函数修改状态不会触发回调
    void SetState(CheckBoxState _state,bool animated = false){
        last_state = state;
        state = _state;
        if(animated){
            time_checking = SDL_GetTicks();
            uflag = false;
        }
        else{
            time_checking = SDL_GetTicks() - 100;
            uflag = true;
        }
        this->InvalidateRect();
    }
};

/**
 * @brief 开关，很好理解吧
 * 
 */
class ToggleButton : public Fgui_Control{
    bool state;

    uint64_t time_switching;
    bool uflag;

    public:

    struct Actions{
        std::function<void(ToggleButton*,bool)> OnChange;
    } actions;

    /**
     * @brief 构造ToggleButton控件
     * 
     * @param rect 范围
     */
    ToggleButton(bool default_state,const SDL_Rect& rect = {0,0,0,0})
	{
        default_rect = rect;

        state = default_state;

        time_switching = 0;
        uflag = true;

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{

        if(enable_animation){
            SDL_Rect draw_area = this->default_rect;
            draw_area.x = relative_rect.x;
            draw_area.y = relative_rect.y;

            SDL_Rect temp = ShrinkRect(draw_area,0.75,0.5);

            double progress = BezierCurve(beforeNumber(time_switching,SDL_GetTicks(),100),BC_EASE_IN_OUT);
            if(!state){
                progress = 1 - progress;
            }

            paintRoundedRect(renderer,temp,color_kit.BorderColor,ColorInterval(color_kit.BackgroundColor,color_kit.ForegroundColor,progress),INT_MAX);

            SDL_Point center_circle = {draw_area.x + draw_area.w * 0.125 + temp.h / 2 + (temp.w - temp.h) * progress,draw_area.y + draw_area.h / 2};

            paintCircleEx(renderer,color_kit.BorderColor,color_kit.TextColor,center_circle,std::min(draw_area.w,draw_area.h) * 0.35);
            
            return;
        }

        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        SDL_Rect temp = ShrinkRect(draw_area,0.75,0.5);

        if(state){
            paintRoundedRect(renderer,temp,color_kit.BorderColor,color_kit.ForegroundColor,INT_MAX);
            SDL_Point center_circle = {draw_area.x + draw_area.w * 0.875 - temp.h / 2,draw_area.y + draw_area.h / 2};
            paintCircleEx(renderer,color_kit.BorderColor,color_kit.TextColor,center_circle,std::min(draw_area.w,draw_area.h) * 0.35);
        }
        else{
            paintRoundedRect(renderer,temp,color_kit.BorderColor,color_kit.BackgroundColor,INT_MAX);
            SDL_Point center_circle = {draw_area.x + draw_area.w * 0.125 + temp.h / 2,draw_area.y + draw_area.h / 2};
            paintCircleEx(renderer,color_kit.BorderColor,color_kit.TextColor,center_circle,std::min(draw_area.w,draw_area.h) * 0.35);
        }

		// SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_20 = toFRect(draw_area);
		// SDL_RenderRect(renderer, &__fr_20);
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        if(event->type == SDL_EVENT_MOUSE_BUTTON_UP){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    state = !state;
                    CallIfNotNull(actions.OnChange,this,state);
                    time_switching = SDL_GetTicks();
                    uflag = false;
                    this->InvalidateRect();
                }
            }
        }
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{
        if(enable_animation){
            if(SDL_GetTicks() - time_switching <= 100){
                if(!uflag){
                    this->InvalidateRect();
                }
            }
            else{
                if(!uflag){
                    uflag = true;
                    this->InvalidateRect();
                }
            }
        }
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.ToggleButton";
    }

    //使用成员函数修改状态不会触发回调
    void SetState(bool _state,bool animated = false){
        state = _state;
        if(animated){
            time_switching = SDL_GetTicks();
            uflag = false;
        }
        else{
            time_switching = SDL_GetTicks() - 100;
            uflag = true;
        }
        this->InvalidateRect();
    }
};

/**
 * @brief 单选按钮，与同ControlBox内的其他单选按钮互斥
 * 
 */
class RadioButton : public Fgui_Control
{

private:

    bool state;

    uint64_t time_checking;
    bool uflag;

public:

    struct Actions{
        std::function<void(RadioButton*)> OnChange;
    } actions;

    /**
     * @brief 构造RadioButton控件
     * 
     * @param rect 范围
     */
	RadioButton(const SDL_Rect& rect = {0,0,0,0})
	{
        default_rect = rect;

        state = false;

        time_checking = 0;
        uflag = true;

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{

        if(enable_animation){
            SDL_Rect draw_area = this->default_rect;
            draw_area.x = relative_rect.x;
            draw_area.y = relative_rect.y;

            double progress = BezierCurve(beforeNumber(time_checking,SDL_GetTicks(),100),BC_EASE_IN_OUT);
            if(!state){
                progress = 1 - progress;
            }

            SDL_Point center_circle = {draw_area.x + draw_area.w / 2,draw_area.y + draw_area.h / 2};

            paintCircleEx(renderer,color_kit.BorderColor,ColorInterval(color_kit.BackgroundColor,color_kit.ForegroundColor,progress),center_circle,std::min(draw_area.w,draw_area.h) / 2);

            if(progress != 0){
                paintCircleEx(renderer,color_kit.ForegroundColor,color_kit.TextColor,center_circle,std::min(draw_area.w,draw_area.h) * 0.3f* progress);
            }

            return;
        }

        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        SDL_Point center_circle = {draw_area.x + draw_area.w / 2,draw_area.y + draw_area.h / 2};

        paintCircleEx(renderer,color_kit.BorderColor,state ? color_kit.ForegroundColor : color_kit.BackgroundColor,center_circle,std::min(draw_area.w,draw_area.h) / 2);

        if(state){
            paintCircleEx(renderer,color_kit.ForegroundColor,color_kit.TextColor,center_circle,std::min(draw_area.w,draw_area.h) * 0.3f);
        }
    }

    #define SDL_USEREVENT_CODE_RADIOBUTTON_REPEL 121001

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        if(event->type == SDL_EVENT_MOUSE_BUTTON_UP){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    if(!state){
                        state = true;
                        CallIfNotNull(actions.OnChange,this);
                        time_checking = SDL_GetTicks();
                        uflag = false;
                        this->InvalidateRect();
                        if(this->parent){
                            SDL_Event event = {0};
                            event.type = SDL_EVENT_USER;
                            event.user.timestamp = SDL_GetTicksNS();
                            event.user.code = SDL_USEREVENT_CODE_RADIOBUTTON_REPEL;
                            this->parent->Broadcast(&event,this);
                        }
                    }
                }
            }
        }
        else if(event->type == SDL_EVENT_USER){
            if(event->user.code == SDL_USEREVENT_CODE_RADIOBUTTON_REPEL){
                if(state){
                    this->SetState(false,true);
                }
            }
        }
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{
        if(enable_animation){
            if(SDL_GetTicks() - time_checking <= 100){
                if(!uflag){
                    this->InvalidateRect();
                }
            }
            else{
                if(!uflag){
                    uflag = true;
                    this->InvalidateRect();
                }
            }
        }
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.RadioButton";
    }

    //使用成员函数修改状态不会触发回调
    void SetState(bool _state,bool animated = false){
        state = _state;
        if(animated){
            time_checking = SDL_GetTicks();
            uflag = false;
        }
        else{
            time_checking = SDL_GetTicks() - 100;
            uflag = true;
        }
        this->InvalidateRect();
    }
};

/**
 * @brief 图片框
 * 
 */
class PictureBox : public Fgui_Control
{
public:

    enum PictureFillingMethod : int{
        //在保持图片原始宽高比的前提下，将图片整体缩放至刚刚好符合图片框的大小
        PICTURE_FILLING_METHOD_FIT = 1,
        //忽视图片原始宽高比，将图片填满整个图片框
        PICTURE_FILLING_METHOD_STRETCH = 2,
        //在保持图片原始宽高比的前提下，将图片整体缩放至刚刚好填满图片框的大小（可能会使图片显示不全）
        PICTURE_FILLING_METHOD_FILL = 3,
        //当图片尺寸小于图片框时，将图片像瓷砖一样重复复制，铺满整个图片框
        PICTURE_FILLING_METHOD_TILE = 4,
        //将图片保持原始尺寸放在容器的正中央
        PICTURE_FILLING_METHOD_CENTER = 5,
    };


private:

    PictureFillingMethod filling_method;
    SDL_Surface* image;

public:

    struct Actions{
        std::function<void(PictureBox *)> OnClick;
    } actions;

    ~PictureBox(){
        if(image){
            SDL_DestroySurface(image);
        }
    }

    void SetImage(SDL_Surface* _image = NULL){
        if(image) SDL_DestroySurface(image);
        if(_image) image = SDL_DuplicateSurface(_image);
        else image = nullptr;
        this->InvalidateRect();
    }

    void SetFillingMethod(PictureFillingMethod _filling_method){
        if(filling_method != _filling_method){
            filling_method = _filling_method;
            this->InvalidateRect();
        }
    }

    /**
     * @brief 构造PictureBox控件
     * 
     * @param rect 范围
     * @param _image 显示的图片，内部会复制传入的图片，所以无需担心传入SDL_Surface的生命周期
     * @param _filling_method 图片的填充方式
     */
	PictureBox(const SDL_Rect& rect = {0,0,0,0},SDL_Surface* _image = NULL,PictureFillingMethod _filling_method = PictureFillingMethod::PICTURE_FILLING_METHOD_FIT)
	{
		if(_image) image = SDL_DuplicateSurface(_image);
        else image = nullptr;

        filling_method = _filling_method;

        default_rect = rect;

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{
        if(!image) return;
        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,image);

        float fw, fh;
        SDL_GetTextureSize(texture, &fw, &fh);
        int img_w = (int)fw, img_h = (int)fh;

        int box_x = draw_area.x;
        int box_y = draw_area.y;
        int box_w = draw_area.w;
        int box_h = draw_area.h;

        SDL_Rect src_rect, dst_rect;

        switch (filling_method)
        {
            case PICTURE_FILLING_METHOD_FIT:
            {
                float scale = fmin((float)box_w / img_w, (float)box_h / img_h);
                int fit_w = (int)(img_w * scale);
                int fit_h = (int)(img_h * scale);
                dst_rect.x = box_x + (box_w - fit_w) / 2;
                dst_rect.y = box_y + (box_h - fit_h) / 2;
                dst_rect.w = fit_w;
                dst_rect.h = fit_h;
                src_rect = {0, 0, img_w, img_h};
    SDL_FRect __fr_21 = toFRect(src_rect);
    SDL_FRect __fr_22 = toFRect(dst_rect);
                SDL_RenderTexture(renderer, texture, &__fr_21, &__fr_22);
            }
            break;

            case PICTURE_FILLING_METHOD_STRETCH:
            {
                dst_rect = default_rect;
                src_rect = {0, 0, img_w, img_h};
    SDL_FRect __fr_23 = toFRect(src_rect);
    SDL_FRect __fr_24 = toFRect(dst_rect);
                SDL_RenderTexture(renderer, texture, &__fr_23, &__fr_24);
            }
            break;

            case PICTURE_FILLING_METHOD_FILL:
            {
                if (img_w * box_h > box_w * img_h)
                {
                    src_rect.w = (img_h * box_w) / box_h;
                    src_rect.h = img_h;
                    src_rect.x = (img_w - src_rect.w) / 2;
                    src_rect.y = 0;
                }
                else
                {
                    src_rect.h = (img_w * box_h) / box_w;
                    src_rect.w = img_w;
                    src_rect.x = 0;
                    src_rect.y = (img_h - src_rect.h) / 2;
                }
                dst_rect = default_rect;
    SDL_FRect __fr_25 = toFRect(src_rect);
    SDL_FRect __fr_26 = toFRect(dst_rect);
                SDL_RenderTexture(renderer, texture, &__fr_25, &__fr_26);
            }
            break;

            case PICTURE_FILLING_METHOD_TILE:
            {
                for (int y = box_y; y < box_y + box_h; y += img_h)
                {
                    for (int x = box_x; x < box_x + box_w; x += img_w)
                    {
                        dst_rect.x = x;
                        dst_rect.y = y;
                        dst_rect.w = img_w;
                        dst_rect.h = img_h;
                        src_rect = {0, 0, img_w, img_h};
    SDL_FRect __fr_27 = toFRect(src_rect);
    SDL_FRect __fr_28 = toFRect(dst_rect);
                        SDL_RenderTexture(renderer, texture, &__fr_27, &__fr_28);
                    }
                }
            }
            break;

            case PICTURE_FILLING_METHOD_CENTER:
            {
                if (img_w <= box_w && img_h <= box_h)
                {
                    dst_rect.x = box_x + (box_w - img_w) / 2;
                    dst_rect.y = box_y + (box_h - img_h) / 2;
                    dst_rect.w = img_w;
                    dst_rect.h = img_h;
                    src_rect = {0, 0, img_w, img_h};
                }
                else
                {
                    dst_rect = default_rect;
                    src_rect.x = (img_w - box_w) / 2;
                    src_rect.y = (img_h - box_h) / 2;
                    src_rect.w = box_w;
                    src_rect.h = box_h;
                    if (src_rect.x < 0)
                        src_rect.x = 0;
                    if (src_rect.y < 0)
                        src_rect.y = 0;
                    if (src_rect.x + src_rect.w > img_w)
                        src_rect.w = img_w - src_rect.x;
                    if (src_rect.y + src_rect.h > img_h)
                        src_rect.h = img_h - src_rect.y;
                }
    SDL_FRect __fr_29 = toFRect(src_rect);
    SDL_FRect __fr_30 = toFRect(dst_rect);
                SDL_RenderTexture(renderer, texture, &__fr_29, &__fr_30);
            }
            break;
        }
        SDL_DestroyTexture(texture);
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{

    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.PictureBox";
    }
};

/**
 * @brief 滑动条，用于数值调节
 * 
 */
class Slider : public Fgui_Control{

    float min_val;
    float max_val;

    float current_val;

    EMASmoother ema_val;

    bool is_dragging;

    public:

    struct Actions{
        std::function<void(Slider*,float)> OnBeginDragging;
        std::function<void(Slider*,float)> OnDragging;
        std::function<void(Slider*,float)> OnEndDragging;
    } actions;

    /**
     * @brief 构造Slider控件
     * 
     * @param rect 范围
     * @param min 最小值
     * @param max 最大值
     * @param default_val 默认值
     */
    Slider(float min = 0.0f,
           float max = 100.0f,
           float default_val = 0.0f,
           const SDL_Rect& rect = {0,0,0,0})
	{
        default_rect = rect;

        // 初始化数值
        min_val = min;
        max_val = max;
        // 限制默认值在合法范围内
        current_val = std::clamp(default_val, min, max);
        ema_val.setLastValue(current_val);

        is_dragging = false;

        ema_val.setAlpha(0.3);

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{
        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        // 收缩绘制区域（和ToggleButton同比例）
        SDL_Rect track_rect = ShrinkRect(draw_area, 0.95, 0.4);
        // 计算滑动比例 0~1
        
        float actual_val = current_val;

        if(enable_animation){
            actual_val = ema_val.getLastValue();
        }

        float progress = (actual_val - min_val) / (max_val - min_val);
        progress = std::clamp(progress, 0.0f, 1.0f);

        // 1. 绘制滑动条背景
        paintRoundedRect(renderer, track_rect, color_kit.BorderColor, color_kit.BackgroundColor, INT_MAX);
        
        // 2. 绘制已填充进度条
        SDL_Rect fill_rect = track_rect;
        fill_rect.w = fill_rect.w * progress;
        paintRoundedRect(renderer, fill_rect, color_kit.BorderColor, color_kit.ForegroundColor, INT_MAX);

        int slider_radius = std::min(draw_area.w, draw_area.h) * 0.25f;
        SDL_Point center_circle = {
            (int)(track_rect.x + track_rect.w * progress),
            draw_area.y + draw_area.h / 2
        };
        paintCircleEx(renderer, color_kit.BorderColor, color_kit.TextColor, center_circle, slider_radius);
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        
        // 鼠标左键按下：开始拖拽
        if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT){
            SDL_Point pt = RelativizePoint({(int)event->button.x, (int)event->button.y}, relative_rect);
            if(isPointInsideRect(pt, default_rect)){
                is_dragging = true;
                UpdateValueByMousePos(pt);
                CallIfNotNull(actions.OnBeginDragging,this,current_val);
            }
        }

        // 鼠标移动：拖拽中更新数值
        if(event->type == SDL_EVENT_MOUSE_MOTION && is_dragging){
            SDL_Point pt = RelativizePoint({(int)event->motion.x, (int)event->motion.y}, relative_rect);
            float old_val = current_val;
            UpdateValueByMousePos(pt);
            if(current_val != old_val){
                CallIfNotNull(actions.OnDragging,this,current_val);
            }
        }

        // 鼠标左键抬起：结束拖拽
        if(event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT){
            if(is_dragging){
                SDL_Point pt = RelativizePoint({(int)event->button.x, (int)event->button.y}, relative_rect);
                UpdateValueByMousePos(pt);
                CallIfNotNull(actions.OnEndDragging,this,current_val);
                is_dragging = false;
            }
        }
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{
        // 动画刷新逻辑（和ToggleButton完全一致）
        if(enable_animation){
            if(abs(ema_val.process(current_val) - current_val) / (this->max_val - this->min_val) * this->default_rect.w > 1){
                this->InvalidateRect();
            }
        }
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.Slider";
    }

    /**
     * @brief 根据鼠标位置计算并更新数值
     * @param pt 鼠标相对坐标
     */
    void UpdateValueByMousePos(const SDL_Point& pt){
        SDL_Rect inner = ShrinkRect(default_rect,0.95,0.4);
        float ratio = (float)(pt.x - inner.x) / inner.w;
        ratio = std::clamp(ratio, 0.0f, 1.0f);
        // 计算新数值
        float new_val = min_val + ratio * (max_val - min_val);
        
        // 数值变化才更新
        if(current_val != new_val){
            current_val = new_val;
            if(!enable_animation){
                ema_val.setLastValue(current_val);
            }
            this->InvalidateRect();
        }
    }

    void SetValue(float _value, bool animated = false){
        // 限制数值范围
        current_val = std::clamp(_value, min_val, max_val);
        ema_val.setLastValue(current_val);
        this->InvalidateRect();
    }

    float GetValue() const{
        return current_val;
    }

    void SetRange(float min, float max){
        min_val = min;
        max_val = max;
        current_val = std::clamp(current_val, min, max);
        ema_val.setLastValue(current_val);
        this->InvalidateRect();
    }

    float GetMinValue() const { return min_val; }
    float GetMaxValue() const { return max_val; }

    void InvalidateRect() override{
        this->InvalidateRect(SDL_Rect{-10,0,default_rect.w + 20,default_rect.h});
    }

    void InvalidateRect(const SDL_Rect& rect) override{
        dirty_region = ExtendRect(dirty_region,rect);
    }
};

/**
 * @brief 组合框(下拉框)控件
 * 
 */
class ComboBox : public Fgui_Control
{
    public:
    struct ComboItem{
        //The UI string to be show for the item
        std::string ui_string;


        //The ID of the item. Avoid use value (int)0xFFFFFFFF,because it's reserved for unselected state.
        int id;


        //Determine the priority for arranging items in the list to be placed at the front. Default to 0. Bigger value is prior.
        int priority;
    };
    protected:

    FontEx* font;
    int ptsize;
    std::string default_text;
	std::vector<ComboItem> items;

    int id_item_chosen;

    //For animation
    EMAColor bg_color;

    bool enable;

    bool _pressing;
    bool _hovered;

    //---------- 下拉列表 ----------
    PopupWindow tooltip;
    // 父渲染器（在 ActionOnPaint 中捕获，用于打开下拉子窗口）
    SDL_Renderer* parent_renderer = nullptr;
    // 下拉列表当前是否可见
    bool dropdown_open = false;
    // 延迟打开标记（事件回调中拿不到渲染器，待定时器帧打开）
    bool want_open = false;
    // 下拉列表滚动偏移（行）
    int scroll_offset = 0;
    // 下拉列表在父窗口客户区中的位置与尺寸
    SDL_Rect dropdown_rect = {0,0,0,0};
    // 控件在父窗口客户区中的屏幕矩形（由事件回调按滚动偏移计算，供定时器帧打开下拉框使用）
    SDL_Rect combo_screen_rect = {0,0,0,0};
    // 下拉框在屏幕上的物理矩形（顶层浮动弹窗定位，用于点击外部判定）
    SDL_Rect dropdown_screen_rect = {0,0,0,0};
    // 父窗口客户区原点在屏幕上的物理坐标（点击换算用）
    SDL_Point parent_client_origin = {0,0};
    // 下拉框使用的 DPI 缩放
    float combo_dpi_scale = 1.0f;
    // 下拉列表最多同时显示的条目数
    static constexpr int MAX_VISIBLE_ITEMS = 8;
    // 滚动条滑块宽度与右内边距（右对齐于下拉框右缘）
    static constexpr int SCROLLBAR_THUMB_WIDTH = 8;
    // 滚动条热区（轨道）宽度——与滑块同宽，保证轨道与滑块视觉宽度一致
    static constexpr int SCROLLBAR_HOT_WIDTH = SCROLLBAR_THUMB_WIDTH;
    // 滚动条右缘距下拉框右缘的距离（留 1px 边框余量）
    static constexpr int SCROLLBAR_HOT_RIGHT = 1;
    // ---------- 下拉框滚动条拖拽状态 ----------
    bool scrollbar_dragging = false;
    // 上一帧左键是否按下（用于检测按下沿）
    bool scrollbar_last_btn_down = false;
    // 抓取点相对滑块顶部的偏移（拖拽时保持）
    int scrollbar_grab_y = 0;

public:

    struct Actions{
        std::function<void(ComboBox *,const ComboItem&)> OnChange;
    } actions;

    ComboItem GetSelectedItem() const{
        for(auto& i : items){
            if(i.id == id_item_chosen){
                return i;
            }
        }
        return {default_text,(int)0xFFFFFFFF,0};
    }

protected:

    //---------- 下拉列表辅助 ----------

    // 条目高度
    int GetItemHeight() const{
        int fh = font ? font->GetPrimaryFontHeight(ptsize) : 0;
        return std::max(default_rect.h, fh + 12);
    }

    // 下拉列表宽度：取控件宽度与最长条目文本宽度中的较大值
    int CalcDropdownWidth() const{
        int max_text_w = 0;
        if(font){
            for(auto& it : items){
                int w = 0, h = 0;
                font->SizeText(ptsize, it.ui_string, &w, &h);
                max_text_w = std::max(max_text_w, w);
            }
        }
        return std::max(default_rect.w, max_text_w + 24);
    }

    // 稳定排序：priority 数值越大越靠前
    static void SortItems(std::vector<ComboItem>& vec){
        std::stable_sort(vec.begin(), vec.end(), [](const ComboItem& a, const ComboItem& b){
            return a.priority > b.priority;
        });
    }

    // 返回下拉框中实际应显示的条目下标（排除当前已选中的条目）
    std::vector<int> GetVisibleItemIndices() const{
        std::vector<int> idxs;
        idxs.reserve(items.size());
        for(size_t i = 0; i < items.size(); i++){
            if((int)items[i].id != id_item_chosen){
                idxs.push_back((int)i);
            }
        }
        return idxs;
    }

    // 滚动条是否可见（条目溢出时）
    bool IsScrollBarVisible() const{
        return (int)GetVisibleItemIndices().size() > MAX_VISIBLE_ITEMS;
    }

    // 滚动条滑块（可视指示条）几何，右对齐于下拉框右缘，下拉框逻辑坐标
    SDL_Rect GetScrollBarThumbRect(const SDL_Rect& size) const{
        const int pad = 4;
        std::vector<int> vis = GetVisibleItemIndices();
        int n = std::max(1,(int)vis.size());
        int track_h = size.h - pad * 2;
        int bar_h = std::max(16, track_h * MAX_VISIBLE_ITEMS / n);
        int max_scroll = std::max(0, (int)vis.size() - MAX_VISIBLE_ITEMS);
        int bar_y = (max_scroll > 0) ? (track_h - bar_h) * scroll_offset / max_scroll : 0;
        int x = size.w - SCROLLBAR_HOT_RIGHT - SCROLLBAR_THUMB_WIDTH;
        return {x, pad + bar_y, SCROLLBAR_THUMB_WIDTH, bar_h};
    }

    // 滚动条可交互热区（比滑块稍宽，便于抓取与点轨道跳转），下拉框逻辑坐标
    SDL_Rect GetScrollBarHotRect(const SDL_Rect& size) const{
        const int pad = 4;
        int track_h = size.h - pad * 2;
        return {size.w - SCROLLBAR_HOT_RIGHT - SCROLLBAR_HOT_WIDTH, pad, SCROLLBAR_HOT_WIDTH, track_h};
    }

    // 由鼠标位置返回命中的条目下标，未命中返回 -1
    int HitTestItem(const SDL_Point& pos) const{
        if(items.empty()) return -1;
        std::vector<int> vis = GetVisibleItemIndices();
        if(vis.empty()) return -1;
        const int pad = 4;
        int ih = GetItemHeight();
        int visible = std::min((int)vis.size() - scroll_offset, (int)MAX_VISIBLE_ITEMS);
        if(visible < 0) visible = 0;
        int right_limit = dropdown_rect.w - pad;
        if(IsScrollBarVisible()) right_limit = dropdown_rect.w - SCROLLBAR_HOT_RIGHT - SCROLLBAR_HOT_WIDTH;
        if(pos.x < pad || pos.x >= right_limit) return -1;
        int row = (pos.y - pad) / ih;
        if(row < 0 || row >= visible) return -1;
        return vis[scroll_offset + row];
    }

    // 计算条目在下拉列表客户区中的矩形
    SDL_Rect GetItemRect(int index) const{
        const int pad = 4;
        int ih = GetItemHeight();
        return {pad, pad + (index - scroll_offset) * ih, dropdown_rect.w - pad * 2, ih};
    }

    // 计算右侧指示符的尺寸与右内边距（随控件大小自动缩放）
    void CalcIndicatorMetrics(const SDL_Rect& draw_area,int& ind_w,int& ind_t,int& pad) const{
        // 长度：控件高约30%，限 8~24；厚度：控件高约8%，限 2~5
        ind_w = std::clamp((int)(draw_area.h * 0.30f), 8, 24);
        ind_t = std::clamp((int)(draw_area.h * 0.04f), 2, 5);
        // 右内边距：指示符右端到控件右缘至少留一个"长度+6px"
        pad = ind_w + 6;
    }

    // 绘制右侧下拉指示符（加号/减号），右对齐并留一定Padding，垂直居中
    void PaintDropdownIndicator(SDL_Renderer* renderer,const SDL_Rect& draw_area){
        int ind_w, ind_t, pad;
        CalcIndicatorMetrics(draw_area, ind_w, ind_t, pad);
        int cx = draw_area.x + draw_area.w - pad - ind_w / 2; // 指示符中心X
        int cy = draw_area.y + draw_area.h / 2;               // 指示符中心Y（垂直居中）
        SDL_Color ind_color = enable ? color_kit.TextColor : color_kit.ForegroundColor;
        SDL_SetRenderDrawColor(renderer, ColorArg(ind_color));
        SDL_Rect hbar = {cx - ind_w / 2 + pad / 2, cy - ind_t / 2, ind_w, ind_t};
        if(dropdown_open){
            
    SDL_FRect __fr_31 = toFRect(hbar);
            SDL_RenderFillRect(renderer, &__fr_31);
        }
        else{
            
            SDL_Rect vbar = {cx - ind_t / 2 + pad / 2, cy - ind_w / 2, ind_t, ind_w};
    SDL_FRect __fr_32 = toFRect(hbar);
            SDL_RenderFillRect(renderer, &__fr_32);
    SDL_FRect __fr_33 = toFRect(vbar);
            SDL_RenderFillRect(renderer, &__fr_33);
        }
    }

    // 绘制左侧选中文本，裁剪到不进入右侧指示符区域，防止文本与加减号重叠
    void PaintSelectedText(SDL_Renderer* renderer,const SDL_Rect& draw_area){
        int ind_w, ind_t, pad;
        CalcIndicatorMetrics(draw_area, ind_w, ind_t, pad);
        // 文本可绘制区域：左侧8px内边距起，到指示符左缘为止
        int indicator_left = draw_area.x + draw_area.w - pad - ind_w; // 指示符最左侧X
        int text_left = draw_area.x + 8;
        SDL_Rect text_clip = {text_left, draw_area.y, indicator_left - text_left, draw_area.h};
        if(text_clip.w <= 0 || text_clip.h <= 0) return;

        // 保存旧裁剪并设置新裁剪（与现有裁剪求交集）
        SDL_Rect old_clip;
        SDL_GetRenderClipRect(renderer, &old_clip);
        SDL_Rect new_clip = SDL_RectEmpty(&old_clip) ? text_clip : RectIntersection(old_clip, text_clip);
        SDL_SetRenderClipRect(renderer, &new_clip);

        font->paintText_Blended(renderer, text_left, draw_area.y + draw_area.h / 2, ptsize,
            GetSelectedItem().ui_string, enable ? color_kit.TextColor : color_kit.ForegroundColor, {0.0f, 0.5f});

        // 恢复旧裁剪
        if(SDL_RectEmpty(&old_clip)) SDL_SetRenderClipRect(renderer, NULL);
        else SDL_SetRenderClipRect(renderer, &old_clip);
    }

    void OpenDropdown(SDL_Renderer* renderer,const SDL_Rect& combo_screen){
        if(!font || items.empty()) return;
        parent_renderer = renderer;

        const int pad = 4;
        int ih = GetItemHeight();
        std::vector<int> vis = GetVisibleItemIndices();
        int visible = std::min((int)vis.size(), (int)MAX_VISIBLE_ITEMS);
        dropdown_rect.w = CalcDropdownWidth();
        dropdown_rect.h = visible * ih + pad * 2;

        SDL_Window* win = SDL_GetRenderWindow(renderer);
        HWND parent_hwnd = win ? SDL_GetWindowHWND(win) : nullptr;
        float scale = PopupWindow::GetDpiScaleOf(renderer);

        // 父窗口客户区原点在屏幕上的物理坐标
        POINT cli = {0, 0};
        if(parent_hwnd) ClientToScreen(parent_hwnd, &cli);

        // 控件与下拉框在屏幕上的物理尺寸/位置
        int drop_w = (int)std::lround(dropdown_rect.w * scale);
        int drop_h = (int)std::lround(dropdown_rect.h * scale);
        int combo_left_s = cli.x + (int)std::lround(combo_screen.x * scale);
        int combo_top_s = cli.y + (int)std::lround(combo_screen.y * scale);
        int combo_right_s = cli.x + (int)std::lround((combo_screen.x + combo_screen.w) * scale);
        int combo_bottom_s = cli.y + (int)std::lround((combo_screen.y + combo_screen.h) * scale);

        // 默认放在控件下方（屏幕物理坐标）
        int drop_x = combo_left_s;
        int drop_y = combo_bottom_s;

        // 依据整个屏幕（显示器工作区，不含任务栏）判断放在下方还是上方
        if(parent_hwnd){
            HMONITOR mon = MonitorFromWindow(parent_hwnd, MONITOR_DEFAULTTONEAREST);
            MONITORINFO mi;
            mi.cbSize = sizeof(mi);
            if(mon && GetMonitorInfo(mon, &mi)){
                // 下方空间不足且上方空间足够 -> 放到控件上方
                if(combo_bottom_s + drop_h > mi.rcWork.bottom && combo_top_s - drop_h >= mi.rcWork.top){
                    drop_y = combo_top_s - drop_h;
                }
                // 水平方向尽量留在屏幕内：超出右缘则右对齐到控件右缘
                if(combo_left_s + drop_w > mi.rcWork.right){
                    drop_x = combo_right_s - drop_w;
                }
                if(drop_x < mi.rcWork.left) drop_x = mi.rcWork.left;
                if(drop_y < mi.rcWork.top) drop_y = mi.rcWork.top;
            }
        }

        dropdown_rect.x = combo_screen.x;                 // 逻辑父客户区位置（参考）
        dropdown_rect.y = combo_screen.y + combo_screen.h;
        dropdown_screen_rect = {drop_x, drop_y, drop_w, drop_h};
        parent_client_origin = {cli.x, cli.y};
        combo_dpi_scale = scale;

        scroll_offset = 0;

        tooltip.SetRenderCallback([this](SDL_Renderer* r, const SDL_Rect& size, const SDL_Point& mouse){
            RenderDropdown(r, size, mouse);
        });

        if(tooltip.Open(renderer, drop_x, drop_y, dropdown_rect.w, dropdown_rect.h)){
            dropdown_open = true;
            // 以当前按键状态初始化，避免把打开时残留的按下误判为滚动条开始拖拽
            scrollbar_last_btn_down = tooltip.IsButtonDown();
            scrollbar_dragging = false;
            this->InvalidateRect();
        }
    }

    void CloseDropdown(){
        if(dropdown_open){
            dropdown_open = false;
            want_open = false;
            tooltip.Close();
            this->InvalidateRect();
        }
    }

    // 渲染下拉列表内容（由 PopupWindow 每帧调用）
    void RenderDropdown(SDL_Renderer* r, const SDL_Rect& size, const SDL_Point& mouse){
        std::vector<int> vis = GetVisibleItemIndices();
        const int pad = 4;
        int ih = GetItemHeight();
        int visible = std::min((int)vis.size() - scroll_offset, (int)MAX_VISIBLE_ITEMS);
        if(visible < 0) visible = 0;

        bool sb_visible = IsScrollBarVisible();
        // 滚动条可见时，条目区右界到滚动条热区左缘，使条目/悬停不延伸到滚动条下方
        int item_right = sb_visible ? (size.w - SCROLLBAR_HOT_RIGHT - SCROLLBAR_HOT_WIDTH) : (size.w - pad);

        // 背景
        SDL_SetRenderDrawColor(r, ColorArg(color_kit.BackgroundColor));
    SDL_FRect __fr_34 = toFRect(size);
        SDL_RenderFillRect(r, &__fr_34);

        // 条目（不含当前已选中的条目），裁剪到不进入滚动条区域
        SDL_Rect old_clip;
        SDL_GetRenderClipRect(r, &old_clip);
        SDL_Rect item_clip = {pad, pad, item_right - pad, size.h - pad * 2};
        SDL_Rect new_clip = SDL_RectEmpty(&old_clip) ? item_clip : RectIntersection(old_clip, item_clip);
        SDL_SetRenderClipRect(r, &new_clip);
        for(int i = 0; i < visible; i++){
            int idx = vis[scroll_offset + i];
            SDL_Rect row = {pad, pad + i * ih, item_right - pad, ih};
            bool hover = tooltip.IsMouseInside() && isPointInsideRect(mouse, row);
            if(hover){
                SDL_SetRenderDrawColor(r, ColorArg(color_kit.ForegroundColor));
    SDL_FRect __fr_35 = toFRect(row);
                SDL_RenderFillRect(r, &__fr_35);
            }
            if(font){
                font->paintText_Blended(r, row.x + 8, row.y + row.h / 2, ptsize, items[idx].ui_string,
                    color_kit.TextColor, {0.0f, 0.5f});
            }
        }
        if(SDL_RectEmpty(&old_clip)) SDL_SetRenderClipRect(r, NULL);
        else SDL_SetRenderClipRect(r, &old_clip);

        // 滚动条（条目溢出时显示）：右对齐于下拉框，带轨道背景与悬停变色反馈
        if(sb_visible){
            SDL_Rect track = GetScrollBarHotRect(size);
            SDL_SetRenderDrawColor(r, ColorArg(color_kit.BackgroundColorDarker));
    SDL_FRect __fr_36 = toFRect(track);
            SDL_RenderFillRect(r, &__fr_36);

            SDL_Rect bar = GetScrollBarThumbRect(size);
            bool sb_hover = tooltip.IsMouseInside() && isPointInsideRect(mouse, track);
            SDL_Color sb_c = sb_hover ? color_kit.ForegroundColorLighter : color_kit.ForegroundColor;
            SDL_SetRenderDrawColor(r, ColorArg(sb_c));
    SDL_FRect __fr_37 = toFRect(bar);
            SDL_RenderFillRect(r, &__fr_37);
        }

        // 边框
        SDL_SetRenderDrawColor(r, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_38 = toFRect(size);
        SDL_RenderRect(r, &__fr_38);
    }

    // 每帧驱动下拉列表：处理滚轮/滚动条拖拽/点击并重绘
    void PumpDropdown(){
        if(!dropdown_open) return;

        // 每帧轮询弹窗鼠标状态（位置/按下/点击/窗外点击）
        tooltip.PollInput();

        int wheel = tooltip.ConsumeWheel();
        if(wheel != 0){
            int max_scroll = std::max(0, (int)GetVisibleItemIndices().size() - (int)MAX_VISIBLE_ITEMS);
            scroll_offset = std::clamp(scroll_offset - wheel / WHEEL_DELTA, 0, max_scroll);
        }

        SDL_Point mpos = tooltip.GetMousePos();     // 逻辑坐标
        bool btn = tooltip.IsButtonDown();
        bool sb_visible = IsScrollBarVisible();

        // 滚动条拖拽状态机
        if(sb_visible){
            if(btn && !scrollbar_last_btn_down){
                // 检测到新按下：命中滚动条热区则开始拖拽（滑块上记抓取偏移，轨道上则滑块中心跳到鼠标处）
                SDL_Rect hot = GetScrollBarHotRect(dropdown_rect);
                if(isPointInsideRect(mpos, hot)){
                    SDL_Rect thumb = GetScrollBarThumbRect(dropdown_rect);
                    int max_scroll = std::max(0, (int)GetVisibleItemIndices().size() - (int)MAX_VISIBLE_ITEMS);
                    const int pad = 4;
                    int track_h = dropdown_rect.h - pad * 2;
                    int bar_h = std::max(16, track_h * MAX_VISIBLE_ITEMS / std::max(1,(int)GetVisibleItemIndices().size()));
                    int max_bar_y = std::max(0, track_h - bar_h);
                    if(isPointInsideRect(mpos, thumb)){
                        scrollbar_grab_y = mpos.y - thumb.y;
                    }
                    else{
                        int new_bar_y = std::clamp(mpos.y - pad - bar_h / 2, 0, max_bar_y);
                        scroll_offset = (max_scroll > 0) ? new_bar_y * max_scroll / std::max(1,max_bar_y) : 0;
                        scrollbar_grab_y = bar_h / 2;
                    }
                    scrollbar_dragging = true;
                }
            }
            else if(btn && scrollbar_dragging){
                // 拖拽中：按鼠标Y更新滑块位置
                int max_scroll = std::max(0, (int)GetVisibleItemIndices().size() - (int)MAX_VISIBLE_ITEMS);
                const int pad = 4;
                int track_h = dropdown_rect.h - pad * 2;
                int bar_h = std::max(16, track_h * MAX_VISIBLE_ITEMS / std::max(1,(int)GetVisibleItemIndices().size()));
                int max_bar_y = std::max(0, track_h - bar_h);
                int new_bar_y = std::clamp(mpos.y - pad - scrollbar_grab_y, 0, max_bar_y);
                scroll_offset = (max_scroll > 0) ? new_bar_y * max_scroll / std::max(1,max_bar_y) : 0;
            }
        }
        scrollbar_last_btn_down = btn;

        // 点击下拉窗口外部 -> 关闭
        if(tooltip.ConsumeOutsideClick()){
            scrollbar_dragging = false;
            CloseDropdown();
            return;
        }

        // 点击处理（含拖拽结束）：滚动条上的点击/拖拽释放不选中条目、不关闭
        SDL_Point cp;
        if(tooltip.ConsumeClick(cp)){
            bool on_sb = sb_visible && isPointInsideRect(cp, GetScrollBarHotRect(dropdown_rect));
            if(scrollbar_dragging || on_sb){
                scrollbar_dragging = false;
            }
            else{
                int idx = HitTestItem(cp);
                if(idx >= 0 && idx < (int)items.size()){
                    id_item_chosen = items[idx].id;
                    CallIfNotNull(actions.OnChange, this, items[idx]);
                }
                scrollbar_dragging = false;
                CloseDropdown();
                return;
            }
        }
        else if(!btn){
            // 按钮已松开但无点击事件（如拖出窗口后松开）-> 结束拖拽
            scrollbar_dragging = false;
        }

        tooltip.Render();
    }

public:

    void SetItems(const std::vector<ComboItem>& _items,int _id_item_chosen){
        this->InvalidateRect();
        items = _items;
        SortItems(items);
        id_item_chosen = _id_item_chosen;
        font->SizeText(ptsize,GetSelectedItem().ui_string,&default_rect.w,&default_rect.h);
        default_rect.w = (float)default_rect.w * 1.5f;
        this->InvalidateRect();
    } 

    void SetFont(FontEx* _font,int _ptsize){
        this->InvalidateRect();
        font = _font;
        ptsize = _ptsize;
        font->SizeText(ptsize,GetSelectedItem().ui_string,&default_rect.w,&default_rect.h);
        default_rect.w = (float)default_rect.w * 1.5f;
        this->InvalidateRect();
    }

    /**
     * @brief Get selected item's ID
     * 
     * @return 0xFFFFFFFF if not selected any one.
     */
    int GetSelectedID() const{
        return GetSelectedItem().id;
    }

    /**
     * @brief 编程式选中条目（不触发 OnChange 回调）
     */
    void SelectItem(int id){
        if(id_item_chosen != id){
            id_item_chosen = id;
            this->InvalidateRect();
        }
    }

    /**
     * @brief 当前下拉列表是否处于打开状态
     */
    bool IsDropdownOpen() const{
        return dropdown_open;
    }

    size_t GetItemCount() const{
        return items.size();
    }

    const ComboItem& GetItem(size_t index) const{
        return items[index];
    }

    bool IsEnabled() const{
        return enable;
    }

    void Enable(){
        enable = true;
    }

    void Disable(){
        enable = false;
    }

    /**
     * @brief 构造ComboBox控件
     * 
     * @param _default_text 不选中任何元素时显示的文本
     * @param _items 候选元素
     * @param _id_item_chosen 默认选择元素，设置为(int)0xFFFFFFFF表示默认不选中任何元素
     * @param _font 字体
     * @param _ptsize 字体大小
     * @param rect 范围
     */
	ComboBox(const std::string& _default_text,const std::vector<ComboItem>& _items,int _id_item_chosen, FontEx* _font,int _ptsize,const SDL_Rect& rect = {0,0,0,0})
	{
		default_text = _default_text;
        items = _items;
        SortItems(items);
        id_item_chosen = _id_item_chosen;
        font = _font;
        ptsize = _ptsize;
        default_rect = rect;
        enable = true;
        font->SizeText(ptsize,GetSelectedItem().ui_string,&default_rect.w,&default_rect.h);

        default_rect.w = (float)default_rect.w * 1.5f;

        bg_color.setLastValue(color_kit.BackgroundColor);

        this->InvalidateRect();
	}

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{
        // 捕获父渲染器，用于打开下拉子窗口
        parent_renderer = renderer;

        if(enable_animation){
            SDL_Rect draw_area = this->default_rect;
            draw_area.x = relative_rect.x;
            draw_area.y = relative_rect.y;
            SDL_Color bg_c = bg_color.getLastValue(); 
            SDL_SetRenderDrawColor(renderer, ColorArg(bg_c));
    SDL_FRect __fr_39 = toFRect(draw_area);
            SDL_RenderFillRect(renderer, &__fr_39);
            // 文本左对齐（左侧留 8px 内边距），裁剪到不进入右侧指示符区域
            PaintSelectedText(renderer, draw_area);
            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_40 = toFRect(draw_area);
            SDL_RenderRect(renderer, &__fr_40);

            // 右侧下拉指示符（加号/减号）
            PaintDropdownIndicator(renderer, draw_area);
            return;
        }

        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        if (_pressing)
		{
			SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColorDarker));
		}
        else if(_hovered)
		{
			SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.ForegroundColor));
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColor));
		}
    SDL_FRect __fr_41 = toFRect(draw_area);
		SDL_RenderFillRect(renderer, &__fr_41);
		
        // 文本左对齐（左侧留 8px 内边距），裁剪到不进入右侧指示符区域
        PaintSelectedText(renderer, draw_area);

		SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_42 = toFRect(draw_area);
		SDL_RenderRect(renderer, &__fr_42);

        // 右侧下拉指示符（加号/减号）
        PaintDropdownIndicator(renderer, draw_area);
    }

	void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        // 计算并保存控件在父窗口客户区中的屏幕矩形（事件回调已包含滚动偏移）
        combo_screen_rect = {relative_rect.x + default_rect.x, relative_rect.y + default_rect.y, default_rect.w, default_rect.h};

        // 下拉列表打开时，对非交互输入（滚轮/失焦/Escape）关闭下拉列表
        if(dropdown_open){
            if(event->type == SDL_EVENT_MOUSE_WHEEL){
                // 弹窗为 WS_EX_NOACTIVATE 不持有键盘焦点，滚轮消息经主窗口 SDL_EVENT_MOUSE_WHEEL 到达此处：
                // 鼠标在下拉列表或控件上方时滚动列表（不关闭），否则关闭下拉列表。
                POINT p;
                if(GetCursorPos(&p)){
                    SDL_Point screen_pt = {p.x, p.y};
                    bool over_dropdown = isPointInsideRect(screen_pt, dropdown_screen_rect);
                    bool over_combo = false;
                    if(combo_dpi_scale > 0.0f){
                        // 主窗口客户区逻辑坐标（与 combo_screen_rect 同坐标系）
                        SDL_Point client_pt = {
                            (int)std::lround((screen_pt.x - parent_client_origin.x) / combo_dpi_scale),
                            (int)std::lround((screen_pt.y - parent_client_origin.y) / combo_dpi_scale)
                        };
                        over_combo = isPointInsideRect(client_pt, combo_screen_rect);
                    }
                    if(over_dropdown || over_combo){
                        int max_scroll = std::max(0, (int)GetVisibleItemIndices().size() - (int)MAX_VISIBLE_ITEMS);
                        scroll_offset = std::clamp(scroll_offset - (int)event->wheel.y, 0, max_scroll);
                        wheel_consumed = true;   // 消费滚轮，阻止父容器同步滚动
                    }
                    else{
                        CloseDropdown();
                    }
                }
                else{
                    CloseDropdown();
                }
            }
            else if(event->type == SDL_EVENT_WINDOW_MOVED){
                // 父窗口移动时关闭，避免浮动弹窗留在旧位置
                CloseDropdown();
            }
            else if(event->type == SDL_EVENT_WINDOW_FOCUS_LOST){
                // 点击我们自己的下拉弹窗也会让主窗口触发 FOCUS_LOST，不能因此关闭下拉框。
                // 仅当焦点真正切到主窗口/本弹窗之外的其它窗口（其它应用）时才关闭。
                HWND fg = GetForegroundWindow();
                HWND main_hwnd = nullptr;
                if(parent_renderer){
                    SDL_Window* w = SDL_GetRenderWindow(parent_renderer);
                    if(w) main_hwnd = SDL_GetWindowHWND(w);
                }
                HWND popup_hwnd = tooltip.GetHwnd();
                if(main_hwnd && fg != main_hwnd && fg != popup_hwnd){
                    CloseDropdown();
                }
            }
            else if(event->type == SDL_EVENT_KEY_DOWN){
                if(event->key.scancode == SDL_SCANCODE_ESCAPE){
                    CloseDropdown();
                }
            }
        }

        if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    _pressing = true;
                    if(enable){
                        if(dropdown_open){
                            CloseDropdown();
                        }
                        else{
                            want_open = true;
                        }
                    }
                    this->InvalidateRect();
                }
                else if(dropdown_open){
                    // 点击主窗口关闭下拉列表；若点击坐标落在下拉列表区域内（事件可能泄漏到主窗口），
                    // 不关闭——由 PopupWindow/PumpDropdown 的全局轮询路径完成条目命中与选中。
                    SDL_Point screen_pt = {
                        parent_client_origin.x + (int)std::lround(event->button.x * combo_dpi_scale),
                        parent_client_origin.y + (int)std::lround(event->button.y * combo_dpi_scale)
                    };
                    bool in_drop = isPointInsideRect(screen_pt, dropdown_screen_rect);
                    if(!in_drop){
                        CloseDropdown();
                    }
                }
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_BUTTON_UP){
            SDL_Point pt = RelativizePoint({(int)event->button.x,(int)event->button.y},relative_rect);
            if(event->button.button == SDL_BUTTON_LEFT){
                if(isPointInsideRect(pt,default_rect)){
                    _pressing = false;
                    this->InvalidateRect();
                }
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_MOTION){
            SDL_Point pt = RelativizePoint({(int)event->motion.x,(int)event->motion.y},relative_rect);
            if (isPointInsideRect(pt, default_rect))
            {
                if (!_hovered)
                {
                    _hovered = true;
                    this->InvalidateRect();
                }
            }
            else
            {
                if (_pressing)
                {
                    _pressing = false;
                    this->InvalidateRect();
                }
                if (_hovered)
                {
                    _hovered = false;
                    this->InvalidateRect();
                }
            }
        }
    }

	void ActionOnTimer(const SDL_Rect& relative_rect) override{
        if(enable_animation){
            SDL_Color target_c;
            if(enable){
                if (_pressing)
                {
                    target_c = color_kit.BackgroundColorDarker;
                }
                else if(_hovered)
                {
                    target_c = color_kit.ForegroundColor;
                }
                else
                {
                    target_c = color_kit.BackgroundColor;
                }
            }
            else{
                target_c = color_kit.ForegroundColorDarker;
            }
            
            bool changed;
            bg_color.process(target_c,changed);
            if(changed){
                this->InvalidateRect();
            }
        }

        // 延迟打开下拉列表（等待父渲染器可用；位置使用事件回调保存的屏幕矩形，含滚动偏移）
        if(want_open && !dropdown_open && parent_renderer){
            want_open = false;
            OpenDropdown(parent_renderer, combo_screen_rect);
        }

        // 驱动已打开的下拉列表（滚动/点击/重绘）
        PumpDropdown();
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.ComboBox";
    }
};

#ifdef ENABLE_QRCODE_MODULE

/**
 * @brief 二维码控件，将文本内容渲染为QR码图像
 * 
 */
class QRCodeBox : public Fgui_Control{
    //内部PictureBox用于显示二维码图像
    PictureBox* pb;

    //待编码的文本内容
    std::string content;

    //纠错等级
    qrcodegen::QrCode::Ecc ecc;

    //每个QR码模块的像素大小
    int module_size;

    //二维码周围的空白边距（以QR码模块为单位）
    int padding;

    //二维码模块颜色
    SDL_Color qr_fg_color;
    //二维码背景颜色
    SDL_Color qr_bg_color;

    /**
     * @brief 根据content重新生成二维码图像
     * 
     */
    void RegenerateQRCode(){
        if(content.empty()){
            pb->SetImage(NULL);
            this->InvalidateRect();
            return;
        }

        try{
            const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(content.c_str(), ecc);

            int size = qr.getSize();
            int offset = padding * module_size;
            int img_size = (size + padding * 2) * module_size;

            //创建32位RGBA表面
            SDL_Surface* surface = SDL_CreateSurface(img_size, img_size, SDL_PIXELFORMAT_RGBA32);
            if(!surface) return;

            //填充像素
            SDL_LockSurface(surface);
            Uint32* pixels = (Uint32*)surface->pixels;
            Uint32 fg = SDL_MapSurfaceRGBA(surface, qr_fg_color.r, qr_fg_color.g, qr_fg_color.b, qr_fg_color.a);
            Uint32 bg = SDL_MapSurfaceRGBA(surface, qr_bg_color.r, qr_bg_color.g, qr_bg_color.b, qr_bg_color.a);

            int pitch = surface->pitch / sizeof(Uint32);

            //先用背景色填充整个表面（包括padding区域）
            for(int py = 0; py < img_size; py++){
                for(int px = 0; px < img_size; px++){
                    pixels[py * pitch + px] = bg;
                }
            }

            //绘制QR码模块（偏移padding区域）
            for(int y = 0; y < size; y++){
                for(int x = 0; x < size; x++){
                    Uint32 color = qr.getModule(x, y) ? fg : bg;
                    for(int dy = 0; dy < module_size; dy++){
                        for(int dx = 0; dx < module_size; dx++){
                            int px = offset + x * module_size + dx;
                            int py = offset + y * module_size + dy;
                            pixels[py * pitch + px] = color;
                        }
                    }
                }
            }
            SDL_UnlockSurface(surface);

            pb->SetImage(surface);
            SDL_DestroySurface(surface);

            this->InvalidateRect();
        }
        catch(const std::exception&){
            //二维码生成失败时清空图像
            pb->SetImage(NULL);
        }
    }

    public:

    /**
     * @brief 构造QRCodeBox控件
     * 
     * @param text 待编码的文本内容
     * @param _ecc 纠错等级，默认MEDIUM
     * @param _module_size 每个QR码模块的像素大小，默认4
     * @param rect 控件区域
     */
    QRCodeBox(const std::string& text = "",
              qrcodegen::QrCode::Ecc _ecc = qrcodegen::QrCode::Ecc::MEDIUM,
              int _module_size = 4,
              int _padding = 2,
              const SDL_Rect& rect = {0,0,0,0})
    {
        pb = new PictureBox;
        pb->SetFillingMethod(PictureBox::PICTURE_FILLING_METHOD_FIT);
        ecc = _ecc;
        module_size = (_module_size < 1) ? 1 : _module_size;
        padding = (_padding < 0) ? 0 : _padding;
        qr_fg_color = {0, 0, 0, 255};
        qr_bg_color = {255, 255, 255, 255};
        default_rect = rect;

        if(!text.empty()){
            SetContent(text);
        }
    }

    ~QRCodeBox(){
        delete pb;
    }

    /**
     * @brief 设置二维码内容并重新生成图像
     * 
     * @param text 待编码的文本
     */
    void SetContent(const std::string& text){
        content = text;
        RegenerateQRCode();
    }

    /**
     * @brief 获取当前二维码内容
     * 
     * @return const std::string& 
     */
    const std::string& GetContent() const{
        return content;
    }

    /**
     * @brief 设置模块像素大小并重新生成图像
     * 
     * @param size 模块像素大小（最小为1）
     */
    void SetModuleSize(int size){
        if(size < 1) size = 1;
        if(size != module_size){
            module_size = size;
            if(!content.empty()) RegenerateQRCode();
        }
    }

    /**
     * @brief 设置纠错等级并重新生成图像
     * 
     * @param _ecc 纠错等级
     */
    /**
     * @brief 设置二维码周围空白边距（以QR码模块为单位）并重新生成图像
     * 
     * @param _padding 边距（模块数，最小为0）
     */
    void SetPadding(int _padding){
        if(_padding < 0) _padding = 0;
        if(_padding != padding){
            padding = _padding;
            if(!content.empty()) RegenerateQRCode();
        }
    }

    /**
     * @brief 获取当前二维码周围空白边距（以QR码模块为单位）
     * 
     * @return int 边距（模块数）
     */
    int GetPadding() const{
        return padding;
    }

    void SetECC(qrcodegen::QrCode::Ecc _ecc){
        if(ecc != _ecc){
            ecc = _ecc;
            if(!content.empty()) RegenerateQRCode();
        }
    }

    /**
     * @brief 设置二维码颜色
     * 
     * @param fg 前景色（深色模块颜色）
     * @param bg 背景色（浅色模块颜色）
     */
    void SetQRColor(SDL_Color fg, SDL_Color bg){
        qr_fg_color = fg;
        qr_bg_color = bg;
        if(!content.empty()) RegenerateQRCode();
    }

    void ActionOnPaint(SDL_Renderer* renderer, const SDL_Rect& dirty_region, const SDL_Rect& relative_rect) override{
        pb->default_rect = this->default_rect;
        pb->ActionOnPaint(renderer, dirty_region, relative_rect);
    }

    void ActionOnEvent(const SDL_Event* event, const SDL_Rect& relative_rect) override{
        pb->default_rect = this->default_rect;
        pb->ActionOnEvent(event, relative_rect);
    }

    void ActionOnTimer(const SDL_Rect& relative_rect) override{
        pb->default_rect = this->default_rect;
        pb->ActionOnTimer(relative_rect);
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.QRCodeBox";
    }

    void ActionOnColorKitChange(const Fgui_ColorKit& ck) override{
        color_kit = ck;
        pb->ActionOnColorKitChange(ck);
        this->InvalidateRect();
    }
};

#endif

/**
 * @brief 进度条，用于展示任务的完成进度，支持三态：正常(Normal)/处理中(Processing)/暂停(Paused)
 * @note Normal态支持数值平滑动画（需启用控件动画），可居中显示百分比文本
 */
class ProgressBar : public Fgui_Control{
public:
    enum ProgressBarState{
        //暂停：显示当前进度但不再前进（填充变暗以示区分）
        PROGRESSBAR_STATE_PAUSED = 0,
        //处理中：进度未知，显示动画
        PROGRESSBAR_STATE_PROCESSING = 1,
        //正常：显示确定进度
        PROGRESSBAR_STATE_NORMAL = 2,
    };

private:
    //数值范围与当前值
    float min_val;
    float max_val;
    float current_val;

    //动画平滑器（Normal态平滑过渡用）
    EMASmoother ema_val;

    //是否在进度条中央显示百分比文本
    bool show_text;
    FontEx* font;
    int ptsize;

    //当前状态
    ProgressBarState state;

    //---------- 动画（Processing态）----------
    //动画周期（毫秒）
    uint64_t anim_period;
    //动画块占轨道宽度的比例
    float anim_block_ratio;
    //动画相位基准时间（进入Processing时重置，使动画从头开始）
    uint64_t anim_time_base;

    uint64_t anim_time_base2;

    //保证范围有效（max > min），避免除零
    void NormalizeRange(){
        if(min_val >= max_val) max_val = min_val + 1.0f;
    }

    //计算动画相位
    double CalcAnimPhase() const{
        double t = fmod((double)(SDL_GetTicks() - anim_time_base), (double)anim_period) / (double)anim_period; // 0~1
        return t;
    }

public:

    struct Actions{
        //数值发生变化时调用（含SetValue/SetProgress主动赋值）
        std::function<void(ProgressBar*,float)> OnValueChanged;
    } actions;

    /**
     * @brief 构造ProgressBar控件
     * 
     * @param min 最小值
     * @param max 最大值
     * @param default_val 默认值
     * @param _font 百分比文本字体，为nullptr则不显示文本
     * @param _ptsize 字体大小
     * @param _state 初始状态
     * @param rect 范围
     */
    ProgressBar(float min,
               float max,
               float default_val,
               FontEx* _font = nullptr,
               int _ptsize = 0,
               ProgressBarState _state = PROGRESSBAR_STATE_NORMAL,
               const SDL_Rect& rect = {0,0,0,0})
    {
        default_rect = rect;

        min_val = min;
        max_val = max;
        NormalizeRange();

        current_val = std::clamp(default_val, min_val, max_val);
        ema_val.setLastValue(current_val);
        ema_val.setAlpha(0.3);

        font = _font;
        ptsize = _ptsize;
        show_text = (_font != nullptr);

        state = _state;
        anim_period = 1200;
        anim_block_ratio = 0.3f;
        anim_time_base = SDL_GetTicks();

        anim_time_base2 = 0;

        this->InvalidateRect();
    }

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{
        SDL_Rect draw_area = this->default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        SDL_Rect track_rect = ShrinkRect(draw_area, 0.95, 0.4);

        paintRoundedRect(renderer, track_rect, color_kit.BorderColor, color_kit.BackgroundColor, INT_MAX);

        if(state == PROGRESSBAR_STATE_PROCESSING){
            double phase = CalcAnimPhase();
            int block_w = std::max(8, (int)(track_rect.w * anim_block_ratio));
            int block_x = track_rect.x + (int)((track_rect.w + block_w) * phase) - block_w;
            SDL_Rect block = {block_x, track_rect.y, block_w, track_rect.h};
            block = RectIntersection(block, track_rect);
            paintRoundedRect(renderer, block, color_kit.BorderColor, color_kit.ForegroundColor, INT_MAX);
            return; // 进度未知，不显示百分比
        }

        // Normal/Paused：绘制确定进度
        float actual_val = current_val;
        if(enable_animation && state == PROGRESSBAR_STATE_NORMAL){
            actual_val = ema_val.getLastValue();
        }

        float progress = (actual_val - min_val) / (max_val - min_val);
        progress = std::clamp(progress, 0.0f, 1.0f);

        if(progress > 0.0f){
            SDL_Rect fill_rect = track_rect;
            fill_rect.w = (int)(fill_rect.w * progress);
            // Paused态用更深的前景色，视觉上变暗以区分
            SDL_Color fill_color = color_kit.ForegroundColor;
            if(state == PROGRESSBAR_STATE_PAUSED) fill_color = color_kit.ForegroundColorDarker;
            paintRoundedRect(renderer, fill_rect, color_kit.BorderColor, fill_color, INT_MAX);
        }

        // 百分比文本
        if(show_text && font){
            int pct = (int)std::lround(progress * 100.0f);
            SDL_Color txt_color = color_kit.TextColor;
            if(state == PROGRESSBAR_STATE_PAUSED) txt_color = color_kit.ForegroundColor;
            font->paintText_Blended(renderer,draw_area.x + draw_area.w / 2,draw_area.y + draw_area.h / 2,ptsize,std::to_string(pct) + "%",txt_color,{0.5f,0.5f});
        }
    }

    void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        // 进度条仅用于展示，不处理交互
    }

    void ActionOnTimer(const SDL_Rect& relative_rect) override{
        // Processing：循环动画
        if(state == PROGRESSBAR_STATE_PROCESSING){
            if((SDL_GetTicks() - anim_time_base2 > anim_period / 20) || enable_animation){
                anim_time_base2 = SDL_GetTicks();
                this->InvalidateRect();
            }
            return;
        }
        // Normal：数值平滑动画（与Slider一致：平滑值相对目标偏移超过1像素才重绘）
        if(enable_animation && state == PROGRESSBAR_STATE_NORMAL){
            float smoothed = ema_val.process(current_val);
            if(std::abs(smoothed - current_val) / (max_val - min_val) * default_rect.w > 1){
                this->InvalidateRect();
            }
        }
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.ProgressBar";
    }

    /**
     * @brief 设置状态
     * 
     * @param _state 新状态（PROGRESSBAR_STATE_PAUSED / PROCESSING / NORMAL）
     */
    void SetState(ProgressBarState _state){
        if(state == _state) return;
        state = _state;
        if(state == PROGRESSBAR_STATE_PROCESSING){
            // 重置动画相位，使推挽动画从头开始
            anim_time_base = SDL_GetTicks();
        }
        else{
            // 离开动画态：同步EMA基准，避免切回Normal时出现回退动画
            ema_val.setLastValue(current_val);
        }
        this->InvalidateRect();
    }

    ProgressBarState GetState() const{
        return state;
    }

    bool IsProcessing() const{
        return state == PROGRESSBAR_STATE_PROCESSING;
    }

    /**
     * @brief 设置动画周期
     * 
     * @param ms 往复周期（毫秒，最小1）
     */
    void SetAnimPeriod(uint64_t ms){
        anim_period = (ms < 1) ? 1 : ms;
        if(state == PROGRESSBAR_STATE_PROCESSING) this->InvalidateRect();
    }

    uint64_t GetAnimPeriod() const{
        return anim_period;
    }

    /**
     * @brief 设置动画块占轨道宽度的比例
     * 
     * @param ratio 比例（0.05~0.6）
     */
    void SetAnimBlockRatio(float ratio){
        anim_block_ratio = std::clamp(ratio, 0.05f, 0.6f);
        if(state == PROGRESSBAR_STATE_PROCESSING) this->InvalidateRect();
    }

    /**
     * @brief 设置当前进度值
     * 
     * @param _value 数值（自动钳位到[min,max]）
     * @param animated 是否平滑过渡（默认false立即跳变；true需启用控件动画，由ActionOnTimer驱动）
     */
    void SetValue(float _value, bool animated = false){
        float new_val = std::clamp(_value, min_val, max_val);
        if(new_val != current_val){
            current_val = new_val;
            if(!animated){
                // 立即跳变：同步EMA基准，防止动画模式下出现回退动画
                ema_val.setLastValue(current_val);
            }
            CallIfNotNull(actions.OnValueChanged,this,current_val);
            this->InvalidateRect();
        }
    }

    float GetValue() const{
        return current_val;
    }

    /**
     * @brief 获取归一化进度（0~1）
     */
    float GetProgress() const{
        return std::clamp((current_val - min_val) / (max_val - min_val), 0.0f, 1.0f);
    }

    /**
     * @brief 设置归一化进度（0~1），自动换算到[min,max]
     */
    void SetProgress(float _progress, bool animated = false){
        SetValue(min_val + std::clamp(_progress, 0.0f, 1.0f) * (max_val - min_val), animated);
    }

    void SetRange(float min, float max){
        min_val = min;
        max_val = max;
        NormalizeRange();
        current_val = std::clamp(current_val, min_val, max_val);
        ema_val.setLastValue(current_val);
        this->InvalidateRect();
    }

    float GetMinValue() const{ return min_val; }
    float GetMaxValue() const{ return max_val; }

    /**
     * @brief 开关/更新百分比文本显示
     * 
     * @param _show 是否显示
     * @param _font 字体（传入则一并更新）
     * @param _ptsize 字号
     */
    void SetShowText(bool _show, FontEx* _font = nullptr, int _ptsize = 0){
        if(_font){
            font = _font;
            ptsize = _ptsize;
        }
        show_text = _show && (font != nullptr);
        this->InvalidateRect();
    }

    bool IsShowText() const{
        return show_text;
    }

    //重绘范围略微外扩，避免圆角边缘残留
    void InvalidateRect() override{
        this->InvalidateRect(SDL_Rect{-10,0,default_rect.w + 20,default_rect.h});
    }

    void InvalidateRect(const SDL_Rect& rect) override{
        dirty_region = ExtendRect(dirty_region,rect);
    }
};

//对应事件发生时向对应ControlBox发送这些事件
#define SDL_USEREVENT_CODE_TAB_ACTIVATE 122001
#define SDL_USEREVENT_CODE_TAB_DEACTIVATE 122002

#define TAB_FLAG_NONE 0
// 显示关闭按钮，按下关闭按钮调用Actions结构体中的OnTabClose
#define TAB_FLAG_CLOSE_BUTTON 1
// 允许用户通过鼠标拖动重新排列标签页（不设置则禁止拖动重排）
#define TAB_FLAG_ALLOW_REARRANGE 2
// 标签页条位置
#define TAB_FLAG_POS_TOP 0x00
#define TAB_FLAG_POS_BOTTOM 0x10
#define TAB_FLAG_POS_LEFT 0x20
#define TAB_FLAG_POS_RIGHT 0x30
// 位置标志掩码
#define TAB_FLAG_POS_MASK 0x30

/**
 * @brief 标签页控件
 * 
 */
class TabControl : public Fgui_Control{
    public:
    class TabItem{
        public:
        // 标签页名称
        std::string name;
        // 标签页ID
        std::string id;
        // 标签页内容
        std::shared_ptr<ControlBox> content;
    };
    private:
    //---------- 外观常量 ----------
    // 标签文本左右内边距
    static constexpr int TAB_PAD = 10;
    // 相邻标签间距
    static constexpr int TAB_GAP = 6;
    // 条首尾留白
    static constexpr int TAB_MARGIN = 6;
    // 标签在条内的缩进（条厚度方向）
    static constexpr int TAB_INSET = 4;
    // 标签最小高度
    static constexpr int TAB_MIN_HEIGHT = 30;
    // 激活标签强调条厚度
    static constexpr int TAB_ACCENT = 3;
    // 关闭按钮预留宽度
    static constexpr int TAB_CLOSE_SLOT = 20;
    // 关闭按钮尺寸
    static constexpr int TAB_CLOSE_SIZE = 16;

    // 标签页列表
    std::vector<TabItem> tabs;
    // 空或者无对应标签页时，认为是没有激活任何标签页
    std::string active_tab_id;
    // 标志位
    int flags;

    // 字体
    FontEx* font;
    // 字号
    int ptsize;

    // 悬停的标签页下标（-1 表示无）
    int hover_tab_index = -1;
    // 按下的标签页下标（-1 表示无），用于点击激活判定
    int press_tab_index = -1;
    // 拖动的标签页下标（-1 表示无）
    int drag_tab_index = -1;
    // 关闭按钮按下的标签页下标（-1 表示无）
    int pending_close_index = -1;
    // 是否已进入拖动状态（移动超过阈值才置 true）
    bool dragging_tabs = false;
    // 拖动起点（本控件本地坐标）
    SDL_Point drag_start = {0,0};
    // hover 背景过渡动画
    EMAColor hover_bg_color;

    std::vector<TabItem>::iterator FindTab(const std::string& id){
        auto fd = std::find_if(tabs.begin(),tabs.end(),[&](const TabItem& tab){
            return tab.id == id;
        });
        if(fd == tabs.end()) throw std::out_of_range("Tab id not found");
        return fd;
    }

    // 标签页条位置（flags 中的位置位）
    int GetTabStripPosition() const{
        return flags & TAB_FLAG_POS_MASK;
    }

    // 当前字号的行高
    int GetFontHeight() const{
        if(font) return std::max(font->GetPrimaryFontHeight(ptsize), 16);
        return 16;
    }

    // 标签文本宽度（px）
    int GetTextWidth(int index) const{
        if(!font || index < 0 || index >= (int)tabs.size()) return 0;
        int w = 0, h = 0;
        font->SizeText(ptsize, tabs[index].name, &w, &h);
        return w;
    }

    // 水平条标签的宽度（含内边距与关闭按钮预留）
    int GetTabWidth(int index) const{
        int w = GetTextWidth(index) + 2 * TAB_PAD;
        if(flags & TAB_FLAG_CLOSE_BUTTON) w += TAB_CLOSE_SLOT;
        return std::max(w, 20);
    }

    // 垂直条标签的高度
    int GetTabHeightV() const{
        return std::max(GetFontHeight() + 2 * TAB_PAD, TAB_MIN_HEIGHT);
    }

    // 标签页条厚度（条垂直方向占用的尺寸）
    int GetStripThickness() const{
        int pos = GetTabStripPosition();
        if(pos == TAB_FLAG_POS_TOP || pos == TAB_FLAG_POS_BOTTOM){
            return std::max(GetFontHeight() + 12, TAB_MIN_HEIGHT) + 2 * TAB_INSET;
        }
        int maxw = 0;
        for(size_t i = 0; i < tabs.size(); i++){
            maxw = std::max(maxw, GetTabWidth((int)i));
        }
        return std::max(maxw, 40) + 2 * TAB_MARGIN;
    }

    // 标签页条矩形（本控件本地坐标）
    SDL_Rect CalcStripRect() const{
        SDL_Rect r = default_rect;
        r.x = 0;
        r.y = 0;
        int pos = GetTabStripPosition();
        if(pos == TAB_FLAG_POS_TOP){
            r.h = GetStripThickness();
        }
        else if(pos == TAB_FLAG_POS_BOTTOM){
            r.h = GetStripThickness();
            r.y = default_rect.h - r.h;
        }
        else if(pos == TAB_FLAG_POS_LEFT){
            r.w = GetStripThickness();
        }
        else{
            r.w = GetStripThickness();
            r.x = default_rect.w - r.w;
        }
        return r;
    }

    // 内容区矩形（本控件本地坐标）
    SDL_Rect CalcContentRect() const{
        SDL_Rect strip = CalcStripRect();
        SDL_Rect r = default_rect;
        r.x = 0;
        r.y = 0;
        int pos = GetTabStripPosition();
        if(pos == TAB_FLAG_POS_TOP){
            r.y = strip.y + strip.h;
            r.h = default_rect.h - strip.h;
        }
        else if(pos == TAB_FLAG_POS_BOTTOM){
            r.h = default_rect.h - strip.h;
        }
        else if(pos == TAB_FLAG_POS_LEFT){
            r.x = strip.x + strip.w;
            r.w = default_rect.w - strip.w;
        }
        else{
            r.w = default_rect.w - strip.w;
        }
        if(r.w < 0) r.w = 0;
        if(r.h < 0) r.h = 0;
        return r;
    }

    // 同步激活页 default_rect 到内容区尺寸，并返回内容区屏幕矩形（无激活页/异常返回空）
    // 事件/定时器路径必须用本函数同步 default_rect，否则 ControlBox 会用旧尺寸（如初始 0 尺寸）
    // 做 ClipRect/ShallEventPenetrate，导致内容区内的控件收不到事件（点不动）
    SDL_Rect SyncActiveContentRect(const SDL_Rect& ctl_screen){
        if(active_tab_id.empty()) return {0,0,0,0};
        try{
            auto fd = FindTab(active_tab_id);
            SDL_Rect content = CalcContentRect();
            fd->content->default_rect = {0,0,content.w,content.h};
            return {ctl_screen.x + content.x, ctl_screen.y + content.y, content.w, content.h};
        }
        catch(...){
            return {0,0,0,0};
        }
    }

    // 第 index 个标签页矩形（本控件本地坐标）
    SDL_Rect CalcTabRect(int index) const{
        if(index < 0 || index >= (int)tabs.size()) return {0,0,0,0};
        SDL_Rect strip = CalcStripRect();
        SDL_Rect r = {0,0,0,0};
        int pos = GetTabStripPosition();
        if(pos == TAB_FLAG_POS_TOP || pos == TAB_FLAG_POS_BOTTOM){
            int x = strip.x + TAB_MARGIN;
            for(int i = 0; i < index; i++){
                x += GetTabWidth(i) + TAB_GAP;
            }
            r.x = x;
            r.y = strip.y + TAB_INSET;
            r.w = GetTabWidth(index);
            r.h = std::max(GetFontHeight() + 12, TAB_MIN_HEIGHT);
        }
        else{
            int y = strip.y + TAB_MARGIN;
            for(int i = 0; i < index; i++){
                y += GetTabHeightV() + TAB_GAP;
            }
            r.x = strip.x + TAB_MARGIN;
            r.y = y;
            r.w = strip.w - 2 * TAB_MARGIN;
            r.h = GetTabHeightV();
        }
        return r;
    }

    // 命中测试：返回鼠标所在的标签页下标，未命中返回 -1
    int HitTestTab(const SDL_Point& pt) const{
        for(int i = 0; i < (int)tabs.size(); i++){
            if(isPointInsideRect(pt, CalcTabRect(i))) return i;
        }
        return -1;
    }

    // 关闭按钮矩形（由标签矩形推导，本控件本地坐标）
    SDL_Rect GetCloseButtonRect(const SDL_Rect& tab_rect) const{
        return {
            tab_rect.x + tab_rect.w - TAB_CLOSE_SLOT - 4,
            tab_rect.y + (tab_rect.h - TAB_CLOSE_SIZE) / 2,
            TAB_CLOSE_SIZE,
            TAB_CLOSE_SIZE
        };
    }

    // 命中测试：是否点在关闭按钮上
    bool HitTestCloseButton(const SDL_Point& pt, int index) const{
        if(!(flags & TAB_FLAG_CLOSE_BUTTON)) return false;
        if(index < 0 || index >= (int)tabs.size()) return false;
        return isPointInsideRect(pt, GetCloseButtonRect(CalcTabRect(index)));
    }

    // 拖动重排：把 from 位置标签移动到 to 位置，并更新 drag_tab_index
    void MoveTab(int from, int to){
        if(from == to) return;
        if(from < 0 || from >= (int)tabs.size()) return;
        if(to < 0 || to >= (int)tabs.size()) return;
        TabItem item = std::move(tabs[from]);
        tabs.erase(tabs.begin() + from);
        tabs.insert(tabs.begin() + to, std::move(item));
        drag_tab_index = to;
    }

    // 沿条轴判断 lpt 是否在 rect 中心线之前
    bool IsBeforeCenter(const SDL_Point& lpt, const SDL_Rect& r) const{
        int pos = GetTabStripPosition();
        if(pos == TAB_FLAG_POS_TOP || pos == TAB_FLAG_POS_BOTTOM){
            return lpt.x < r.x + r.w / 2;
        }
        return lpt.y < r.y + r.h / 2;
    }

    // 沿条轴判断 lpt 是否在 rect 中心线之后
    bool IsBeyondCenter(const SDL_Point& lpt, const SDL_Rect& r) const{
        int pos = GetTabStripPosition();
        if(pos == TAB_FLAG_POS_TOP || pos == TAB_FLAG_POS_BOTTOM){
            return lpt.x > r.x + r.w / 2;
        }
        return lpt.y > r.y + r.h / 2;
    }

    // 直接从列表移除指定下标标签（不调用 OnTabClose），并修正激活状态与交互状态
    void RemoveTabAt(int index){
        if(index < 0 || index >= (int)tabs.size()) return;
        bool was_active = (tabs[index].id == active_tab_id);
        std::string next_active;
        if(was_active && tabs.size() > 1){
            // 关闭的标签移除后，原 index 位置由 index+1 顶上；若被关闭的是最后一个则退到 index-1
            int ni = (index < (int)tabs.size() - 1) ? index + 1 : index - 1;
            next_active = tabs[ni].id;
        }
        SDL_Event event = {0};
        event.type = SDL_EVENT_USER;
        event.user.timestamp = SDL_GetTicksNS();
        event.user.code = SDL_USEREVENT_CODE_TAB_DEACTIVATE;
        tabs[index].content->MaintainEvent(&event,{});
        tabs[index].content->ClearInvaildRect();
        tabs.erase(tabs.begin() + index);
        if(was_active){
            active_tab_id = "";
            if(!next_active.empty()) ActivateTab(next_active);
        }
        if(hover_tab_index >= (int)tabs.size()) hover_tab_index = -1;
        if(press_tab_index >= (int)tabs.size()) press_tab_index = -1;
        if(drag_tab_index >= (int)tabs.size()) drag_tab_index = -1;
        if(pending_close_index >= (int)tabs.size()) pending_close_index = -1;
        dragging_tabs = false;
        this->InvalidateRect();
    }

    // 关闭标签页：经 OnTabClose 回调判定后移除
    void DoCloseTab(int index){
        if(index < 0 || index >= (int)tabs.size()) return;
        bool ok = true;
        if(actions.OnTabClose){
            ok = actions.OnTabClose(tabs[index]);
        }
        if(ok) RemoveTabAt(index);
    }

    // 绘制单个标签页
    void PaintTab(SDL_Renderer* renderer, const SDL_Rect& tab, int index){
        bool active = (tabs[index].id == active_tab_id);
        bool hover = (index == hover_tab_index);
        // 是否正处于拖动中的标签页
        bool dragging = (dragging_tabs && index == drag_tab_index);

        SDL_Color fill;
        SDL_Color text_c;
        if(dragging){
            // 拖动中的标签：高亮浮起，让用户清楚正在拖动的是哪个标签
            fill = color_kit.ForegroundColorDarker;
            text_c = color_kit.TextColor;
        }
        else if(active){
            fill = color_kit.BackgroundColor;
            text_c = color_kit.TextColor;
        }
        else if(hover){
            if(enable_animation){
                fill = hover_bg_color.getLastValue();
            }
            else{
                fill = color_kit.ForegroundColorDarker;
            }
            text_c = color_kit.ForegroundColorLighter;
        }
        else{
            fill = color_kit.BackgroundColorLighter;
            text_c = color_kit.ForegroundColor;
        }

        // 方形外观（radius=0，去掉圆角，标签更简洁利落）
        int radius = 0;
        if(dragging){
            // 阴影偏移（不依赖 alpha 混合，用纯色深灰），模拟"浮起"效果
            SDL_Rect sh = {tab.x + 2, tab.y + 3, tab.w, tab.h};
            SDL_Color shadow = ColorInterval(color_kit.BackgroundColorDarker, SDL_Color{0,0,0,255}, 0.4f);
            paintRoundedRect(renderer, sh, shadow, shadow, radius);
            // 加粗方形边框：先画外扩 2px 的纯边框色矩形，再画主标签盖在上面，露出的一圈即描边
            SDL_Rect outer = {tab.x - 1, tab.y - 1, tab.w + 2, tab.h + 2};
            paintRoundedRect(renderer, outer, color_kit.ForegroundColor, color_kit.ForegroundColor, radius);
            // 主标签：亮边框 + 深色高亮填充
            paintRoundedRect(renderer, tab, color_kit.ForegroundColor, fill, radius);
        }
        else{
            paintRoundedRect(renderer, tab, color_kit.BorderColor, fill, radius);
        }

        // 激活强调条（位于条外缘一侧）——方形外观下直接横贯整条宽度；拖动中的标签跳过
        if(active && !dragging){
            SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.ForegroundColor));
            SDL_Rect acc = tab;
            int pos = GetTabStripPosition();
            if(pos == TAB_FLAG_POS_TOP){
                acc.x += radius;
                acc.y = tab.y;
                acc.w -= 2 * radius;
                acc.h = TAB_ACCENT;
            }
            else if(pos == TAB_FLAG_POS_BOTTOM){
                acc.x += radius;
                acc.y = tab.y + tab.h - TAB_ACCENT;
                acc.w -= 2 * radius;
                acc.h = TAB_ACCENT;
            }
            else if(pos == TAB_FLAG_POS_LEFT){
                acc.x = tab.x;
                acc.y += radius;
                acc.w = TAB_ACCENT;
                acc.h -= 2 * radius;
            }
            else{
                acc.x = tab.x + tab.w - TAB_ACCENT;
                acc.y += radius;
                acc.w = TAB_ACCENT;
                acc.h -= 2 * radius;
            }
            if(acc.w > 0 && acc.h > 0){
    SDL_FRect __fr_43 = toFRect(acc);
                SDL_RenderFillRect(renderer, &__fr_43);
            }
        }

        // 文本（裁剪到不进入关闭按钮区域）
        if(font){
            SDL_Rect ta = tab;
            if(flags & TAB_FLAG_CLOSE_BUTTON) ta.w -= TAB_CLOSE_SLOT;
            if(ta.w > 0){
                SDL_Rect old_clip;
                SDL_GetRenderClipRect(renderer, &old_clip);
                SDL_Rect new_clip = SDL_RectEmpty(&old_clip) ? ta : RectIntersection(old_clip, ta);
                SDL_SetRenderClipRect(renderer, &new_clip);
                font->paintText_Blended(renderer, ta.x + ta.w / 2, ta.y + ta.h / 2, ptsize, tabs[index].name, text_c, {0.5f, 0.5f});
                if(SDL_RectEmpty(&old_clip)) SDL_SetRenderClipRect(renderer, NULL);
                else SDL_SetRenderClipRect(renderer, &old_clip);
            }
        }

        // 关闭按钮
        if(flags & TAB_FLAG_CLOSE_BUTTON){
            SDL_Rect btn = GetCloseButtonRect(tab);
            SDL_Point c = {btn.x + btn.w / 2, btn.y + btn.h / 2};
            int r = btn.w / 2 - 1;
            if(hover){
                paintCircleEx(renderer, color_kit.BorderColor, color_kit.ForegroundColorDarker, c, r);
            }
            SDL_Color xc = hover ? color_kit.TextColor : color_kit.ForegroundColor;
            SDL_SetRenderDrawColor(renderer, ColorArg(xc));
            int o = r - 3;
            SDL_RenderLine(renderer, c.x - o, c.y - o, c.x + o, c.y + o);
            SDL_RenderLine(renderer, c.x + o, c.y - o, c.x - o, c.y + o);
        }
    }

    public:

    TabControl(int _flags = TAB_FLAG_NONE,FontEx* _font = nullptr,int _ptsize = 0, const SDL_Rect& rect = SDL_Rect{0,0,0,0}){
        flags = _flags;
        font = _font;
        ptsize = _ptsize;
        this->default_rect = rect;
        hover_tab_index = -1;
        press_tab_index = -1;
        drag_tab_index = -1;
        pending_close_index = -1;
        dragging_tabs = false;
        drag_start = {0,0};
        hover_bg_color.setLastValue(color_kit.BackgroundColorLighter);
        this->InvalidateRect();
    }

    ~TabControl(){
        ClearTabs();
    }

    struct Actions{
        // 关闭标签页回调，参数为要关闭的标签页，返回值决定是否移除该标签页，不重载默认移除
        std::function<bool(TabItem&)> OnTabClose;
        // 激活标签页回调，参数为要激活的标签页，返回值决定是否激活该标签页，不重载默认激活
        std::function<bool(TabItem&)> OnTabActive;
        // 重新排列标签页回调，参数为用户拖动的标签页，当标签页列表被用户拖动并重新排列时调用
        std::function<void(TabItem&)> OnTabRearrange;

    } actions;

    TabItem& PushbackTab(const TabItem& tab){
        tabs.push_back(tab);
        return tabs.back();
    };

    TabItem& PushfrontTab(const TabItem& tab){
        tabs.insert(tabs.begin(),tab);
        return tabs.front();
    };

    // 移除标签页（不触发 OnTabClose），并修正激活状态
    void EraseTab(const std::string& id){
        auto fd = FindTab(id);
        RemoveTabAt((int)std::distance(tabs.begin(), fd));
    };

    // 激活标签页，无视Actions回调结果，传空字符串表示取消激活任何标签
    void ActivateTab(const std::string& id){
        if(id == active_tab_id) return;
        SDL_Event event = {0};
        event.type = SDL_EVENT_USER;
        event.user.timestamp = SDL_GetTicksNS();
        if(id.empty()){
            try{
                auto prev_fd = FindTab(active_tab_id);
                event.user.code = SDL_USEREVENT_CODE_TAB_DEACTIVATE;
                prev_fd->content->MaintainEvent(&event,{});
                prev_fd->content->ClearInvaildRect();
            }
            catch(...){
                //ignore it
            }
            active_tab_id = "";
            this->InvalidateRect();
            return;
        }
        try{
            auto prev_fd = FindTab(active_tab_id);
            event.user.code = SDL_USEREVENT_CODE_TAB_DEACTIVATE;
            prev_fd->content->MaintainEvent(&event,{});
            prev_fd->content->ClearInvaildRect();
        }
        catch(...){
            //ignore it
        }
        auto fd = FindTab(id);
        active_tab_id = id;
        event.user.code = SDL_USEREVENT_CODE_TAB_ACTIVATE;
        fd->content->MaintainEvent(&event,{});
        fd->content->InvalidateRect();
        this->InvalidateRect();
    };

    // 置前标签页，但不激活它
    void TopTab(const std::string& id){
        auto fd = FindTab(id);
        std::swap(*fd,*tabs.begin());
    };

    // 置后标签页，但不激活它
    void BottomTab(const std::string& id){
        auto fd = FindTab(id);
        std::swap(*fd,*(tabs.end() - 1));
    };

    int GetTabIndex(const std::string& id){
        auto fd = FindTab(id);
        return std::distance(tabs.begin(),fd);
    };

    size_t GetTabCount() const{
        return tabs.size();
    };

    void ClearTabs(){
        SDL_Event event = {0};
        event.type = SDL_EVENT_USER;
        event.user.timestamp = SDL_GetTicksNS();
        try{
            auto prev_fd = FindTab(active_tab_id);
            event.user.code = SDL_USEREVENT_CODE_TAB_DEACTIVATE;
            prev_fd->content->MaintainEvent(&event,{});
            prev_fd->content->ClearInvaildRect();
        }
        catch(...){
            //ignore it
        }
        tabs.clear();
        active_tab_id = "";
        hover_tab_index = -1;
        press_tab_index = -1;
        drag_tab_index = -1;
        pending_close_index = -1;
        dragging_tabs = false;
        this->InvalidateRect();
    };

    // 返回当前激活的标签页；没有激活任何标签页时返回 nullptr
    TabItem* GetActiveTab(){
        if(active_tab_id.empty()) return nullptr;
        try{
            auto fd = FindTab(active_tab_id);
            return &*fd;
        }
        catch(...){
            return nullptr;
        }
    };

    TabItem& GetTab(const std::string& id){
        auto fd = FindTab(id);
        return *fd;
    };

    // 修改字体与字号
    void SetFont(FontEx* _font,int _ptsize){
        this->InvalidateRect();
        font = _font;
        ptsize = _ptsize;
        this->InvalidateRect();
    };

    // 修改标签页条位置（TAB_FLAG_POS_TOP/BOTTOM/LEFT/RIGHT）
    void SetStripPosition(int pos_flag){
        int new_pos = pos_flag & TAB_FLAG_POS_MASK;
        if(new_pos != (flags & TAB_FLAG_POS_MASK)){
            flags = (flags & ~TAB_FLAG_POS_MASK) | new_pos;
            this->InvalidateRect();
        }
    };

    // 当前是否允许拖动重排
    bool IsAllowRearrange() const{
        return (flags & TAB_FLAG_ALLOW_REARRANGE) != 0;
    };

    // 开关拖动重排
    void SetAllowRearrange(bool allow){
        if(allow) flags |= TAB_FLAG_ALLOW_REARRANGE;
        else flags &= ~TAB_FLAG_ALLOW_REARRANGE;
        dragging_tabs = false;
        drag_tab_index = -1;
    };

    //---------- 渲染 ----------

    void ActionOnPaint(SDL_Renderer* renderer,const SDL_Rect& dirty_region,const SDL_Rect& relative_rect) override{
        SDL_Rect draw_area = default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        SDL_Rect strip = CalcStripRect();      // 本控件本地坐标
        SDL_Rect content = CalcContentRect();  // 本控件本地坐标

        // 1. 标签页条背景
        SDL_Rect strip_screen = {draw_area.x + strip.x, draw_area.y + strip.y, strip.w, strip.h};
        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColorDarker));
    SDL_FRect __fr_44 = toFRect(strip_screen);
        SDL_RenderFillRect(renderer, &__fr_44);

        // 2. 各标签页
        for(int i = 0; i < (int)tabs.size(); i++){
            SDL_Rect tab = CalcTabRect(i);
            tab.x += draw_area.x;
            tab.y += draw_area.y;
            PaintTab(renderer, tab, i);
        }

        // 3. 内容区背景与边框（脏区不含内容区时这些绘制会被裁剪掉，无副作用）
        SDL_Rect content_screen = {draw_area.x + content.x, draw_area.y + content.y, content.w, content.h};
        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColor));
    SDL_FRect __fr_45 = toFRect(content_screen);
        SDL_RenderFillRect(renderer, &__fr_45);
        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
    SDL_FRect __fr_46 = toFRect(content_screen);
        SDL_RenderRect(renderer, &__fr_46);

        // 4. 把本控件脏区与内容区的交集投递给激活页并渲染
        if(!active_tab_id.empty() && content.w > 0 && content.h > 0){
            try{
                auto fd = FindTab(active_tab_id);
                auto& cbox = fd->content;
                cbox->default_rect = {0,0,content.w,content.h};
                SDL_Rect dirty_content = RectIntersection(dirty_region, content);
                if(dirty_content.w > 0 && dirty_content.h > 0){
                    SDL_Rect nd = {
                        dirty_content.x - content.x,
                        dirty_content.y - content.y,
                        dirty_content.w,
                        dirty_content.h
                    };
                    cbox->InvalidateRect(nd);
                    cbox->MaintainRender(renderer, content_screen);
                }
            }
            catch(...){
                //ignore it
            }
        }
    }

    //---------- 事件 ----------

    void ActionOnEvent(const SDL_Event* event,const SDL_Rect& relative_rect) override{
        // 本控件屏幕矩形（事件 relative_rect 不含 default_rect.xy）
        SDL_Rect ctl_screen = {relative_rect.x + default_rect.x, relative_rect.y + default_rect.y, default_rect.w, default_rect.h};

        // 鼠标位置 -> 本控件本地坐标
        SDL_Point mpt = {-99999,-99999};
        if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN || event->type == SDL_EVENT_MOUSE_BUTTON_UP){
            mpt = {(int)event->button.x, (int)event->button.y};
        }
        else if(event->type == SDL_EVENT_MOUSE_MOTION){
            mpt = {(int)event->motion.x, (int)event->motion.y};
        }
        else if(event->type == SDL_EVENT_MOUSE_WHEEL){
            float mxf, myf;
            SDL_GetMouseState(&mxf, &myf);
            mpt = {(int)mxf, (int)myf};
        }
        SDL_Point lpt = RelativizePoint(mpt, ctl_screen);

        if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT){
            int idx = HitTestTab(lpt);
            if(idx >= 0){
                press_tab_index = idx;
                pending_close_index = -1;
                if(HitTestCloseButton(lpt, idx)){
                    pending_close_index = idx;
                    drag_tab_index = -1;
                    dragging_tabs = false;
                }
                else if(IsAllowRearrange()){
                    drag_tab_index = idx;
                    drag_start = lpt;
                    dragging_tabs = false;
                }
                else{
                    drag_tab_index = -1;
                    dragging_tabs = false;
                }
                this->InvalidateRect();
            }
            else{
                press_tab_index = -1;
                drag_tab_index = -1;
                dragging_tabs = false;
                pending_close_index = -1;
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_MOTION){
            int idx = HitTestTab(lpt);
            if(idx != hover_tab_index){
                hover_tab_index = idx;
                this->InvalidateRect();
            }
            if(drag_tab_index >= 0 && IsAllowRearrange()){
                if(!dragging_tabs){
                    if(std::abs(lpt.x - drag_start.x) + std::abs(lpt.y - drag_start.y) >= 5){
                        dragging_tabs = true;
                        this->InvalidateRect();
                    }
                }
                if(dragging_tabs){
                    bool moved = false;
                    for(int guard = 0; guard < (int)tabs.size(); guard++){
                        int cur = drag_tab_index;
                        if(cur < 0) break;
                        if(cur > 0){
                            SDL_Rect prev_r = CalcTabRect(cur - 1);
                            if(IsBeforeCenter(lpt, prev_r)){
                                MoveTab(cur, cur - 1);
                                moved = true;
                                continue;
                            }
                        }
                        if(cur < (int)tabs.size() - 1){
                            SDL_Rect next_r = CalcTabRect(cur + 1);
                            if(IsBeyondCenter(lpt, next_r)){
                                MoveTab(cur, cur + 1);
                                moved = true;
                                continue;
                            }
                        }
                        break;
                    }
                    if(moved){
                        CallIfNotNull(actions.OnTabRearrange, tabs[drag_tab_index]);
                        this->InvalidateRect();
                    }
                }
            }
        }
        else if(event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT){
            if(dragging_tabs){
                // 拖动结束
                dragging_tabs = false;
                drag_tab_index = -1;
                press_tab_index = -1;
                pending_close_index = -1;
                this->InvalidateRect();
            }
            else if(pending_close_index >= 0){
                if(HitTestCloseButton(lpt, pending_close_index)){
                    DoCloseTab(pending_close_index);
                }
                pending_close_index = -1;
                press_tab_index = -1;
                drag_tab_index = -1;
            }
            else if(press_tab_index >= 0){
                int idx = press_tab_index;
                bool ok = true;
                if(actions.OnTabActive){
                    ok = actions.OnTabActive(tabs[idx]);
                }
                if(ok){
                    ActivateTab(tabs[idx].id);
                }
                press_tab_index = -1;
                drag_tab_index = -1;
                this->InvalidateRect();
            }
            pending_close_index = -1;
        }

        // 向激活页转发事件（ControlBox 自带区域过滤，落在条上的鼠标事件不会穿透到内容区）
        // 必须先同步激活页 default_rect 到内容区尺寸，否则 ControlBox 事件命中会用旧尺寸（如初始 0 尺寸）导致内部控件点不动
        if(!active_tab_id.empty()){
            SDL_Rect content_screen = SyncActiveContentRect(ctl_screen);
            if(content_screen.w > 0 && content_screen.h > 0){
                try{
                    auto fd = FindTab(active_tab_id);
                    fd->content->MaintainEvent(event, content_screen);
                    // 激活页 ControlBox（或其子树）消费了滚轮则向上层报告，父容器不再滚动自身
                    if(event->type == SDL_EVENT_MOUSE_WHEEL){
                        wheel_consumed = fd->content->IsWheelConsumed();
                    }
                }
                catch(...){
                    //ignore it
                }
            }
        }
    }

    //---------- 定时器 ----------

    void ActionOnTimer(const SDL_Rect& relative_rect) override{
        // 同步激活页 default_rect 到内容区尺寸，并传播其脏区到本控件（内容区内的子控件脏区需要由本控件触发重绘）
        if(!active_tab_id.empty()){
            try{
                auto fd = FindTab(active_tab_id);
                auto& cbox = fd->content;
                SDL_Rect content = CalcContentRect();
                cbox->default_rect = {0,0,content.w,content.h};
                // 关键：接上激活页 content box 的定时器。cbox 是 ControlBox，其 ActionOnTimer 内
                // 的 TestInvalidateRect 会把内容区子控件(按钮等)invalidate 的脏区汇总到 cbox 自身；
                // 若不调用 MaintainTimer，子控件脏区永远卡在子控件里传不上来，内容区将永不重绘
                // （表现：按钮按下/hover 无任何视觉反馈，看似"点不动/没反应"）。
                // ★必须传 cbox 局部坐标 {0,0,w,h}，绝不能传屏幕坐标：TestInvalidateRect 把汇总
                // 的脏区当作 cbox 局部坐标写入 dirty_region，ControlBox::InvalidateRect 内部也按
                // 局部坐标相交/clamp；传屏幕坐标会导致 dirty_region 被 clamp 空、且传播不到子控件。
                cbox->MaintainTimer({0,0,content.w,content.h});
                // 汇总 cbox 自身的脏区（含其子控件上传的脏区）并传播到本控件以触发重绘
                SDL_Rect cd = cbox->GetRelativeInvaildRect();
                if(cd.w > 0 && cd.h > 0){
                    SDL_Rect abs_dirty = {content.x + cd.x, content.y + cd.y, cd.w, cd.h};
                    this->InvalidateRect(abs_dirty);
                }
            }
            catch(...){
                //ignore it
            }
        }

        // hover 过渡动画
        if(enable_animation){
            SDL_Color target = (hover_tab_index >= 0) ? color_kit.ForegroundColorDarker : color_kit.BackgroundColorLighter;
            bool changed;
            hover_bg_color.process(target, changed);
            if(changed){
                this->InvalidateRect();
            }
        }
    }

    std::string ActionOnGetTypeName() const override{
        return "Fgui.TabControl";
    }

    void ActionOnColorKitChange(const Fgui_ColorKit& ck) override{
        color_kit = ck;
        hover_bg_color.setLastValue(color_kit.BackgroundColorLighter);
        for(auto& tab : tabs){
            tab.content->ActionOnColorKitChange(ck);
        }
        this->InvalidateRect();
    }
};

//TODO:List,KeyBind

#undef CallIfNotNull
#undef SetValueIfNotNull

#endif
