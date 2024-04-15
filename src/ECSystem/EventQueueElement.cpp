#include "EventQueueElement.h"

bool EventQueueElement::operator<(const EventQueueElement &other) const{
    if(time == other.time){
            return data < other.data;
    }
    return difftime(time, other.time) < 0;
}


bool EventQueueElement::operator>(const EventQueueElement &other) const{
    if(time == other.time){
            return data > other.data;
    }
    return difftime(time, other.time) > 0;
}
