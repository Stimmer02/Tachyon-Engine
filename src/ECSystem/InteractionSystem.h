#ifndef INTERACTIONSYSTEM_H
#define INTERACTIONSYSTEM_H

#include "System.h"
#include "WindowContext.h"
#include "MouseButtonMonitor.h"

#include <functional>

class InteractionSystem : public System{
private:

    WindowContext * context;
    MouseButtonMonitor mouseMonitor;

    void OnLoad() override{

    }

    void OnUnload() override{

    }

    void Execute() override{

        EventInfo info = mouseMonitor.Query(GLFW_MOUSE_BUTTON_LEFT);

        if( info.type == EventType::ONTRIGGER)
            fprintf(stdout, "Mouse pressed\n");


        mouseMonitor.Update();

    }

public:

    InteractionSystem(WindowContext * context) : System(){

        this->context = context;
        context->ShareContext( &mouseMonitor );
    }

    void Share() override{

    }

    ~InteractionSystem(){

    }
};



#endif
