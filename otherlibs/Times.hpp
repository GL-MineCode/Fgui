#ifndef __INC_TIMES_
#define __INC_TIMES_

#include <SDL.h>

class Timer {  
private:  
    Uint64 targetTicksPerFrame; 
    Uint64 lastFrameTicks;
public:  
    Timer() {  
        targetTicksPerFrame = 1000 / 30;  
        lastFrameTicks = SDL_GetTicks64();
    }  
    void Set(Uint64 maxFPS){
        targetTicksPerFrame = 1000 / maxFPS;  
        lastFrameTicks = SDL_GetTicks64(); 
    }
    Timer(Uint64 maxFPS) {  
        this->Set(maxFPS); 
    }  
    void Delay() {  
        Uint64 currentTicks = SDL_GetTicks64();  
        Uint64 elapsedTicks = currentTicks - lastFrameTicks;   
        if (elapsedTicks < targetTicksPerFrame) {  
            SDL_Delay(static_cast<Uint32>(targetTicksPerFrame - elapsedTicks));  
        }  
        lastFrameTicks = SDL_GetTicks64();  
    }  
};
#endif