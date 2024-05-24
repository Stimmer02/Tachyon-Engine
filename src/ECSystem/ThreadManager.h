#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#ifdef DEBUG

#include <stdio.h>

#endif

#include "IRunnable.h"

#include <vector>
#include <thread>
#include <stdint.h>

class ThreadManager{
private:

    std::vector< std::pair<std::thread, IRunnable * > > runnables;

public:

    void Run( IRunnable * _runnable );

    void Kill( IRunnable * _runnable );

    void StopAll();

};

#endif
