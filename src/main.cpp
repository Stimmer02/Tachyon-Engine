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

    std::string error = builder.getError();
    if (error != ""){
        std::cout << "BUILD SUCCESSFUL WITH ERRORS: \n";
        std::cout << error << std::endl;
    }

    PhysicsProcessor* physicsProcessor = builder.getPhysicsProcessor();

    uint count = physicsProcessor->countVoxels();
    std::cout << "pre-run count: " << count << std::endl;
    // physicsProcessor->generateFrame();
    count = physicsProcessor->countVoxels();
    std::cout << "post-run count: " << count << std::endl;


    delete physicsProcessor;
    return 0;
}