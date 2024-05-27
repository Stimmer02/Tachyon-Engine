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

    this->resourceManager = snr;

    PPConfigPath = (const char*)(snr->Find("ppconfigpath"));
    WindowContext* windowContext = (WindowContext*)(snr->Find("context"));
    if (windowContext != nullptr){
        log = windowContext->GetContextLogger();
        errorFunction = new std::function<void(std::string)>([&](std::string error){
            log->Write(LogMessageType::M_ERROR, error.c_str());
            log->Flush();
        });
    }
}

void PhysicsProcessorSystem::OnLoad(){

    Sprite2D * sprite = (Sprite2D*)(resourceManager->Find("tbo"));

    if( sprite != nullptr)
        TBO = sprite->GetTextureID();

    if (TBO == 0){
        std::fprintf(stderr, "PhysicsProcessorSystem failed to find TBO\n");
        exit(-1);
    }
    if (PPConfigPath == nullptr){
        std::fprintf(stderr, "PhysicsProcessorSystem failed to find PPConfigPath\n");
        exit(-1);
    }

    PhysicsProcessorBuilder builder;
    if (builder.parseSystemConfig(PPConfigPath) != 0){
        log->Write(LogMessageType::M_INFO, "Building PhysicsProcessor error: %s", builder.getError().c_str());
        return;
    }

    builder.setTBO(TBO);

    if (errorFunction != nullptr){
        builder.setClErrorFunction(errorFunction);
    }

    log->Write(LogMessageType::M_INFO, "Building PhysicsProcessor with config: %s", PPConfigPath);
    char errorCode = builder.build(true);
    if (errorCode != 0){
        log->Write(LogMessageType::M_ERROR, "PhysicsProcessor failed to build with error code: %d\n ERROR LOG:\n%s", errorCode, builder.getError().c_str());
        exit(errorCode);
    }
    log->Write(LogMessageType::M_INFO, "PhysicsProcessor built successfully");

    log->Flush();

    physicsProcessor = builder.getPhysicsProcessor();
}

void PhysicsProcessorSystem::OnUnload(){

}

void PhysicsProcessorSystem::Execute(){
    physicsProcessor->generateFrame();
}
