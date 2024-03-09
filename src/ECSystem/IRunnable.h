#ifndef IRUNNABLE_H
#define IRUNNABLE_H

class IRunnable {
public:
    virtual void Run() = 0;
    virtual void Stop() = 0;
};

#endif