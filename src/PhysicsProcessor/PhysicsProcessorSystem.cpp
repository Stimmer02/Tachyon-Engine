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
        log->Flush();
        exit(-1);
    }

    builder.setTBO(TBO);

    if (errorFunction != nullptr){
        builder.setClErrorFunction(errorFunction);
    }

    log->Write(LogMessageType::M_INFO, "Building PhysicsProcessor with config: %s", PPConfigPath);
    char errorCode = builder.build(false);
    if (errorCode != 0){
        log->Write(LogMessageType::M_ERROR, "PhysicsProcessor failed to build with error code: %d\n ERROR LOG:\n%s", errorCode, builder.getError().c_str());
        log->Flush();
        exit(errorCode);
    }
    std::string error = builder.getError();
    if (error.length() > 0){
        log->Write(LogMessageType::M_ERROR, "PhysicsProcessor built with errors:\n%s", error.c_str());
        log->Flush();
    } else {
        log->Write(LogMessageType::M_INFO, "PhysicsProcessor built successfully");
        log->Flush();
    }

    physicsProcessor = builder.getPhysicsProcessor();


    // load simulation
    Configurator config(PPConfigPath);
    std::string simulationPath = "";
    config.ParseString("SIMULATION_SAVE_PATH", simulationPath, "");

    if (simulationPath == ""){
        log->Write(LogMessageType::M_INFO, "No simulation path found in config\n");
    } else {
        errorCode = physicsProcessor->loadSimulation(simulationPath);
        switch (errorCode){
            case 1:
                log->Write(LogMessageType::M_ERROR, "Failed to open simulation save file: %s\n", simulationPath.c_str());
                break;
            case 2:
                log->Write(LogMessageType::M_ERROR, "Simulation save file (%s) size does not match specified simulation size\n", simulationPath.c_str());
                break;
            default:
                log->Write(LogMessageType::M_INFO, "Simulation loaded successfully: %s\n", simulationPath.c_str());
                uint voxelCount = physicsProcessor->countVoxels();
                log->Write(LogMessageType::M_INFO, "Voxel count: %d\n", voxelCount);
                break;
        }
    }


    

    // physicsProcessor->spawnVoxelsInArea(512-128/2, 0, 128, 128, 2);
    // physicsProcessor->spawnVoxelsInArea(8, 1024-32, 1024-8, 24, 3);

    // physicsProcessor->spawnVoxelsInArea(0, 1024-8, 1024, 8, 1);
    // physicsProcessor->spawnVoxelsInArea(1024-8, 0, 8, 1024, 1);
    // physicsProcessor->spawnVoxelsInArea(0, 0, 8, 1024, 1);

    // voxelCount = physicsProcessor->countVoxels();
    // std::printf("Voxel count: %d\n", voxelCount);
}

void PhysicsProcessorSystem::OnUnload(){

}

void PhysicsProcessorSystem::Execute(){
    physicsProcessor->generateFrame();
    // uint voxelCount = physicsProcessor->countVoxels();
    // std::printf("Voxel count: %d\n", voxelCount);
}
