#include "EventRegister.h"

EventRegister::EventRegister(){
    std::string name = "/logs/log.txt";

    logFile.open(name, std::fstream::trunc); // open or create log.txt and clear it;

    if(!logFile.is_open()){
        std::cout << "mamy problem bo tego błędu nie da się wypisać do pliku z logami \n";
    }
}
EventRegister::~EventRegister(){
    if(logFile.is_open()){
        logFile.close();
    }
}

void EventRegister::Write(enum MessageType _type, const char * _format, ...){
    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char* helper = new char[4096]; //tyle powinno wystarczyć

    va_list args;
    va_start(args, _format);

    sprintf(helper, _format, args);

    va_end(args);

    std::unique_lock<std::mutex> loc(mut);

    eventQueue.push({{currentTime, _type}, std::string(helper)});

    delete [] helper;
}

void EventRegister::Flush(){
    std::unique_lock<std::mutex> loc(mut);
    flush(logFile);
}
