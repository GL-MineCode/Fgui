/*

								SMOOTH CPP
			This CPP docment is writen for make number more smooth.
			It run nicely!
			It's free to use commercially
			Writen by: GL

*/
#ifndef __INC_SMOOTH_CPP
#define __INC_SMOOTH_CPP

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <functional>
#include <cstdint>

#ifndef SDL_stdinc_h_

typedef int8_t Sint8;

typedef uint8_t Uint8;

typedef int16_t Sint16;

typedef uint16_t Uint16;

typedef int32_t Sint32;

typedef uint32_t Uint32;


typedef int64_t Sint64;

typedef uint64_t Uint64;

#endif

#ifndef SDL_timer_h_

#include <Windows.h>

#define SDL_GetTicks64 GetTickCount64

#define SDL_GetTicks GetTickCount

#endif

#ifndef SDL_rect_h_

typedef struct SDL_Rect
{
    int x, y;
    int w, h;
} SDL_Rect;

#endif

#ifndef HAVE_M_PI
#ifndef M_PI
#define M_PI    3.14159265358979323846264338327950288   /**< pi */
#endif
#endif

#define HALF_PI (M_PI / 2.0f)

typedef unsigned long long Uint64;

/**
 * @brief "之前"，函数特性：对于从time_loc的时间到time_now之差，取0到dur_time的部分作为线性上升沿，其余大于dur_time的部分为1。函数周期：不是周期函数
 * 
 * @param time_loc 开始计时的时间戳
 * @param time_now 当前时间戳
 * @param dur_time 上升沿持续时间
 * @return double 表示上升沿和下降沿状态的归一化数值
 */
double beforeNumber(Uint64 time_loc, Uint64 time_now, Uint64 dur_time) {
	Uint64 dur_t = time_now - time_loc;
	if (dur_t > dur_time) {
		return 1.0f;
	}
	return (double)dur_t / (double)dur_time;
}

/**
 * @brief "之间"，函数特性：对于从time_loc的时间到time_now之差，取dur_time * K到dur_time * (K + 1)的部分作为线性上升沿（K∈Z），其余大于dur_time的部分为1。函数周期：dur_time
 * 
 * @param time_loc 开始计时的时间戳
 * @param time_now 当前时间戳
 * @param dur_time 上升沿持续时间
 * @return double 表示上升沿和下降沿状态的归一化数值
 */
double inNumber(Uint64 time_loc, Uint64 time_now, Uint64 dur_time) {
	Uint64 dur_t = (time_now - time_loc) % dur_time;
	return (double)dur_t / (double)dur_time;
}

/**
 * @brief "活塞"，函数特性：先上升rising_time的时间，然后保持rising_hold的时间为1，然后下降falling_time的时间，然后保持falling_hold时间为0，之后一直重复该过程。函数周期：(rising_time + rising_hold + falling_time + falling_hold)
 * 
 * @tparam _T 时间参数的类型
 * @param time_now 当前时间
 * @param rising_time 上升沿持续时间
 * @param rising_hold 上升沿保持时间
 * @param falling_time 下降沿持续时间
 * @param falling_hold 下降沿保持时间
 * 
 * @note 就像这样：╱￣╲__╱￣╲__╱￣╲__╱￣╲__
 * 
 * @return double 表示上升沿和下降沿状态的归一化数值
 */
template <typename _T>
double Piston(_T time_now,_T rising_time,_T rising_hold,_T falling_time,_T falling_hold)
{
	static_assert((std::is_integral_v<_T> || std::is_floating_point_v<_T>),"The template parameter must be of numeric type.");

	_T temp;
	if constexpr (std::is_integral_v<_T>) temp = time_now % (rising_time + rising_hold + falling_time + falling_hold);
	else if constexpr (std::is_floating_point_v<_T>) temp = fmod(time_now,rising_time + rising_hold + falling_time + falling_hold);

	_T cnt = rising_time;
	if (temp < cnt)
	{
		return (double)(temp / rising_time);
	}
	cnt += rising_hold;
	if (temp < cnt)
	{
		return 1.0;
	}
	cnt += falling_time;
	if (temp < cnt)
	{
		return (double)((cnt - temp) / falling_time);
	}
	return 0.0;
}

class NumberEnabler{
	Uint64 u1;
	Uint64 u2;
	bool nstate;
	double ans;
	public:
	NumberEnabler() : nstate(false),u1(0),u2(0),ans(0){

	}
	double State(bool state,Uint64 tm,Uint64 timeHold){
		if(state){
            if(nstate){
                u1 = tm - (1.0f - beforeNumber(u2,tm,timeHold)) * timeHold;
            }else{
                u2 = tm;
            }
            nstate = true;
        }else{
            nstate = false;
        }
		return ans = beforeNumber(u1,tm,timeHold);
	}
	double Get() const{
		return ans;
	}
};

class NumberEnablerRev{
	Uint64 u1;
	Uint64 u2;
	bool nstate;
	double ans;
	public:
	NumberEnablerRev() : nstate(false),u1(0),u2(0){

	}
	double State(bool state,Uint64 tm,Uint64 timeHold){
		if(state){
            if(nstate){
                u1 = tm - (1.0f - beforeNumber(u2,tm,timeHold)) * timeHold;
            }else{
                u2 = tm;
            }
            nstate = true;
        }else{
            nstate = false;
        }
		return ans = 1.0f - beforeNumber(u1,tm,timeHold);
	}
	double Get() const{
		return ans;
	}
};

class Chronograph{
	Uint64 time;
	public:
	Chronograph(){
		time = 0;
	}
	void Reset(){
		time = SDL_GetTicks64();
	}
	Uint64 GetPast(){
		return SDL_GetTicks64() - time;
	}
	double GetTimeProgress(Uint64 duration){
		return beforeNumber(time,SDL_GetTicks64(),duration);
	}

};

class OneShot{
    bool called = false;
    public:
    void Reset(){
        called = false;
    }
    bool Shot(){
        if(!called){
            called = true;
            return true;
        }else{
            return false;
        }
    }
};

struct FrozenRect : public SDL_Rect {
	bool inited;
	FrozenRect() : inited(false){
	}
    FrozenRect& operator=(const SDL_Rect& _rect) {
		if(!inited){
			this->x = _rect.x;
			this->y = _rect.y;
			this->w = _rect.w;
			this->h = _rect.h;
			inited = true;
			return *this;
		}
        int x1 = std::min(this->x, _rect.x);
        int y1 = std::min(this->y, _rect.y);
        int x2 = std::max(this->x + this->w, _rect.x + _rect.w);
        int y2 = std::max(this->y + this->h, _rect.y + _rect.h);
        this->x = x1;
        this->y = y1;
        this->w = x2 - x1;
        this->h = y2 - y1;
        return *this;
    }
};

inline int RectXCenter(const SDL_Rect& rect){
	return rect.x + rect.w / 2;
}

inline int RectYCenter(const SDL_Rect& rect){
	return rect.y + rect.h / 2;
}

template<typename _T>
_T linear_interpolation(_T* array, float pos) {
    int index = static_cast<int>(pos);
    float frac = pos - index;

    _T value1 = array[index];
    _T value2 = array[index + 1];

    float result = static_cast<float>(value1) + (static_cast<float>(value2) - static_cast<float>(value1)) * frac;

    if constexpr (std::is_integral_v<_T>) {
        return static_cast<_T>(result + 0.5f);
    } else {
        return static_cast<_T>(result);
    }
}

template<typename _T>
_T quadratic_interpolation(_T* array, float pos) {
    int index = static_cast<int>(pos);
    float frac = pos - index;

    _T value0 = array[index - 1];
    _T value1 = array[index];
    _T value2 = array[index + 1];

    float result = static_cast<float>(value1) + 0.5f * frac * 
                   (static_cast<float>(value2) - static_cast<float>(value0) + 
                    frac * (static_cast<float>(value2) - 2 * static_cast<float>(value1) + static_cast<float>(value0)));

    if constexpr (std::is_integral_v<_T>) {
        return static_cast<_T>(result + 0.5f);
    } else {
        return static_cast<_T>(result);
    }
}

template<typename _T>
_T cubic_interpolation(_T* array, float pos) {
    int index = static_cast<int>(pos);
    float frac = pos - index;

    _T value0 = array[index - 1];
    _T value1 = array[index];
    _T value2 = array[index + 1];
    _T value3 = array[index + 2];

    float a0 = static_cast<float>(value3) - static_cast<float>(value2) - static_cast<float>(value0) + static_cast<float>(value1);
    float a1 = static_cast<float>(value0) - static_cast<float>(value1) - a0;
    float a2 = static_cast<float>(value2) - static_cast<float>(value0);
    float a3 = static_cast<float>(value1);

    float result = a0 * frac * frac * frac + a1 * frac * frac + a2 * frac + a3;

    if constexpr (std::is_integral_v<_T>) {
        return static_cast<_T>(result + 0.5f);
    } else {
        return static_cast<_T>(result);
    }
}

template<typename _T>
_T linear_interpolation(_T* array, double pos) {
    int index = static_cast<int>(pos);
    double frac = pos - index;

    _T value1 = array[index];
    _T value2 = array[index + 1];

    double result = static_cast<double>(value1) + (static_cast<double>(value2) - static_cast<double>(value1)) * frac;

    if constexpr (std::is_integral_v<_T>) {
        return static_cast<_T>(result + 0.5f);
    } else {
        return static_cast<_T>(result);
    }
}

template<typename _T>
_T quadratic_interpolation(_T* array, double pos) {
    int index = static_cast<int>(pos);
    double frac = pos - index;

    _T value0 = array[index - 1];
    _T value1 = array[index];
    _T value2 = array[index + 1];

    double result = static_cast<double>(value1) + 0.5f * frac * 
                   (static_cast<double>(value2) - static_cast<double>(value0) + 
                    frac * (static_cast<double>(value2) - 2 * static_cast<double>(value1) + static_cast<double>(value0)));

    if constexpr (std::is_integral_v<_T>) {
        return static_cast<_T>(result + 0.5f);
    } else {
        return static_cast<_T>(result);
    }
}

template<typename _T>
_T cubic_interpolation(_T* array, double pos) {
    int index = static_cast<int>(pos);  // 获取整数部分
    double frac = pos - index;           // 获取小数部分

    _T value0 = array[index - 1];
    _T value1 = array[index];
    _T value2 = array[index + 1];
    _T value3 = array[index + 2];

    double a0 = static_cast<double>(value3) - static_cast<double>(value2) - static_cast<double>(value0) + static_cast<double>(value1);
    double a1 = static_cast<double>(value0) - static_cast<double>(value1) - a0;
    double a2 = static_cast<double>(value2) - static_cast<double>(value0);
    double a3 = static_cast<double>(value1);

    double result = a0 * frac * frac * frac + a1 * frac * frac + a2 * frac + a3;

    if constexpr (std::is_integral_v<_T>) {
        return static_cast<_T>(result + 0.5f);
    } else {
        return static_cast<_T>(result);
    }
}

class EMASmoother
{
public:
    EMASmoother(float _alpha = 0.2) : alpha(_alpha), lastValue(0) {}

	void resetLastValue(){
		lastValue = 0;
	}

	void setLastValue(float v){
		lastValue = v;
	}

	float getLastValue(){
		return lastValue;
	}

	void setAlpha(float _alpha = 0.2){
		alpha = _alpha;
	}

    float process(float input)
    {
        lastValue = alpha * input + (1 - alpha) * lastValue;
        return lastValue;
    }

private:
    float alpha;
    float lastValue;
};

/**
 * @brief 计算某个值线性逼近一个特定的目标值
 * 
 * @tparam _T 泛型模板参数
 * @param current 当前的值
 * @param target 目标的值
 * @param speed 每次调用时当前值朝目标值的最大位移
 * @return _T 位移后的值
 */
template<typename _T> _T LinearApproach(_T current,_T target,_T speed){
	if(current < target){
		return std::min(current + speed,target);
	}
	else if(current > target){
		return std::max(current - speed,target);
	}
	return target;
}

constexpr double BEZIER_CURVE_EPSILON = 1e-6;

/**
 * @brief 贝塞尔平滑函数
 * 
 * @param x 要算的X坐标(0~1)
 * @param a 左控制点X坐标
 * @param b 左控制点Y坐标
 * @param c 右控制点X坐标
 * @param d 右控制点Y坐标
 * @return double 算出的Y坐标
 */
double BezierCurve(double x, double a, double b, double c, double d) {
	// if (x <= 0.0f) return 0;
	// if (x >= 1.0f) return 1.0f;
	double px3 = 3 * a;
	double px2 = 3 * (c - a) - px3;
	double px1 = 1 - px3 - px2;
	double py3 = 3 * b;
	double py2 = 3 * (d - b) - py3;
	double py1 = 1 - py3 - py2;

#define __GetX(t) (((px1 * t + px2) * t + px3) * t)
#define __GetY(t) (((py1 * t + py2) * t + py3) * t)

	double t = x;

	//Newton's method
	for (int i = 0; i < 8; i++) {
		double g = __GetX(t) - x;
		if (fabs(g) < BEZIER_CURVE_EPSILON) {
			return __GetY(t);
		}
		double d = (3 * px1 * t + 2 * px2) * t + px3;
		if (fabs(d) < 1e-6) {
			break;
		}
		t = t - g / d;
	}

	//牛顿迭代失效时改用二分法
	double low = 0.0;
    double high = 1.0;
    for (int i = 0; i < 10; i++) {
        t = (low + high) * 0.5;
        double xMid = __GetX(t);
        if (xMid < x) {
            low = t;
        } else {
            high = t;
        }
    }
    return __GetY(t);
}

/**
 * @brief "带贝塞尔平滑函数的活塞"，函数特性：先上升rising_time的时间，然后保持rising_hold的时间为1，然后下降falling_time的时间，然后保持falling_hold时间为0，之后一直重复该过程，其中我们对上升沿和下降沿分别应用不同的贝塞尔平滑函数。函数周期：(rising_time + rising_hold + falling_time + falling_hold)
 * 
 * @tparam _T 时间参数的类型
 * @param time_now 当前时间
 * @param rising_time 上升沿持续时间
 * @param rising_hold 上升沿保持时间
 * @param falling_time 下降沿持续时间
 * @param falling_hold 下降沿保持时间
 * @param Ax1 上升沿贝塞尔平滑函数左控制点X坐标
 * @param Ay1 上升沿贝塞尔平滑函数左控制点Y坐标
 * @param Ax2 上升沿贝塞尔平滑函数右控制点X坐标
 * @param Ay2 上升沿贝塞尔平滑函数右控制点Y坐标
 * @param Bx1 下降沿贝塞尔平滑函数左控制点X坐标
 * @param By1 下降沿贝塞尔平滑函数左控制点Y坐标
 * @param Bx2 下降沿贝塞尔平滑函数右控制点X坐标
 * @param By2 下降沿贝塞尔平滑函数右控制点Y坐标
 * 
 * @note 就像这样：╱￣╲__╱￣╲__╱￣╲__╱￣╲__
 * 
 * @return double 表示上升沿和下降沿状态的归一化数值
 */
template <typename _T>
double PistonWithBezierCurve(_T time_now,_T rising_time,_T rising_hold,_T falling_time,_T falling_hold,double Ax1,double Ay1,double Ax2,double Ay2,double Bx1,double By1,double Bx2,double By2)
{
	static_assert(!(std::is_integral_v<_T> || std::is_floating_point_v<_T>),"The template parameter must be of numeric type.");

	_T temp;
	if constexpr (std::is_integral_v<_T>) temp = time_now % (rising_time + rising_hold + falling_time + falling_hold);
	else if constexpr (std::is_floating_point_v<_T>) temp = fmod(time_now,rising_time + rising_hold + falling_time + falling_hold);

	_T cnt = rising_time;
	if (temp < cnt)
	{
		return BezierCurve((double)(temp / rising_time),Ax1,Ay1,Ax2,Ay2);
	}
	cnt += rising_hold;
	if (temp < cnt)
	{
		return 1.0;
	}
	cnt += falling_time;
	if (temp < cnt)
	{
		return BezierCurve((double)((cnt - temp) / falling_time),Bx1,By1,Bx2,By2);
	}
	return 0.0;
}

namespace TimingFunc {

	template<typename _T> _T COS(double timing, _T rate) {
		return (1.0f - cos(timing * M_PI) / 2.0f) * rate;
	}

	template<typename _T> _T COS_F(double timing, _T rate) {
		return (cos(timing * M_PI) / 2.0f) * rate;
	}

	template<typename _T> _T _bezier_curve(double timing, _T rate, double a, double b, double c, double d) {
		if (timing <= 0.0f) return 0;
		if (timing >= 1.0f) return rate;
		double px3 = 3 * a;
		double px2 = 3 * (c - a) - px3;
		double px1 = 1 - px3 - px2;
		double py3 = 3 * b;
		double py2 = 3 * (d - b) - py3;
		double py1 = 1 - py3 - py2;

#define __GetX(t) (((px1 * t + px2) * t + px3) * t)
#define __GetY(t) (((py1 * t + py2) * t + py3) * t)

		double t = timing;

		//Newton's method
		for (int i = 0; i < 8; i++) {
			double g = __GetX(t) - timing;
			if (fabs(g) < BEZIER_CURVE_EPSILON) {
				return __GetY(t) * rate;
			}
			double d = (3 * px1 * t + 2 * px2) * t + px3;
			if (fabs(d) < 1e-6) {
				break;
			}
			t = t - g / d;
		}
		//Failed
		return __GetY(t) * rate;
	}

	template<typename _T> _T bezier_curve(double timing, _T rate, double a, double b, double c, double d) {
		return 1.0f - _bezier_curve<_T>(timing, rate, a, b, c, d);
	}



#define EASE(timing,rate) bezier_curve(timing,rate,0.25f,0.1f,0.25f,1.0f)

#define LINEAR(timing,rate) bezier_curve(timing,rate,0.0f,0.0f,0.1f,0.1f)

#define EASE_IN(timing,rate) bezier_curve(timing,rate,0.42f,0.0f,1.0f,1.0f)

#define EASE_OUT(timing,rate) bezier_curve(timing,rate,0.0f,0.0f,0.58f,1.0f)

#define EASE_IN_OUT(timing,rate) bezier_curve(timing,rate,0.42f,0.0f,0.58f,1.0f)

}

#define BC_EASE 0.25f,0.1f,0.25f,1.0f
#define BC_LINEAR 0.25f,0.1f,0.25f,1.0f
#define BC_EASE_IN 0.25f,0.1f,0.25f,1.0f
#define BC_EASE_OUT 0.25f,0.1f,0.25f,1.0f
#define BC_EASE_IN_OUT 0.25f,0.1f,0.25f,1.0f

template<typename _T> _T QuickNumberEnabler(
	const NumberEnabler& ne,
	_T amount,
	_T offset,
	double a,
	double b,
	double c,
	double d
){
	return offset + double(amount * BezierCurve(ne.Get(),a,b,c,d));
}

template<typename _T> _T QuickNumberEnabler(
	const NumberEnablerRev& ne,
	_T amount,
	_T offset,
	double a,
	double b,
	double c,
	double d
){
	return offset + double(amount * BezierCurve(ne.Get(),a,b,c,d));
}

#undef __GetX
#undef __GetY

#endif
