#include "PhysicsProcessorBuilder.h"


int main(){
    PhysicsProcessorBuilder PPB;
    engineConfig config;

    PPB.build("./engine_kernel_fragments/", "./engine_structs/", 1, config, 0, 0);
}
