#include "InteractiveComponent.h"


InteractiveComponent::InteractiveComponent(const int& x, const int& y, const int& width, const int& height) : Component(x, y, width, height){
    this->UID = GUID++;
}

// void InteractiveComponent::Render(){
//     this->Render();
// }

void InteractiveComponent::AssignEvent(const EventType& event, Function delegate){
    if(events.find(event)!=events.end()){
        fprintf(stderr, "Function is already assigend.\n");
        return ;
    }

    events[event] = delegate;
}

uint32_t InteractiveComponent::GetGUID(){
    return UID;
}

void InteractiveComponent::OnEvent(const EventType& event){

    if(events.find(event)==events.end()){
        fprintf(stderr, "Can't find any callable function. \n");
        return ;
    }

    Function function = events[event];

    function();

}


