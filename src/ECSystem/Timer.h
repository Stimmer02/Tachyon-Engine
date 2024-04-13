#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <chrono>

typedef std::chrono::high_resolution_clock::time_point Timepoint;

class Timer {

private:

    Timepoint lastTime;

    double accumulatedTime;
    double deltaFrame;
    double deltaTime;

    uint32_t lastFrameCount;

    Timer(){
        this->lastTime = GetCurrentTime();
        this->deltaTime = 1.0f/60.0f;
    }


public:

    void TicTac(){
        Timepoint currentTime = GetCurrentTime();
        deltaFrame = GetDurationInSeconds(currentTime - lastTime);
        lastTime = currentTime;

        accumulatedTime *= (accumulatedTime < 1.0);
        accumulatedTime += deltaFrame;

        lastFrameCount = 1.0f / deltaFrame;
        deltaTime = deltaFrame * 1000.0f;
    }

    double & GetDeltaTime(){
        return deltaTime;
    }

    double & GetDeltaFrame(){
        return deltaFrame;
    }

    uint32_t & GetFrameCount(){
        return lastFrameCount;
    }

    double & GetAccumulatedTime(){
        return accumulatedTime;
    }

    static Timer& GetInstance(){
        static Timer instance;

        return instance;
    }

    static Timepoint GetCurrentTime(){
        return std::chrono::high_resolution_clock::now();
    }

    static double GetDurationInSeconds(const std::chrono::high_resolution_clock::duration& duration){
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    }

    static double GetDurationInMiliseconds(const std::chrono::high_resolution_clock::duration& duration){
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }


};

#endif
