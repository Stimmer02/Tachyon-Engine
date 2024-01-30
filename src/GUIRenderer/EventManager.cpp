#include "EventManager.h"


void EventManager::Subscribe(const EventType & event, const IEventListener * listener){
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

void EventManager::Unsubscribe(const EventType & event, const IEventListener * listener){
    if(listener==NULL){
        fprintf(stderr, "Listener can't be null.\n");
        return;
    }

    auto& eventListeners = listeners[event];

    for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it) {
        if (*it == listener) {
            it = eventListeners.erase(it);
            return;
        }
    }

}

void EventManager::Publish(const EventType & event, const IEventListener * listener){
    if(listener==NULL){
        fprintf(stderr, "Listener can't be null.\n");
        return;
    }

    auto& eventListeners = listeners[event];

    for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it) {
        if (*it == listener) {
            ((IEventListener *)listener)->OnEvent(event);
            return;
        }
    }

}

void EventManager::Publish(const EventType & event){

    auto& eventListeners = listeners[event];
    for (auto& element : eventListeners) {
        ((IEventListener *)element)->OnEvent(event);
    }

}
