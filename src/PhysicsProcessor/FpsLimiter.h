#ifndef FPSLIMITER_H
#define FPSLIMITER_H

#include "../ECSystem/System.h"
#include "../ECSystem/Timer.h"
#include "../ECSystem/Configurator.h"


#include <thread>


class FpsLimiter : public System{
public:

    FpsLimiter();   
    ~FpsLimiter();

    void Share(SharedNameResolver* snr) override;
    void OnLoad() override;
    void OnUnload() override;

private:
    void Execute() override;

    SharedNameResolver* resourceManager;
    Timepoint lastTimepoint;
    const char* PPConfigPath;

    float fps;
    Timer* timer;
};

#endif
