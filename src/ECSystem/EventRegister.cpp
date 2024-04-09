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
    std::unique_lock<std::mutex> loc(mut);

}

void EventRegister::Flush(){
    std::unique_lock<std::mutex> loc(mut);
    flush(logFile);
}
