#ifndef __INC_SDL_EVENTPLUS
#define __INC_SDL_EVENTPLUS
#include <SDL3/SDL.h>
#include <cstring>
class EventPlus
{
private:
	SDL_Event *base;
	bool key_state[SDL_SCANCODE_COUNT];
	bool key_p_state[SDL_SCANCODE_COUNT];
	// SDL3的时间戳与SDL_GetTicks()均为Uint64
	Uint64 key_p_time[SDL_SCANCODE_COUNT];
	bool LMB_state;
	bool RMB_state;
	bool MMB_state;
	bool LMB_p_state;
	bool RMB_p_state;
	bool MMB_p_state;
	SDL_Point LMB_p_pos;
	SDL_Point RMB_p_pos;
	SDL_Point MMB_p_pos;
	Uint64 LMB_p_time;
	Uint64 RMB_p_time;
	Uint64 MMB_p_time;
	Sint32 wheel_state;
	SDL_Point mousepos;
	Uint64 mouse_move_time;
	SDL_Point mouse_move_offset;
	SDL_Window* win;
	SDL_Rect windowsize;
public:
	EventPlus()
	{
		this->Init(NULL,NULL);
	}
	EventPlus(SDL_Event *res,SDL_Window* windowToGet = NULL)
	{
		this->Init(res,windowToGet);
	}
	void Init(SDL_Event *res,SDL_Window* windowToGet){
		this->base = res;
		this->win = windowToGet;
		memset(key_state, false, sizeof(key_state));

		LMB_state = false;
		RMB_state = false;
		MMB_state = false;
		LMB_p_state = false;
		RMB_p_state = false;
		MMB_p_state = false;
		LMB_p_pos = {0,0};
		RMB_p_pos = {0,0};
		MMB_p_pos = {0,0};
		LMB_p_time = 0;
		RMB_p_time = 0;
		MMB_p_time = 0;
		wheel_state = 0;
		mousepos = {0,0};
		mouse_move_time = 0;
		mouse_move_offset = {0,0};
		if(windowToGet){
			SDL_GetWindowPosition(windowToGet,&windowsize.x,&windowsize.y);
			SDL_GetWindowSize(windowToGet,&windowsize.w,&windowsize.h);
		}
		else{
			windowsize = {0,0,0,0};
		}
	}
	inline void reset(void)
	{
		this->wheel_state = 0;
		this->LMB_p_state = false;
		this->RMB_p_state = false;
		this->MMB_p_state = false;
		mouse_move_offset = {0,0};
		memset(key_p_state, false, sizeof(key_p_state));
	}

	inline void viewEvent(){
		if (this->base->type == SDL_EVENT_KEY_DOWN)
		{
			if(win && this->base->key.windowID != SDL_GetWindowID(win)) return;

			// SDL3中SDL_KeyboardEvent不再有keysym结构体，scancode直接位于key事件上
			SDL_Scancode sc = this->base->key.scancode;
			if (!key_state[sc])
			{
				key_p_state[sc] = true;
				key_state[sc] = true;
				key_p_time[sc] = this->base->key.timestamp;
			}
		}
		else if (this->base->type == SDL_EVENT_KEY_UP)
		{
			if(win && this->base->key.windowID != SDL_GetWindowID(win)) return;

			SDL_Scancode sc = this->base->key.scancode;
			key_state[sc] = false;
		}
		else if (this->base->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			if(win && this->base->button.windowID != SDL_GetWindowID(win)) return;

			if (this->base->button.button == SDL_BUTTON_LEFT)
			{
				LMB_p_state = true;
				LMB_state = true;
				// SDL3中按钮坐标x/y为float
				LMB_p_pos = {static_cast<int>(this->base->button.x),static_cast<int>(this->base->button.y)};
				LMB_p_time = this->base->button.timestamp;
			}
			else if (this->base->button.button == SDL_BUTTON_RIGHT)
			{
				RMB_p_state = true;
				RMB_state = true;
				RMB_p_pos = {static_cast<int>(this->base->button.x),static_cast<int>(this->base->button.y)};
				RMB_p_time = this->base->button.timestamp;
			}else if(this->base->button.button == SDL_BUTTON_MIDDLE){
				MMB_p_state = true;
				MMB_state = true;
				MMB_p_pos = {static_cast<int>(this->base->button.x),static_cast<int>(this->base->button.y)};
				MMB_p_time = this->base->button.timestamp;
			}
		}
		else if (this->base->type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			if(win && this->base->button.windowID != SDL_GetWindowID(win)) return;

			if (this->base->button.button == SDL_BUTTON_LEFT)
			{
				LMB_state = false;
			}
			else if (this->base->button.button == SDL_BUTTON_RIGHT)
			{
				RMB_state = false;
			}
			else if (this->base->button.button == SDL_BUTTON_MIDDLE)
			{
				MMB_state = false;
			}
		}
		else if (this->base->type == SDL_EVENT_MOUSE_WHEEL)
		{
			if(win && this->base->wheel.windowID != SDL_GetWindowID(win)) return;

			// SDL3中wheel.y为float，且保留了direction字段
			if (this->base->wheel.direction == SDL_MOUSEWHEEL_NORMAL)
				this->wheel_state = static_cast<Sint32>(this->base->wheel.y);
			else
				this->wheel_state = -static_cast<Sint32>(this->base->wheel.y);
		}
		else if (this->base->type == SDL_EVENT_MOUSE_MOTION)
		{
			if(win && this->base->motion.windowID != SDL_GetWindowID(win)) return;

			// SDL3中motion.x/y为float
			this->mousepos = {static_cast<int>(this->base->motion.x), static_cast<int>(this->base->motion.y)};
			mouse_move_time = this->base->motion.timestamp;

			mouse_move_offset.x += static_cast<int>(this->base->motion.xrel);
			mouse_move_offset.y += static_cast<int>(this->base->motion.yrel);
		}
		else if(this->base->type == SDL_EVENT_WINDOW_RESIZED){
			if(win && this->base->window.windowID != SDL_GetWindowID(win)) return;

			this->windowsize.w = this->base->window.data1;
			this->windowsize.h = this->base->window.data2;
		}
		else if(this->base->type == SDL_EVENT_WINDOW_MOVED){
			if(win && this->base->window.windowID != SDL_GetWindowID(win)) return;

			this->windowsize.x = this->base->window.data1;
			this->windowsize.y = this->base->window.data2;
		}

		if(!win) return;
		//解决SDL的一个bug，鼠标捕获不触发SDL_EVENT_MOUSE_BUTTON_UP事件
		//SDL3中SDL_GetGlobalMouseState接受float*输出坐标
		float mouse_global_x = 0.0f;
		float mouse_global_y = 0.0f;
		Uint32 buttons = SDL_GetGlobalMouseState(&mouse_global_x,&mouse_global_y);
		if((buttons & SDL_BUTTON_LMASK) == 0 && LMB_state){
			SDL_Event fakeUp{0};
			fakeUp.type = SDL_EVENT_MOUSE_BUTTON_UP;
			fakeUp.button.button = SDL_BUTTON_LEFT;
			fakeUp.button.x = -1;
			fakeUp.button.y = -1;
			fakeUp.button.clicks = 1;
			fakeUp.button.windowID = SDL_GetWindowID(win);
			// SDL3中SDL_MouseButtonEvent使用bool down代替Uint8 state
			fakeUp.button.down = false;
			fakeUp.button.timestamp = SDL_GetTicks();
			fakeUp.button.which = SDL_TOUCH_MOUSEID;
			SDL_PushEvent(&fakeUp);
		}
		if((buttons & SDL_BUTTON_RMASK) == 0 && RMB_state){
			SDL_Event fakeUp{0};
			fakeUp.type = SDL_EVENT_MOUSE_BUTTON_UP;
			fakeUp.button.button = SDL_BUTTON_RIGHT;
			fakeUp.button.x = -1;
			fakeUp.button.y = -1;
			fakeUp.button.clicks = 1;
			fakeUp.button.windowID = SDL_GetWindowID(win);
			fakeUp.button.down = false;
			fakeUp.button.timestamp = SDL_GetTicks();
			fakeUp.button.which = SDL_TOUCH_MOUSEID;
			SDL_PushEvent(&fakeUp);
		}
		if((buttons & SDL_BUTTON_MMASK) == 0 && MMB_state){
			SDL_Event fakeUp{0};
			fakeUp.type = SDL_EVENT_MOUSE_BUTTON_UP;
			fakeUp.button.button = SDL_BUTTON_MIDDLE;
			fakeUp.button.x = -1;
			fakeUp.button.y = -1;
			fakeUp.button.clicks = 1;
			fakeUp.button.windowID = SDL_GetWindowID(win);
			fakeUp.button.down = false;
			fakeUp.button.timestamp = SDL_GetTicks();
			fakeUp.button.which = SDL_TOUCH_MOUSEID;
			SDL_PushEvent(&fakeUp);
		}
	}

	inline int waitpoll(void)
	{
		// SDL3中SDL_WaitEvent返回bool，此处保持int返回类型以兼容原有调用
		int ret = SDL_WaitEvent(this->base);
		this->viewEvent();
		return ret;
	}

	inline int poll(void)
	{
		// SDL3中SDL_PollEvent返回bool，此处保持int返回类型以兼容原有调用
		int ret = SDL_PollEvent(this->base);
		this->viewEvent();
		return ret;
	}
	inline bool press(SDL_Scancode scancode)
	{
		return this->key_p_state[scancode];
	}
	inline bool hold(SDL_Scancode scancode)
	{
		return this->key_state[scancode];
	}
	inline bool holdLonger(SDL_Scancode scancode,Uint32 time_req)
	{
		return (SDL_GetTicks() - this->key_p_time[scancode]) > time_req;
	}
	inline Uint64 holdTime(SDL_Scancode scancode)
	{
		return this->key_p_time[scancode];
	}
	inline bool LMBhold()
	{
		return this->LMB_state;
	}
	inline bool LMBlonger(Uint32 time_req)
	{
		return (SDL_GetTicks() - this->LMB_p_time) > time_req;
	}
	inline Uint64 LMBtime()
	{
		return this->LMB_p_time;
	}
	inline bool RMBhold()
	{
		return this->RMB_state;
	}
	inline bool RMBlonger(Uint32 time_req)
	{
		return (SDL_GetTicks() - this->RMB_p_time) > time_req;
	}
	inline Uint64 RMBtime()
	{
		return this->RMB_p_time;
	}
	inline bool MMBhold()
	{
		return this->MMB_state;
	}
	inline bool MMBlonger(Uint32 time_req)
	{
		return (SDL_GetTicks() - this->MMB_p_time) > time_req;
	}
	inline Uint64 MMBtime()
	{
		return this->MMB_p_time;
	}
	inline bool LMB()
	{
		return this->LMB_p_state;
	}
	inline bool RMB()
	{
		return this->RMB_p_state;
	}
	inline bool MMB()
	{
		return this->MMB_p_state;
	}
	inline const SDL_Rect& windowSize(){
		return windowsize;
	}
	template <typename _T>
	_T wheel(_T rate)
	{
		return (_T)this->wheel_state * rate;
	}
	inline Sint32 wheel()
	{
		return this->wheel_state;
	}
	inline const SDL_Point& mousePos(){
		return mousepos;
	}
	inline const SDL_Point& LMBpressPos(){
		return LMB_p_pos;
	}
	inline const SDL_Point& RMBpressPos(){
		return RMB_p_pos;
	}
	inline bool mouseOn(const SDL_Rect& rect){
		return (mousepos.x > rect.x) && (mousepos.x < rect.x + rect.w) && (mousepos.y > rect.y) && (mousepos.y < rect.y + rect.h);
	}
	inline bool mouseHover(Uint32 time_req){
		return (SDL_GetTicks() - this->mouse_move_time) > time_req;
	}
	inline const SDL_Point& mouseOffset(){
		return this->mouse_move_offset;
	}
};

#endif
