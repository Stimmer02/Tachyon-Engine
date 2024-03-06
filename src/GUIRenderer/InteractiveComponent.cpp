#include "InteractiveComponent.h"


InteractiveComponent::InteractiveComponent(const int& x, const int& y, const int& width, const int& height) : Component(x, y, width, height){

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


void InteractiveComponent::OnEvent(const EventType& event){

    if(events.find(event)==events.end()){
        fprintf(stderr, "Can't find any callable function. \n");
        return ;
    }

    Function function = events[event];

    function();

}


