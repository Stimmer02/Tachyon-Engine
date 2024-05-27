#include "PhysicsProcessorSystem.h"

PhysicsProcessorSystem::PhysicsProcessorSystem(){
    physicsProcessor = nullptr;
}

PhysicsProcessorSystem::~PhysicsProcessorSystem(){
    delete physicsProcessor;
}

void PhysicsProcessorSystem::Share(SharedNameResolver* snr){
    TBO = (GLuint*)(snr->Find("TBO"));
    PPConfigPath = (std::string*)(snr->Find("PPConfigPath"));
    windowContext = (WindowContext*)(snr->Find("context"));

    if (TBO == nullptr || PPConfigPath == nullptr || windowContext == nullptr){
        std::fprintf(stderr, "PhysicsProcessorSystem failed to find TBO, PPConfigPath, or context\n");
        exit(-1);
    }
}

void PhysicsProcessorSystem::OnLoad(){
    PhysicsProcessorBuilder builder;
    if (builder.parseSystemConfig(*PPConfigPath) != 0){
        std::cout << builder.getError() << std::endl;
        return;
    }
    builder.setTBO(*TBO);
    char errorCode = builder.build(true);
    if (errorCode != 0){
        std::cout << "ERROR CODE: " << int(errorCode) << '\n' << builder.getError() << std::endl;
        exit(errorCode);
    }

    physicsProcessor = builder.getPhysicsProcessor();
}

void PhysicsProcessorSystem::OnUnload(){
    
}

void PhysicsProcessorSystem::Execute(){
    physicsProcessor->generateFrame();
}