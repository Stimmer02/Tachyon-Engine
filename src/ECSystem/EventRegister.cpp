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
    std::chrono::nanoseconds currentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
    char helper[4096*10] = {}; //tyle powinno wystarczyć
    EventQueueElement elementToAdd;

    va_list args;
    va_start(args, _format);

    vsnprintf(helper, sizeof(helper), _format, args);
    
    va_end(args);

    elementToAdd.time = currentTime;
    elementToAdd.data = std::string(helper);
    
    if(elementToAdd.data.size() > 0){
        if(elementToAdd.data[elementToAdd.data.size() - 1] != '\n'){    
            elementToAdd.data += '\n';
        }
    }
    elementToAdd.type = _type;


    std::unique_lock<std::mutex> loc(mut);

    eventQueue.push(elementToAdd);

}

void EventRegister::Flush(){
    EventQueueElement eqe;
    std::time_t helper;
    char time[30];
    std::unique_lock<std::mutex> loc(mut);
    while(!eventQueue.empty()){
        eqe = eventQueue.top();
        helper = std::chrono::system_clock::to_time_t(std::chrono::system_clock::time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(eqe.time)));
        eventQueue.pop();
        strftime(time, sizeof(time), "%d %b %Y %H:%M:%S", localtime(&helper));
        // fprintf(logFile, "%s", "ABC");
        fprintf(logFile, "%s [%s] : %s", time, types[eqe.type], eqe.data.c_str());
    }
    fflush(logFile);
}

