#include "ThreadManager.h"

void ThreadManager::Run( IRunnable * _runnable ){

    if( _runnable == nullptr )
        return;

    runnables.emplace_back( std::thread(&IRunnable::Run, _runnable), _runnable );

#ifdef DEBUG
    fprintf(stdout, "[DEBUG] Running new thread\n");
#endif

}

void ThreadManager::Kill( IRunnable * _runnable ){

    if( _runnable == nullptr )
        return;

    for(uint32_t i=0; i < runnables.size(); i++){

        if( runnables[i].second == _runnable ){
            _runnable->Stop();
            runnables[i].first.join();
            break;
        }

    }

#ifdef DEBUG
    fprintf(stdout, "[DEBUG] Killed thread\n");
#endif

}

void ThreadManager::StopAll(){

    for(uint32_t i=0; i < runnables.size(); i++){

        runnables[i].second->Stop();
        runnables[i].first.join();

    }

    runnables.clear();

#ifdef DEBUG
    fprintf(stdout, "[DEBUG] Joining all threads\n");
#endif

}
