#include "PhysicsProcessor_Fallback.h"

PhysicsProcessor::PhysicsProcessor_Fallback(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config){
    this->openCLContext = openCLContext;
    this->engine = engine;
    this->config = config;
}

void PhysicsProcessor_Fallback::generateFrame(){
    
}

void PhysicsProcessor_Fallback::spawnVoxel(uint x, uint y, uint substanceID){
    
}

uint PhysicsProcessor_Fallback::countVoxels(){
    
}