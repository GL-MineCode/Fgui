#ifndef __INC_SDL_EVENTPLUS
#define __INC_SDL_EVENTPLUS
#include <SDL.h>
#include <cstring>
class EventPlus
{
private:
	SDL_Event *base;
	bool key_state[SDL_NUM_SCANCODES];
	bool key_p_state[SDL_NUM_SCANCODES];
	Uint32 key_p_time[SDL_NUM_SCANCODES];
	bool LMB_state;
	bool RMB_state;
	bool MMB_state;
	bool LMB_p_state;
	bool RMB_p_state;
	bool MMB_p_state;
	SDL_Point LMB_p_pos;
	SDL_Point RMB_p_pos;
	SDL_Point MMB_p_pos;
	Uint32 LMB_p_time;
	Uint32 RMB_p_time;
	Uint32 MMB_p_time;
	Sint32 wheel_state;
	SDL_Point mousepos;
	Uint32 mouse_move_time;
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
		if (this->base->type == SDL_KEYDOWN)
		{
			if(win && this->base->key.windowID != SDL_GetWindowID(win)) return;

			SDL_Scancode sc = this->base->key.keysym.scancode;
			if (!key_state[sc])
			{
				key_p_state[sc] = true;
				key_state[sc] = true;
				key_p_time[sc] = this->base->key.timestamp;
			}
		}
		else if (this->base->type == SDL_KEYUP)
		{
			if(win && this->base->key.windowID != SDL_GetWindowID(win)) return;

			SDL_Scancode sc = this->base->key.keysym.scancode;
			key_state[sc] = false;
		}
		else if (this->base->type == SDL_MOUSEBUTTONDOWN)
		{
			if(win && this->base->button.windowID != SDL_GetWindowID(win)) return;

			if (this->base->button.button == SDL_BUTTON_LEFT)
			{
				LMB_p_state = true;
				LMB_state = true;
				LMB_p_pos = {this->base->button.x,this->base->button.y};
				LMB_p_time = this->base->button.timestamp;
			}
			else if (this->base->button.button == SDL_BUTTON_RIGHT)
			{
				RMB_p_state = true;
				RMB_state = true;
				RMB_p_pos = {this->base->button.x,this->base->button.y};
				RMB_p_time = this->base->button.timestamp;
			}else if(this->base->button.button == SDL_BUTTON_MIDDLE){
				MMB_p_state = true;
				MMB_state = true;
				MMB_p_pos = {this->base->button.x,this->base->button.y};
				MMB_p_time = this->base->button.timestamp;
			}
		}
		else if (this->base->type == SDL_MOUSEBUTTONUP)
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
		else if (this->base->type == SDL_MOUSEWHEEL)
		{
			if(win && this->base->wheel.windowID != SDL_GetWindowID(win)) return;

			if (this->base->wheel.direction == SDL_MOUSEWHEEL_NORMAL)
				this->wheel_state = this->base->wheel.y;
			else
				this->wheel_state = -this->base->wheel.y;
		}
		else if (this->base->type == SDL_MOUSEMOTION)
		{
			if(win && this->base->motion.windowID != SDL_GetWindowID(win)) return;

			this->mousepos = {this->base->motion.x, this->base->motion.y};
			mouse_move_time = this->base->motion.timestamp;

			mouse_move_offset.x += this->base->motion.xrel;
			mouse_move_offset.y += this->base->motion.yrel;
		}
		else if(this->base->type == SDL_WINDOWEVENT){
			if(win && this->base->window.windowID != SDL_GetWindowID(win)) return;

			if(this->base->window.event == SDL_WINDOWEVENT_RESIZED){
				this->windowsize.w = this->base->window.data1;
				this->windowsize.h = this->base->window.data2;
			}else if(this->base->window.event == SDL_WINDOWEVENT_MOVED){
				this->windowsize.x = this->base->window.data1;
				this->windowsize.y = this->base->window.data2;
			}
		}

		if(!win) return;
		//解决SDL的一个bug，鼠标捕获不触发SDL_MOUSEBUTTONUP事件
		int temp;
		Uint32 buttons = SDL_GetGlobalMouseState(&temp,&temp);
		if((buttons & SDL_BUTTON_LMASK) == 0 && LMB_state){
			SDL_Event fakeUp{0};
			fakeUp.type = SDL_MOUSEBUTTONUP;
			fakeUp.button.button = SDL_BUTTON_LEFT;
			fakeUp.button.x = -1;
			fakeUp.button.y = -1;
			fakeUp.button.clicks = 1;
			fakeUp.button.windowID = SDL_GetWindowID(win);
			fakeUp.button.state = SDL_RELEASED;
			fakeUp.button.timestamp = SDL_GetTicks();
			fakeUp.button.which = SDL_TOUCH_MOUSEID;
			SDL_PushEvent(&fakeUp);
		}
		if((buttons & SDL_BUTTON_RMASK) == 0 && RMB_state){
			SDL_Event fakeUp{0};
			fakeUp.type = SDL_MOUSEBUTTONUP;
			fakeUp.button.button = SDL_BUTTON_RIGHT;
			fakeUp.button.x = -1;
			fakeUp.button.y = -1;
			fakeUp.button.clicks = 1;
			fakeUp.button.windowID = SDL_GetWindowID(win);
			fakeUp.button.state = SDL_RELEASED;
			fakeUp.button.timestamp = SDL_GetTicks();
			fakeUp.button.which = SDL_TOUCH_MOUSEID;
			SDL_PushEvent(&fakeUp);
		}
		if((buttons & SDL_BUTTON_MMASK) == 0 && MMB_state){
			SDL_Event fakeUp{0};
			fakeUp.type = SDL_MOUSEBUTTONUP;
			fakeUp.button.button = SDL_BUTTON_MIDDLE;
			fakeUp.button.x = -1;
			fakeUp.button.y = -1;
			fakeUp.button.clicks = 1;
			fakeUp.button.windowID = SDL_GetWindowID(win);
			fakeUp.button.state = SDL_RELEASED;
			fakeUp.button.timestamp = SDL_GetTicks();
			fakeUp.button.which = SDL_TOUCH_MOUSEID;
			SDL_PushEvent(&fakeUp);
		}
	}

	inline int waitpoll(void)
	{
		int ret = SDL_WaitEvent(this->base);
		this->viewEvent();
		return ret;
	}

	inline int poll(void)
	{
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
	inline Uint32 holdTime(SDL_Scancode scancode)
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
	inline Uint32 LMBtime()
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
	inline Uint32 RMBtime()
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
	inline Uint32 MMBtime()
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
