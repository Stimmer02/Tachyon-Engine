#ifndef _PHYSICSPROCSSORBUILDER_H
#define _PHYSICSPROCSSORBUILDER_H

#include "KernelBuilder.h"
#include "StructCollector.h"
#include "PhysicsProcessor.h"
#include "PhysicsProcessor_Fallback.h"

#include <stdint.h>

class PhysicsProcessorBuilder{
public:
    IPhysicsProcessor* build(KernelBuilder& kernelBuilder, StructCollector& structCollector, GLuint PBO, engineConfig config, uint platformID = 0, uint deviceID = 0);
    IPhysicsProcessor* build(const std::string kernelFragmentsDirectory, const std::string structsDirectory, uint32_t texure, engineConfig config, uint platformID = 0, uint deviceID = 0);
    static std::string getErrorString(cl_int error);
};

#endif
