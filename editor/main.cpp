#include "MessageBus.h"

#include <stdio.h>

int main(){

    MessageBus bus;

    Message mess; 
    
    bus.SendToChannel("Hello", mess);

    return 0;
}
