#include "EventRegister.h"


EventRegister::EventRegister(){

    logFile = fopen("/logs/log.txt", "w"); // open or create log.txt and clear it;

    if(logFile == NULL){
        std::cout << "mamy problem bo tego błędu nie da się wypisać do pliku z logami \n";
    }
}
EventRegister::~EventRegister(){
    if(logFile != NULL){
        fclose(logFile);
    }
}

void EventRegister::Write(enum MessageType _type, const char * _format, ...){
    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char* helper = new char[4096]; //tyle powinno wystarczyć
    EventQueueElement elementToAdd;

    va_list args;
    va_start(args, _format);

    sprintf(helper, _format, args);

    va_end(args);

    elementToAdd.time = currentTime;
    elementToAdd.data = helper;
    elementToAdd.type = _type;

    delete [] helper;

    std::unique_lock<std::mutex> loc(mut);

    eventQueue.push(elementToAdd);

}

void EventRegister::Flush(){
    EventQueueElement eqe;
    char time[20];
    std::unique_lock<std::mutex> loc(mut);
    while(!eventQueue.empty()){
        eqe = eventQueue.top();
        eventQueue.pop();
        strftime(time, sizeof(time), "%d %b %Y %H:M:%S", localtime(&eqe.time));
        fprintf(logFile, "%s", "ABC");
//         fprintf(logFile, "%s, %s, %s", time, MessageTypeToString(eqe.type), eqe.data.c_str());
    }
    fflush(logFile);
}

const char* EventRegister::MessageTypeToString(MessageType type){
    switch (type) {
        case INFO: return "INFO";
        case ERROR: return "ERROR";
        case DEBUG: return "DEBUG";
        default: return "OTHER";
    }
}
