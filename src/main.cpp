#include "PhysicsProcessor/PhysicsProcessorBuilder.h"


int main(){
    PhysicsProcessorBuilder builder;
    if (builder.parseSystemConfig("./config/system.cfg") != 0){
        std::cout << builder.getError() << std::endl;
        return -1;
    }
    builder.setPBO(1); //placeholder
    char errorCode = builder.build(true);
    if (errorCode != 0){
        std::cout << "ERROR CODE: " << int(errorCode) << '\n' << builder.getError() << std::endl;
        return -1;
    }
}