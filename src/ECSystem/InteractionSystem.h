#ifndef INTERACTIONSYSTEM_H
#define INTERACTIONSYSTEM_H

#include "System.h"
#include "WindowContext.h"
#include "MouseButtonMonitor.h"
#include "KeyboardMonitor.h"

#include <functional>

class InteractionSystem : public System{
private:

    WindowContext * context;
    MouseButtonMonitor * mouseMonitor;
    KeyboardMonitor * keyboardMonitor;

    void OnLoad() override{

    }

    void OnUnload() override{

    }

    void Execute() override{

    }

public:

    InteractionSystem(WindowContext * context, MouseButtonMonitor * mouseMonitor, KeyboardMonitor * keyboardMonitor) : System(){
        this->context = context;
        this->mouseMonitor = mouseMonitor;
        this->keyboardMonitor = keyboardMonitor;
    }

    void Share() override{

    }

    ~InteractionSystem(){

    }
};



#endif
