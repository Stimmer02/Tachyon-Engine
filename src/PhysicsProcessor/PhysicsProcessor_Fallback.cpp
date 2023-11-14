#include "PhysicsProcessor_Fallback.h"

PhysicsProcessor_Fallback::PhysicsProcessor_Fallback(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device){
    this->context = openCLContext;
    this->engine = engine;
    this->config = config;
    this->device = device;
    
    this->queue = cl::CommandQueue(openCLContext, device);
    
    this->hostFallbackBuffer = new unsigned char[sizeof(color) * config.simulationWidth * config.simulationHeight];
    this->pbo_mem = clCreateBuffer(context(), CL_MEM_WRITE_ONLY, sizeof(color) * config.simulationWidth * config.simulationHeight, NULL, NULL);
    this->pbo_buff = cl::Buffer(pbo_mem);
    
    glBindBuffer(GL_ARRAY_BUFFER, PBO);
    
    this->voxels = cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(struct voxel) * config.simulationWidth * config.simulationHeight);
    this->size = 0;
    
    cl::Program::Sources sources;
    std::string structures =
        "struct vector2D{"
        "    unsigned int x;"
        "    unsigned int y;"
        "};"
        "struct voxel{"
        "    unsigned int substanceID;"
        "    struct vector2D forceVector;"
        "};";
    std::string kernel_code =
        "   void kernel spawn_voxel(uint x, uint y, uint substanceID, global struct voxel* matrix, uint dimX){"
        "       matrix[y * dimX + x]->vector2D.x = 0;"
        "       matrix[y * dimX + x]->vector2D.y = 0;"
        "       matrix[y * dimX + x]->substanceID = substanceID;"
        "   }";
    
    sources.push_back(structures);
    sources.push_back(kernel_code);
    cl::Program program(openCLContext, sources);
    
    if (program.build() != CL_SUCCESS) {
        std::printf("Error building: %s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device).c_str());
        exit(1);
    }
    
    this->spawn_voxelKernel = cl::Kernel(program, "spawn_voxel");
}

PhysicsProcessor_Fallback::~PhysicsProcessor_Fallback(){
    delete[] hostFallbackBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PhysicsProcessor_Fallback::generateFrame(){
    queue.enqueueNDRangeKernel(engine, cl::NullRange, cl::NDRange(config.simulationWidth, config.simulationHeight), cl::NDRange(16, 16));
    queue.enqueueReadBuffer(this->pbo_buff, CL_FALSE, 0, sizeof(color) * config.simulationWidth * config.simulationHeight, hostFallbackBuffer, NULL, NULL);
    queue.finish();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color) * config.simulationWidth * config.simulationHeight, hostFallbackBuffer);
}

void PhysicsProcessor_Fallback::spawnVoxel(uint x, uint y, uint substanceID){
    spawn_voxelKernel.setArg(0, x);
    spawn_voxelKernel.setArg(1, y);
    spawn_voxelKernel.setArg(2, substanceID);
    spawn_voxelKernel.setArg(3, this->voxels);
    spawn_voxelKernel.setArg(4, this->config.simulationHeight);
    queue.enqueueNDRangeKernel(spawn_voxelKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));
    queue.finish();
    
    ++size;
}

uint PhysicsProcessor_Fallback::countVoxels(){
	return size;
}
