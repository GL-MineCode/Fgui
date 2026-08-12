#ifndef __INC_SDLWINDOWSTUFF_
#define __INC_SDLWINDOWSTUFF_

#include<SDL.h>
#include<string>
#include<vector>

#ifdef _WIN32

#include<windows.h>
#include <windowsx.h>
#include "AdvancedSDLWindow.hpp"
#include "GL_DWMoperations.hpp"
#include<imm.h>

namespace{

    void SetIMState(HWND hwnd,bool state){
        HIMC himc = ImmGetContext(hwnd);
        ImmSetOpenStatus(himc, state);
        ImmReleaseContext(hwnd, himc); 
    }

}

void SDLCALL WindowHookOnResizableFunction(void *userdata, void *hWnd, unsigned int message, Uint64 wParam, Sint64 lParam);

#endif

class SDLWindowStuff {
	public:
		SDL_Window* native_handle;
#ifdef _WIN32
		HWND hwnd;
	private:
		bool Init_Windows32() {
			hwnd = ASW_GetWindowHandleFromSDLWindow(native_handle);
			return hwnd != NULL;
		}
        SDLWindowStuff(const std::string& title, int x, int y, int w, int h, Uint32 flags) 
        {
			native_handle = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
            if (!native_handle) return; // 构造可能失败
            #ifdef _WIN32
                    Init_Windows32();
            #endif
        }

        // 私有析构函数
        ~SDLWindowStuff() {
            // 确保 Release() 未清理时兜底
            if (native_handle) SDL_DestroyWindow(native_handle);
        }
	public:

        static SDLWindowStuff* Create(const std::string& title, int w, int h, Uint32 flags,int x = SDL_WINDOWPOS_CENTERED,int y = SDL_WINDOWPOS_CENTERED) {
            return new SDLWindowStuff(title, x, y, w, h, flags);
        }

		//The function may only work on Windows11
		void SetTitleColor(SDL_Color color) {
			GL_DWMO::SetTextColor(hwnd, RGB(color.r, color.g, color.b));
		}

		//The function may only work on Windows11
		void SetCaptionColor(SDL_Color color) {
			GL_DWMO::SetCaptionColor(hwnd, RGB(color.r, color.g, color.b));
		}

		//The function may only work on Windows11
		void SetBorderColor(SDL_Color color) {
			GL_DWMO::SetBorderColor(hwnd, RGB(color.r, color.g, color.b));
		}

		//The function may only work on Windows11
		// 0:Default 1:Not round 2:Round 3:Small round
		void SetPreference(int state) {
			GL_DWMO::SetCornerPreference(hwnd, (GL_DWMO::DWM_WINDOW_CORNER_PREFERENCE)state);
		}

		//The function may only work on Windows11
		void SetDarkMode(bool isDark) {
			GL_DWMO::SetDarkMode(hwnd, isDark);
		}

		//The function may only work on Windows11
		//In fact,SDL cover all the window and DO NOT render the background
		//If you can stop it from being covered by SDL,the function might work.
		void SetBackground(GL_DWMO::DWM_SYSTEMBACKDROP_TYPE type) {
			GL_DWMO::SetBackground(hwnd,type);
		}

		void SetLayered(bool state){
			LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);  
			if (state) {  
				exStyle |= (WS_EX_LAYERED);  
			} else {  
				exStyle &= ~(WS_EX_LAYERED);  
			}
			SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle); 
		}

		void SetAlpha(unsigned char alpha){
			SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
		}

		void SetColorkey(SDL_Color color){
			SetLayeredWindowAttributes(hwnd, RGB(color.r, color.g, color.b), 0, LWA_COLORKEY);
		}

		void SetToolWindow(bool state){
			LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);  
			if (state) {  
				exStyle |= (WS_EX_TOOLWINDOW);  
			} else {  
				exStyle &= ~(WS_EX_TOOLWINDOW);  
			}
			SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle); 
		}

		void SetTopmost(){
			SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
		}

		void SetIMEnabled(bool state){
			SetIMState(hwnd,state);
		}

		void ForceTopmost(){
			HWND hForeWnd = NULL;
			DWORD dwForeID = 0;
			DWORD dwCurID = 0;
			hForeWnd = GetForegroundWindow();
			dwCurID = GetCurrentThreadId();
			dwForeID = GetWindowThreadProcessId(hForeWnd, NULL);
			AttachThreadInput(dwCurID, dwForeID, TRUE);
			ShowWindow(hwnd, SW_SHOWNORMAL);
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			SetForegroundWindow(hwnd);
			AttachThreadInput(dwCurID, dwForeID, FALSE);
		}

		void ForceTop(){
			HWND hForeWnd = NULL;
			DWORD dwForeID = 0;
			DWORD dwCurID = 0;
			hForeWnd = GetForegroundWindow();
			dwCurID = GetCurrentThreadId();
			dwForeID = GetWindowThreadProcessId(hForeWnd, NULL);
			AttachThreadInput(dwCurID, dwForeID, TRUE);
			ShowWindow(hwnd, SW_SHOWNORMAL);
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			SetForegroundWindow(hwnd);
			AttachThreadInput(dwCurID, dwForeID, FALSE);
		}

		//给WindowHookOnResizableFunction用的，别动这个成员
		struct BorderlessResizeableData{
			bool isResizing = false;
			POINT resizeStartPoint;
			SDL_Rect resizeStartRect;
			int resizeStartEdge = 0;
		} brd;

		//注意！这个函数会覆盖您的默认SDL窗口钩子，若您仍然想使用该功能，可以在你的窗口钩子的开头调用WindowHookOnResizableFunction，并给该函数的userdata传入该类的指针
		void EnableBorderlessResizeable(){
			SDL_SetWindowsMessageHook(WindowHookOnResizableFunction,this);
		}

#endif
		void Release() {
            if (native_handle) {
                SDL_DestroyWindow(native_handle);
                native_handle = nullptr;
            }
            delete this;  // 关键：自毁操作
        }
		void Hide(){
			SDL_HideWindow(native_handle);
		}
		void Show(){
			SDL_ShowWindow(native_handle);
		}
		void SetPos(int x,int y){
			SDL_SetWindowPosition(native_handle,x,y);
		}
		void SetSize(int w,int h){
			SDL_SetWindowSize(native_handle,w,h);
		}
		void SetFullscreen(bool state,bool use_soft_fullscreen){
			if(state){
				SDL_SetWindowFullscreen(native_handle,SDL_WINDOW_FULLSCREEN | (use_soft_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
			}else{
				SDL_SetWindowFullscreen(native_handle,0);
			}	
		}
		Uint32 GetID(){
			return SDL_GetWindowID(native_handle);
		}
		SDL_Renderer* CreateRenderer(Uint32 flags = SDL_RENDERER_TARGETTEXTURE){
			return SDL_CreateRenderer(native_handle,-1,flags);
		}
};

#ifdef _WIN32

// 0=无, 1=左, 2=右, 3=上, 4=下, 5=左上, 6=右上, 7=左下, 8=右下

// 检测鼠标在哪个边缘
static int GetResizeEdge(int x, int y, int windowWidth, int windowHeight)
{
    const int borderSize = 8; // 边缘检测区域大小
    
    bool left = (x < borderSize);
    bool right = (x > windowWidth - borderSize);
    bool top = (y < borderSize);
    bool bottom = (y > windowHeight - borderSize);
    
    if (left && top) return 5;      // 左上
    if (right && top) return 6;     // 右上
    if (left && bottom) return 7;    // 左下
    if (right && bottom) return 8;   // 右下
    if (left) return 1;             // 左
    if (right) return 2;            // 右
    if (top) return 3;              // 上
    if (bottom) return 4;           // 下
    
    return 0;                       // 不在边缘
}

// 设置鼠标光标样式
static void SetResizeCursor(int edge)
{
    HCURSOR newCursor = NULL;
    
    switch (edge) {
        case 1: // 左
        case 2: // 右
            newCursor = LoadCursor(NULL, IDC_SIZEWE);
            break;
        case 3: // 上
        case 4: // 下
            newCursor = LoadCursor(NULL, IDC_SIZENS);
            break;
        case 5: // 左上
        case 8: // 右下
            newCursor = LoadCursor(NULL, IDC_SIZENWSE);
            break;
        case 6: // 右上
        case 7: // 左下
            newCursor = LoadCursor(NULL, IDC_SIZENESW);
            break;
        default:
            newCursor = LoadCursor(NULL, IDC_ARROW);
            break;
    }
    SetCursor(newCursor);
}

// 更新SDL窗口大小
static void UpdateSDLWindowSize(HWND hwnd,int x,int y,SDLWindowStuff* sws,bool do_update_renderer_draw_area)
{
	SDL_Rect newRect = sws->brd.resizeStartRect;
	POINT pt;
	GetCursorPos(&pt);
	int delta_x = sws->brd.resizeStartPoint.x - pt.x;
	int delta_y = sws->brd.resizeStartPoint.y - pt.y;
	switch(sws->brd.resizeStartEdge){
		case 1://左
		newRect.x -= delta_x;
		newRect.w += delta_x;
		break;

		case 2://右
		newRect.w -= delta_x;
		break;

		case 3://上
		newRect.y -= delta_y;
		newRect.h += delta_y;
		break;

		case 4://下
		newRect.h -= delta_y;
		break;

		case 5://左上
		newRect.x -= delta_x;
		newRect.w += delta_x;
		newRect.y -= delta_y;
		newRect.h += delta_y;
		break;

		case 6:// 右上
		newRect.w -= delta_x;
		newRect.y -= delta_y;
		newRect.h += delta_y;
		break;

		case 7://左下
		newRect.x -= delta_x;
		newRect.w += delta_x;
		newRect.h -= delta_y;
		break;

		case 8://右下
		newRect.w -= delta_x;
		newRect.h -= delta_y;
		break;

		default:
		return;
	}
	if(do_update_renderer_draw_area){
		// MoveWindow(hwnd,newRect.x,newRect.y,newRect.w,newRect.h,true);
		// PostMessage(hwnd,WM_WINDOWPOSCHANGED,NULL,NULL);

		/*
			HACK!!!
			没错我翻了SDL源码才搞了这种操作的，千万别学
		*/
		// {
		// 	char* hack = (char*)sws->native_handle;
		// 	hack += 40;//w成员的位置
		// 	*((int*)hack) = newRect.w;

		// 	hack = (char*)sws->native_handle;
		// 	hack += 44;//h成员的位置
		// 	*((int*)hack) = newRect.h;

		// 	hack = (char*)sws->native_handle;
		// 	hack += 76;//windowed成员的位置
		// 	((SDL_Rect*)hack)->w = newRect.w;
		// 	((SDL_Rect*)hack)->h = newRect.h;
		// }

		SDL_SetWindowSize(sws->native_handle,newRect.w,newRect.h);

		// SDL_Event event;
        // event.type = SDL_WINDOWEVENT;
        // event.window.event = SDL_WINDOWEVENT_SIZE_CHANGED;
        // event.window.data1 = newRect.w;
        // event.window.data2 = newRect.h;
        // event.window.windowID = SDL_GetWindowID(sws->native_handle);
		// SDL_PushEvent(&event);

		SDL_SetWindowPosition(sws->native_handle,newRect.x,newRect.y);
	}else{
		//MoveWindow(hwnd,newRect.x,newRect.y,newRect.w,newRect.h,false);
		SDL_SetWindowSize(sws->native_handle,newRect.w,newRect.h);
		SDL_SetWindowPosition(sws->native_handle,newRect.x,newRect.y);
	}
}

void SDLCALL WindowHookOnResizableFunction(void *userdata, void *hWnd, unsigned int message, Uint64 wParam, Sint64 lParam)
{
	SDLWindowStuff* sws = (SDLWindowStuff*)userdata;
    if (sws->hwnd != hWnd) return;
    
	static int windowWidth,windowHeight;
	SDL_GetWindowSize(sws->native_handle,&windowWidth,&windowHeight);

    switch (message) {
    case WM_MOUSEMOVE: {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        
        if (sws->brd.isResizing) {
			SetResizeCursor(sws->brd.resizeStartEdge);
			//UpdateSDLWindowSize(sws->hwnd,windowWidth,windowHeight,sws);
			UpdateSDLWindowSize((HWND)hWnd,x,y,sws,false);
        } else {
            // 检测鼠标位置并设置光标
            int edge = GetResizeEdge(x, y, windowWidth, windowHeight);
            SetResizeCursor(edge);
        }
        return;
    }
        
    case WM_LBUTTONDOWN: {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        
        sws->brd.resizeStartEdge = GetResizeEdge(x, y, windowWidth, windowHeight);
        if (sws->brd.resizeStartEdge != 0) {
            sws->brd.isResizing = true;
			POINT pt;
			GetCursorPos(&pt);
            sws->brd.resizeStartPoint.x = pt.x;
            sws->brd.resizeStartPoint.y = pt.y;
			SDL_GetWindowPosition(sws->native_handle,&sws->brd.resizeStartRect.x,&sws->brd.resizeStartRect.y);
			SDL_GetWindowSize(sws->native_handle,&sws->brd.resizeStartRect.w,&sws->brd.resizeStartRect.h);
            //GetWindowRect(sws->hwnd, &sws->brd.resizeStartRect);
            SetCapture(sws->hwnd);
        }
        return;
    }
        
    case WM_LBUTTONUP:
        if (sws->brd.isResizing) {
			int x = GET_X_LPARAM(lParam);
        	int y = GET_Y_LPARAM(lParam);
			UpdateSDLWindowSize((HWND)hWnd,x,y,sws,true);
            sws->brd.isResizing = false;
            sws->brd.resizeStartEdge = 0;
            ReleaseCapture();
        }
        return;
        
    case WM_SIZE:
        return;
        
    case WM_DESTROY:
        // 清理资源
        if (sws->brd.isResizing) {
            sws->brd.isResizing = false;
            ReleaseCapture();
        }
        return;
    }
}

#endif

#endif
