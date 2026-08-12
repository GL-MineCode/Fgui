#ifndef __INC_STOPWATCH_
#define __INC_STOPWATCH_

#include "TimeCounter.h"

class StopWatch{
    htime_t timedur;
    long long last_perf;
    public:
    StopWatch(){
        TC_Init();
        timedur.QuadPart = 0;
        last_perf = LLONG_MAX;
    }
    ~StopWatch(){
    }
    void Start(){
        TC_Begin(&timedur);
    }
    long long Stop(){
        return last_perf = TC_End(&timedur);
    }
    double SecondPerf(){
        return TC_SecondDur(last_perf);
    }
    double MillisecondPerf(){
        return TC_MillisecondDur(last_perf);
    }
};


#endif //__INC_STOPWATCH_
