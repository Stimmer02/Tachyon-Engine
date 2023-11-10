#include "PhysicsProcessor.h"

PhysicsProcessor::PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config){
    this->openCLContext = openCLContext;
    this->engine = engine;
    this->config = config;
}

void PhysicsProcessor::generateFrame(){
    
}

void PhysicsProcessor::spawnVoxel(uint x, uint y, uint substanceID){
    
}

uint PhysicsProcessor::countVoxels(){
    
}