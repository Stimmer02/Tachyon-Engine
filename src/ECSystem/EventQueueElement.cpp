#include "EventQueueElement.h"

bool EventQueueElement::operator<(const EventQueueElement &other) const{
    if(time == other.time){
            return data < other.data;
    }
    return time.count() - other.time.count() < 0;
}


bool EventQueueElement::operator>(const EventQueueElement &other) const{
    if(time == other.time){
            return data > other.data;
    }
    return time.count() - other.time.count() > 0;
}
