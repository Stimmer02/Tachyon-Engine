#ifndef PHYSICSPROCESSORSYSTEM_H
#define PHYSICSPROCESSORSYSTEM_H

#include "PhysicsProcessorBuilder.h"
#include "../ECSystem/System.h"
#include "../GUIRenderer/WindowContext.h"

class PhysicsProcessorSystem : public System{
public:
    friend class PhysicsProcessorBuilder;

    PhysicsProcessorSystem();
    ~PhysicsProcessorSystem();

    void Share(SharedNameResolver* snr) override;
    void OnLoad() override;
    void OnUnload() override;

private:
    void Execute() override;

    PhysicsProcessor* physicsProcessor;

    GLuint* TBO;
    std::string* PPConfigPath;
    std::function<void(std::string)>* errorFunction;
    ILog* log;
};

#endif