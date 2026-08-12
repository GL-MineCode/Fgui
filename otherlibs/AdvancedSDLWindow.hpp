#ifndef __INC_ADVANCED_SDL_WINDOW_
#define __INC_ADVANCED_SDL_WINDOW_
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0A00
#include<windows.h>
namespace ASW_API_WIN32 {
	inline int SetWindowBoradless(HWND hwnd) {
		return SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~(WS_CAPTION | WS_SIZEBOX));
	}
	//Make sure the window would not appear in the task bar
	inline int SetWindowToolWindow(HWND hwnd) {
		return SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) |(WS_EX_TOOLWINDOW));
	}
	inline int SetWindowLayered(HWND hwnd) {
		return SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	}
	inline int SetWindowTransparent(HWND hwnd) {
		return SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
	}
	inline int SetWindowNoactive(HWND hwnd) {
		return SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_NOACTIVATE);
	}
	inline bool SetWindowSafety(HWND hwnd,bool enabled){
		return SetWindowDisplayAffinity(hwnd,enabled ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);
	}
	inline bool GetWindowSafety(HWND hwnd){
		DWORD aff;
		GetWindowDisplayAffinity(hwnd,&aff);
		return aff == WDA_EXCLUDEFROMCAPTURE;
	}
//Tip:use SetWindowLayered before using it
	inline int SetWindowAlpha(HWND hwnd, BYTE alpha) {
		return SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
	}
//Tip:use SetWindowLayered before using it
	inline int SetWindowColorKey(HWND hwnd, unsigned char r, unsigned char g, unsigned char b) {
		return SetLayeredWindowAttributes(hwnd, RGB(r, g, b), 0, LWA_COLORKEY);
	}
	inline int SetWindowTopMost(HWND taget) {
		return SetWindowPos(taget, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}

	inline int CloseWindow(HWND window) {
		SendMessageA(window, WM_CLOSE, 0, 0);
		return 0;
	}
}
#include<SDL.h>
#include<SDL_syswm.h>

HWND ASW_GetWindowHandleFromSDLWindow(SDL_Window* win){
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(win,&info);
    return info.info.win.window;
}

#endif
