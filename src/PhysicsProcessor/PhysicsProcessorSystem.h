#ifndef PHYSICSPROCESSORSYSTEM_H
#define PHYSICSPROCESSORSYSTEM_H

#include "PhysicsProcessorBuilder.h"
#include "../ECSystem/System.h"
#include "../GUIRenderer/WindowContext.h"
#include "../ECSystem/SharedNameResolver.h"
#include "../GUIRenderer/Sprite2D.h"
#include "../GUIRenderer/Input.h"

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
    SharedNameResolver* resourceManager;
    GLuint TBO;
    const char* PPConfigPath;
    std::function<void(std::string)>* errorFunction;
    ILog* log;

    bool pause;
    bool pauseKeyPressed;
    Input* input;
};

#endif
