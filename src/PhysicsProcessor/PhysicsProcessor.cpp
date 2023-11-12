#include "PhysicsProcessor.h"

PhysicsProcessor::PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device){
    this->context = openCLContext;
    this->engine = engine;
    this->config = config;
    
    this->device = device;
    this->queue = cl::CommandQueue(openCLContext, device);
    
    cl_int error;
    
    this->pbo_mem = clCreateFromGLBuffer(context(), CL_MEM_WRITE_ONLY, PBO, &error);
    
    clEnqueueAcquireGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
    
    this->voxels = cl::Buffer buffer_dims(openCLContext, CL_MEM_READ_WRITE, sizeof(struct voxel) * config.simulationWidth * config.simulationHeight);
    this->size = 0;
    
    cl::Program::Sources sources;
    std::string color_structure =
        "   struct color {"
        "       unsigned char R, G, B;"
        "   };";
    std::string kernel_code =
        "   void kernel create_gradient(global struct color* A, global const int* dims, global const int* anim){"
        "       int IDX, IDY, global_ID, anim2;"
        "       float gradientStepX, gradientStepY;"
        ""
        "       IDX = get_global_id(0);"
        "       IDY = get_global_id(1);"
        "       if (IDX < dims[0] && IDY < dims[1]){"
        "           global_ID = IDX + IDY * get_global_size(0);"
        ""
        "           anim2 = *anim * (get_global_size(0) + get_global_size(1)) / 2 /1000;"
        "           gradientStepX  = 3.14159263f / (float)get_global_size(0);"
        "           gradientStepY  = 3.14159263f / (float)get_global_size(1);"
        ""
        "           A[global_ID].R = (sin((IDY+anim2)*gradientStepY) + 1) * 127;"
        "           A[global_ID].G = (cos((IDX+anim2*2)*gradientStepX) + 1) * 127;"
        "           A[global_ID].B = (sin(((IDY-anim2*2)*gradientStepY + (IDX-anim2)*gradientStepX)/2) + 1) * 127;"
        "       }"
        "   }";

    sources.push_back({color_structure.c_str(), color_structure.length()});
    sources.push_back({kernel_code.c_str(), kernel_code.length()});
    cl::Program program(openCLContext, sources);

    if (program.build() != CL_SUCCESS) {
        std::printf("Error building: %s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device).c_str());
        return cl::Program();
    }
    
    cl::Kernel create_gradient(program, "create_gradient");
    create_gradient.setArg(0, pbo_buff);
    create_gradient.setArg(1, buffer_dims);
    create_gradient.setArg(2, buffer_anim);
}

PhysicsProcessor::~PhysicsProcessor(){
    clEnqueueReleaseGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
}

void PhysicsProcessor::generateFrame(){
    queue.enqueueNDRangeKernel(create_gradient, cl::NullRange, cl::NDRange(config.simulationWidth, config.simulationHeight), cl::NDRange(config.simulationWidth, config.simulationHeight));
    queue.finish();
}

void PhysicsProcessor::spawnVoxel(uint x, uint y, uint substanceID){
    struct vector2D{
        cl_uint x;
        cl_uint y;
    } newVector2D;
    newVector2D.x = 0;
    newVector2D.y = 0;
    
    struct voxel{
        cl_uint substanceID;
        struct vector2D forceVector;
        // psisicStateVariables TODO next iteration
    } newVoxel;
    newVoxel.substanceID = substanceID;
    newVoxel.forceVector = newVector2D;
    
    ++size;
    
    memcpy(this->voxels[config.simulationWidth * x + y], newVoxel, sizeof(struct voxel));
}

uint PhysicsProcessor::countVoxels(){
	return size;
}
