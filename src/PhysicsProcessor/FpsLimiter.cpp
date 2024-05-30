#include "FpsLimiter.h"

FpsLimiter::FpsLimiter() : fps(10){
    timer = nullptr;
}

FpsLimiter::~FpsLimiter(){
    if (timer != nullptr){
        delete timer;
    }
}

void FpsLimiter::Share(SharedNameResolver* snr){
    resourceManager = snr;

    timer = (Timer*)(snr->Find("timer"));
}

void FpsLimiter::OnLoad(){
    if (timer == nullptr){
        std::fprintf(stderr, "FpsLimiter failed to find Timer\n");
        exit(-1);
    }

    lastTimepoint = Timer::GetCurrentTime();
}

void FpsLimiter::OnUnload(){

}

void FpsLimiter::Execute(){
    Timepoint currentTime = Timer::GetCurrentTime();
    double duration = Timer::GetDurationInSeconds(currentTime - lastTimepoint);
    double frameDuration = 1.0f / fps;

    if (duration < frameDuration){
        std::this_thread::sleep_for(std::chrono::duration<double>(frameDuration - duration));
    }

    lastTimepoint = currentTime;
    
}