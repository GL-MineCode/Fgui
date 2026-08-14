#ifndef __INC_WINDOW_DC_TO_SDL_SURFACE_
#define __INC_WINDOW_DC_TO_SDL_SURFACE_

#include <Windows.h>
#include <SDL3/SDL.h>
#include <vector>

SDL_Surface* GetWindowSurface(HWND window) {
    HDC target = GetDC(window);
    if (!target) return nullptr;

    HDC hdcMemDC = CreateCompatibleDC(target);
    if (!hdcMemDC) {
        ReleaseDC(window, target);
        return nullptr;
    }

    RECT rect;
    if (!GetClientRect(window, &rect)) {
        DeleteDC(hdcMemDC);
        ReleaseDC(window, target);
        return nullptr;
    }

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    HBITMAP hBitmap = CreateCompatibleBitmap(target, width, height);
    if (!hBitmap) {
        DeleteDC(hdcMemDC);
        ReleaseDC(window, target);
        return nullptr;
    }

    HGDIOBJ oldBitmap = SelectObject(hdcMemDC, hBitmap);
    BitBlt(hdcMemDC, 0, 0, width, height, target, 0, 0, SRCCOPY);

    SDL_Surface* sur = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBX8888);
    if (sur) {
        BITMAPINFOHEADER bi = {0};
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = width;
        bi.biHeight = -height;  // Top-down
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;

        SDL_LockSurface(sur);
        GetDIBits(hdcMemDC, hBitmap, 0, height, sur->pixels, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
        SDL_UnlockSurface(sur);
    }

    SelectObject(hdcMemDC, oldBitmap);
    DeleteDC(hdcMemDC);
    ReleaseDC(window, target);
    DeleteObject(hBitmap);

    return sur;
}

class WindowCapturer {
    HWND window = nullptr;
    HDC target = nullptr;
    HDC hdcMemDC = nullptr;
    HBITMAP hBitmap = nullptr;
public:
    int width = 0;
    int height = 0;
    std::vector<BYTE> pixelBuffer;

    WindowCapturer(HWND wnd) : window(wnd) {
        RECT rect;
        if (!GetClientRect(window, &rect)) return;
        
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
        
        target = GetDC(window);
        if (!target) return;
        
        hdcMemDC = CreateCompatibleDC(target);
        if (!hdcMemDC) {
            ReleaseDC(window, target);
            target = nullptr;
            return;
        }
        
        hBitmap = CreateCompatibleBitmap(target, width, height);
        if (!hBitmap) {
            DeleteDC(hdcMemDC);
            ReleaseDC(window, target);
            hdcMemDC = nullptr;
            target = nullptr;
            return;
        }
        
        pixelBuffer.resize(width * height * 4);
    }

    ~WindowCapturer() {
        if (hdcMemDC) DeleteDC(hdcMemDC);
        if (target) ReleaseDC(window, target);
        if (hBitmap) DeleteObject(hBitmap);
    }

    void ReadPixel() {
        if (!hdcMemDC || !hBitmap) return;
        
        HGDIOBJ oldBitmap = SelectObject(hdcMemDC, hBitmap);
        BitBlt(hdcMemDC, 0, 0, width, height, target, 0, 0, SRCCOPY);
        
        BITMAPINFOHEADER bi = {0};
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = width;
        bi.biHeight = -height;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        
        GetDIBits(hdcMemDC, hBitmap, 0, height, pixelBuffer.data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS);
        SelectObject(hdcMemDC, oldBitmap);
    }

    SDL_Surface* GetSurface() const {
        if (pixelBuffer.empty()) return nullptr;
        return SDL_CreateSurfaceFrom(
            width, height, SDL_PIXELFORMAT_XRGB8888,
            (void*)pixelBuffer.data(), width * 4);
    }

    WindowCapturer(const WindowCapturer&) = delete;
    void operator=(const WindowCapturer&) = delete;
};

#endif // __INC_WINDOW_DC_TO_SDL_SURFACE_
