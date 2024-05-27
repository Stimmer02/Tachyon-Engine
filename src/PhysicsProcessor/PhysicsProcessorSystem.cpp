#include "PhysicsProcessorSystem.h"

PhysicsProcessorSystem::PhysicsProcessorSystem(){
    physicsProcessor = nullptr;
    errorFunction = nullptr;
    PPConfigPath = nullptr;
    TBO = 0;
}

PhysicsProcessorSystem::~PhysicsProcessorSystem(){
    delete physicsProcessor;
    if (errorFunction != nullptr){
        delete errorFunction;
    }
}

void PhysicsProcessorSystem::Share(SharedNameResolver* snr){
    TBO = ((Sprite2D*)(snr->Find("Sprite2D")))->GetTextureID();
    PPConfigPath = (std::string*)(snr->Find("PPConfigPath"));
    WindowContext* windowContext = (WindowContext*)(snr->Find("context"));
    if (windowContext != nullptr){
        log = windowContext->GetContextLogger();
        errorFunction = new std::function<void(std::string)>([&](std::string error){
            log->Write(LogMessageType::M_ERROR, error.c_str());
        });
    }
}

void PhysicsProcessorSystem::OnLoad(){
    if (TBO == 0){
        std::fprintf(stderr, "PhysicsProcessorSystem failed to find TBO\n");
        exit(-1);
    }
    if (PPConfigPath == nullptr){
        std::fprintf(stderr, "PhysicsProcessorSystem failed to find PPConfigPath\n");
        exit(-1);
    }

    PhysicsProcessorBuilder builder;
    if (builder.parseSystemConfig(*PPConfigPath) != 0){
        std::cout << builder.getError() << std::endl;
        return;
    }

    builder.setTBO(TBO);

    if (errorFunction != nullptr){
        builder.setClErrorFunction(errorFunction);
    }

    log->Write(LogMessageType::M_INFO, "Building PhysicsProcessor with config: %s", PPConfigPath->c_str());
    char errorCode = builder.build(true);
    if (errorCode != 0){
        log->Write(LogMessageType::M_ERROR, "PhysicsProcessor failed to build with error code: %d\n ERROR LOG:\n%s", errorCode, builder.getError().c_str());
        exit(errorCode);
    }
    log->Write(LogMessageType::M_INFO, "PhysicsProcessor built successfully");

    physicsProcessor = builder.getPhysicsProcessor();
}

void PhysicsProcessorSystem::OnUnload(){

}

void PhysicsProcessorSystem::Execute(){
    physicsProcessor->generateFrame();
}
