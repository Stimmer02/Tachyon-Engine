#include "EventManager.h"


void EventManager::Subscribe(const EventType & event, IEventListener * listener){
    if(listener==NULL){
        fprintf(stderr, "Listener can't be null.\n");
        return; 
    }

    auto& eventListeners = listeners[event];
    for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it) {
        if (*it == listener) {
            fprintf(stderr, "Listener is already attached to this event.\n");
            return;
        }
    }

    eventListeners.push_back(listener);
}

void EventManager::Unsubscribe(const EventType & event, IEventListener * listener){
    if(listener==NULL){
        fprintf(stderr, "Listener can't be null.\n");
        return; 
    }

    auto& eventListeners = listeners[event];
    for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it) {
        if (*it == listener) {
            it = eventListeners.erase(it);  // Use the iterator to erase the element
            return;
        }
    }

    fprintf(stderr, "Listener is not attached to this event system.\n");
}

void EventManager::Publish(const EventType & event, IEventListener * listener){
    if(listener==NULL){
        fprintf(stderr, "Listener can't be null.\n");
        return; 
    }

    auto& eventListeners = listeners[event];
    for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it) {
        if (*it == listener) {
            listener->OnEvent(event);
            return;
        }
    }

    fprintf(stderr, "Listener is not attached to this event system.\n");
}

void EventManager::Publish(const EventType & event){

    auto& eventListeners = listeners[event];
    for (auto& element : eventListeners) {
        element->OnEvent(event);
    }

}