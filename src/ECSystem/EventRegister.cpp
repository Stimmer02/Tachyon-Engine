#include "EventRegister.h"

EventRegister::EventRegister(FILE * file, bool isSystemStream){

    this->logFile = file;
    this->isSystemStream = isSystemStream;

    if(logFile == NULL){
        std::cout << "mamy problem bo tego błędu nie da się wypisać do pliku z logami \n";
    }
}
EventRegister::EventRegister(const char* filepath){

    this->logFile = fopen(filepath, "w"); // open or create log.txt and clear it;
    this->isSystemStream = false;

    if(logFile == NULL){
        std::cout << "mamy problem bo tego błędu nie da się wypisać do pliku z logami \n";
    }
}
EventRegister::~EventRegister(){
    if(logFile != NULL && !isSystemStream){
        fclose(logFile);
    }
}

void EventRegister::Write(enum LogMessageType _type, const char * _format, ...){
    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char helper[4096] = {}; //tyle powinno wystarczyć
    EventQueueElement elementToAdd;

    va_list args;
    va_start(args, _format);

    vsnprintf(helper, sizeof(helper), _format, args);

    va_end(args);

    elementToAdd.time = currentTime;
    elementToAdd.data = std::string(helper);
    elementToAdd.type = _type;


    std::unique_lock<std::mutex> loc(mut);

    eventQueue.push(elementToAdd);

}

void EventRegister::Flush(){
    EventQueueElement eqe;
    char time[30];
    std::unique_lock<std::mutex> loc(mut);
    while(!eventQueue.empty()){
        eqe = eventQueue.top();
        eventQueue.pop();
        strftime(time, sizeof(time), "%d %b %Y %H:%M:%S", localtime(&eqe.time));
        // fprintf(logFile, "%s", "ABC");
        fprintf(logFile, "%s [%s] : %s", time, types[eqe.type], eqe.data.c_str());
    }
    fflush(logFile);
}

