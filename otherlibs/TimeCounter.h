#ifndef __INC_TIMECOUNTER
#define __INC_TIMECOUNTER

#include <Windows.h>

LARGE_INTEGER freq;
LARGE_INTEGER temp;
typedef LARGE_INTEGER htime_t;

bool TC_Init(){
	if (!QueryPerformanceFrequency(&freq)) {   
	    return false;  
	}  
	return true;
}

inline void TC_Begin(htime_t* time){
	QueryPerformanceCounter(time);
}

inline long long TC_End(const htime_t* time){
	QueryPerformanceCounter(&temp);
	return temp.QuadPart - time->QuadPart;
}

double TC_SecondDur(long long dur){
	return (double)dur / (double)freq.QuadPart;
}

double TC_MillisecondDur(long long dur){
	return (double)dur / (double)(freq.QuadPart / 1000);
}

#endif
