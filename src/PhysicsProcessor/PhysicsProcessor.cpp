#include "PhysicsProcessor.h"
#include <CL/opencl.hpp>

PhysicsProcessor::PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device){
    this->context = openCLContext;
    this->config = config;
    this->device = device;
    this->engine = engine;

    this->queue = cl::CommandQueue(openCLContext, device);

    cl_int error;

    this->pbo_mem = clCreateFromGLBuffer(context(), CL_MEM_WRITE_ONLY, PBO, &error);
    this->pbo_buff = cl::Buffer(pbo_mem);

    clEnqueueAcquireGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);

    cl::Buffer* voxels = new cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(struct voxel) * config.simulationWidth * config.simulationHeight);

    this->allocatedGPUMemory.push_back(voxels);

    this->chunk = cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(struct chunk));

    this->size = 0;

    cl::Program::Sources sources;
    std::string structures =
        "struct __attribute__ ((aligned)) vector2D{"
        "    unsigned int x;"
        "    unsigned int y;"
        "};"
        "struct __attribute__ ((aligned)) voxel{"
        "    unsigned int substanceID;"
        "    struct vector2D forceVector;"
        "};"
        "struct __attribute__ ((aligned)) chunk{"
        "   struct voxel* voxels;"
        "};";
    std::string kernel_code =
        "   void kernel spawn_voxel(uint x, uint y, uint substanceID, global struct chunk* matrix, uint dimX){"
        "       matrix->voxels[y * dimX + x].forceVector.x = 0;"
        "       matrix->voxels[y * dimX + x].forceVector.y = 0;"
        "       matrix->voxels[y * dimX + x].substanceID = substanceID;"
        "   }"
        "   void kernel set_chunk(global struct chunk* matrix, global struct voxel* voxels){"
        "       matrix->voxels = voxels;"
        "   }";


    sources.push_back(structures);
    sources.push_back(kernel_code);
    cl::Program program(openCLContext, sources);


    if (program.build() != CL_SUCCESS) {
        std::printf("Error building: %s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device).c_str());
        clEnqueueReleaseGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
        exit(1);
    }


    cl::Kernel set_chunkKernel(program, "set_chunk");
    set_chunkKernel.setArg(0, this->chunk);
    set_chunkKernel.setArg(1, *voxels);
    queue.enqueueNDRangeKernel(set_chunkKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));

    this->spawn_voxelKernel = cl::Kernel(program, "spawn_voxel");

    this->engine.setArg(0, this->chunk);
    this->engine.setArg(1, this->pbo_buff);
}

PhysicsProcessor::~PhysicsProcessor(){
    clEnqueueReleaseGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
    for (cl::Buffer* i :allocatedGPUMemory){
        delete i;
    }
}

void PhysicsProcessor::generateFrame(){
    queue.enqueueNDRangeKernel(engine, cl::NullRange, cl::NDRange(config.simulationWidth, config.simulationHeight), cl::NDRange(16, 16));
    queue.finish();
}

void PhysicsProcessor::spawnVoxel(uint x, uint y, uint substanceID){
    spawn_voxelKernel.setArg(0, x);
    spawn_voxelKernel.setArg(1, y);
    spawn_voxelKernel.setArg(2, substanceID);
    spawn_voxelKernel.setArg(3, this->chunk);
    spawn_voxelKernel.setArg(4, this->config.simulationHeight);
    queue.enqueueNDRangeKernel(spawn_voxelKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));
    queue.finish();
    
    ++size;
}

uint PhysicsProcessor::countVoxels(){
	return size;
}
