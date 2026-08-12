#ifndef __INC_DWM_OPERATIONS_
#define __INC_DWM_OPERATIONS_
#include<Windows.h>
#include<dwmapi.h>

//Windows 11 required!
namespace GL_DWMO{
	
	enum DWM_WINDOW_CORNER_PREFERENCE{
		/*
		* Let the system decide whether or not to round window corners
		*/
		DWMWCP_DEFAULT                                 = 0,

		/*
		* Never round window corners
		*/
		DWMWCP_DONOTROUND                              = 1,

		/*
		* Round the corners if appropriate
		*/
		DWMWCP_ROUND                                   = 2,

		/*
		* Round the corners if appropriate, with a small radius
		*/
		DWMWCP_ROUNDSMALL                              = 3

	};

	enum DWM_SYSTEMBACKDROP_TYPE
	{
		DWMSBT_AUTO,             // [Default] Let DWM automatically decide the system-drawn backdrop for this window.
		DWMSBT_NONE,             // Do not draw any system backdrop.
		DWMSBT_MAINWINDOW,       // Draw the backdrop material effect corresponding to a long-lived window.
		DWMSBT_TRANSIENTWINDOW,  // Draw the backdrop material effect corresponding to a transient window.
		DWMSBT_TABBEDWINDOW,     // Draw the backdrop material effect corresponding to a window with a tabbed title bar.
	};

	void SetDarkMode(HWND hwnd,const BOOL isDark){
		DwmSetWindowAttribute(hwnd,20,&isDark,sizeof(BOOL));
	}

	void SetTextColor(HWND hwnd,const COLORREF color){
		DwmSetWindowAttribute(hwnd,36,&color,sizeof(COLORREF));
	}

	void SetCaptionColor(HWND hwnd,const COLORREF color){
		DwmSetWindowAttribute(hwnd,35,&color,sizeof(COLORREF));
	}

	void SetCornerPreference(HWND hwnd,const DWM_WINDOW_CORNER_PREFERENCE wcp){
		DwmSetWindowAttribute(hwnd,33,&wcp,sizeof(DWM_WINDOW_CORNER_PREFERENCE));
	}

	void SetBorderColor(HWND hwnd,const COLORREF color){
		DwmSetWindowAttribute(hwnd,34,&color,sizeof(COLORREF));
	}

	void SetBackground(HWND hwnd,const DWM_SYSTEMBACKDROP_TYPE st){
		DwmSetWindowAttribute(hwnd,38,&st,sizeof(DWM_SYSTEMBACKDROP_TYPE));
	}

}



#endif
