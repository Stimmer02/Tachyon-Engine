#include "PhysicsProcessor_Fallback.h"
#include "IPhysicsProcessor.h"

// Initializing GPU memory and allocating GPU.
void PhysicsProcessor_Fallback::allocateHostMemory(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, struct engineConfig config, cl::Device device){
    // Setting class variables.
    this->context = openCLContext;
    this->engine = engine;
    this->config = config;
    this->device = device;

    // Creating an OpenCL command queue.
    this->queue = cl::CommandQueue(openCLContext, device);

    // Creating an OpenCL memory object associated with OpenGL PBO (Pixel Buffer Object). Fallback exclusive difference with PhysicsProcessor.
    this->hostFallbackBuffer = new unsigned char[sizeof(color) * config.simulationWidth * config.simulationHeight];
    this->pbo_mem = clCreateBuffer(context(), CL_MEM_WRITE_ONLY, sizeof(color) * config.simulationWidth * config.simulationHeight, NULL, NULL);
    this->pbo_buff = cl::Buffer(pbo_mem);

    // Bind OpenGL buffer object (PBO) to the GL_ARRAY_BUFFER target. Fallback exclusive difference with PhysicsProcessor.
    glBindBuffer(GL_ARRAY_BUFFER, PBO);
}

// Defining data structures as a string.
std::string PhysicsProcessor_Fallback::structuresAsString(){
    std::string structures =
        "struct __attribute__ ((packed)) color{"
        "    unsigned char R;"
        "    unsigned char G;"
        "    unsigned char B;"
        "    unsigned char A;"
        "};"
        "struct __attribute__ ((aligned)) vector2D{"
        "    unsigned int x;"
        "    unsigned int y;"
        "};"
        "struct __attribute__ ((aligned)) voxel{"
        "    unsigned int substanceID;"
        "    struct vector2D forceVector;"
        "};"
        "struct __attribute__ ((aligned)) chunk{"
        "    struct voxel* voxels;"
        "};"
        "struct __attribute__ ((aligned)) substance{"
        "    struct color color;"
        "    float mass;"
        "    float jammingFactor;"
        "};"
        "struct __attribute__ ((aligned)) substanceTable{"
        "    struct substance* substances;"
        "};"
        "struct __attribute__ ((aligned)) engineResources{"
        "    struct substanceTable* substanceTable;"
        "    struct chunk* worldMap;"
        "    struct color* PBO;"
        "};"
        "struct __attribute__ ((aligned)) engineConfig{"
        "    uint simulationWidth;"
        "    uint simulationHeight;"
        "    float gravity;"
        "    float timefactor;"
        "    float atmosphereViscosity;"
        "};";

    return structures;
}

// Creating OpenCL kernel code as a string.
std::string PhysicsProcessor_Fallback::kernelCodeAsString(){
    std::string kernel_code =
        "    void kernel spawn_voxel(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){"
        "        resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.x = 0;"
        "        resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.y = 0;"
        "        resources->worldMap->voxels[y * config->simulationWidth + x].substanceID = substanceID;"
        "    }"
        ""
        "    void kernel set_chunk(global struct chunk* matrix, global struct voxel* voxels){"
        "        matrix->voxels = voxels;"
        "    }"
        "    void kernel set_substanceTable(global struct substanceTable* table, global struct substance* substances){"
        "        table->substances = substances;"
        "    }"
        "    void kernel set_engineResources(global struct engineResources* resources, global struct substanceTable* table, global struct chunk* matrix, global struct color* PBO){"
        "        resources->substanceTable = table;"
        "        resources->worldMap = matrix;"
        "       resources->PBO = PBO;"
        "    }"
                "    void kernel sum_voxel(global struct engineResources* resources, global uint* workArr, uint size, global uint* returnValue){"
        "        private uint id = get_global_id(0);"
        "        private uint dim = get_local_size(0);"
        "        private char dividionRest = size & 0x1;"
        "        private uint currentSize = size >> 1;"
        "        private uint index;"
        ""
        "        for (private uint i = id; i < currentSize; i += dim){"
        "           index = i << 1;"
        "            workArr[i] = (resources->worldMap->voxels[index].substanceID > 0) + (resources->worldMap->voxels[index+1].substanceID > 0);"
        "        }"
        "        barrier(CLK_LOCAL_MEM_FENCE);"
        "        if (dividionRest){"
        "            if (id == 0){"
        "               workArr[currentSize] = (resources->worldMap->voxels[currentSize << 1].substanceID > 0);"
        "           }"
        "           currentSize++;"
        "        }"
        "        barrier(CLK_LOCAL_MEM_FENCE);"
        "        dividionRest = currentSize & 0x1;"
        "        currentSize >>= 1;"
        ""
        "        while (currentSize > 1){"
        "            for (private uint i = id; i < currentSize; i += dim){"
        "               index = i << 1;"
        "               workArr[i] = workArr[index] + workArr[index+1];"
        "            }"
        "            barrier(CLK_LOCAL_MEM_FENCE);"
        "            if (dividionRest){"
        "               if (id == 0){"
        "                   workArr[currentSize] = workArr[currentSize << 1];"
        "               }"
        "               currentSize++;"
        "           }"
        "            barrier(CLK_LOCAL_MEM_FENCE);"
        "            dividionRest = currentSize & 0x1;"
        "            currentSize >>= 1;"
        "        }"
        "        barrier(CLK_LOCAL_MEM_FENCE);"
        "        if (id == 0){"
        "            *returnValue = workArr[0] + workArr[1];"
        "            if (dividionRest == 1){"
        "               *returnValue += workArr[2];"
        "            }"
        "       }"
        "   }"
        "    void kernel spawn_voxel_in_area(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){"
        "       uint globalID, IDX, IDY;"
        "       IDX = x + get_global_id(0);"
        "       IDY = y + get_global_id(1);"
        "       globalID = IDY * config->simulationWidth + IDX;"
        ""
        "        if (config->simulationWidth > IDX && config->simulationHeight > IDY){"
        "           resources->worldMap->voxels[globalID].forceVector.x = 0;"
        "           resources->worldMap->voxels[globalID].forceVector.y = 0;"
        "           resources->worldMap->voxels[globalID].substanceID = substanceID;"
        "        }"
        "   }";

    return kernel_code;
}

// Main construtror operations.
void PhysicsProcessor_Fallback::constructorMain(cl::Context openCLContext, struct engineConfig config, cl::Device device) {
    // Allocating GPU memory for various structures.
    cl::Buffer* voxels = new cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(struct voxel) * config.simulationWidth * config.simulationHeight);
    this->allocatedGPUMemory.push_back(voxels);

    cl::Buffer* chunk = new cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(struct chunk));
    this->allocatedGPUMemory.push_back(chunk);

    const uint substanceCount = 3;
    substance* tempSubstanceTable = new substance[substanceCount];

    tempSubstanceTable[0].jammingFactor = 0;
    tempSubstanceTable[0].mass = 0;
    tempSubstanceTable[0].color.R = 0;
    tempSubstanceTable[0].color.G = 0;
    tempSubstanceTable[0].color.B = 0;
    tempSubstanceTable[0].color.A = 0;

    tempSubstanceTable[1].jammingFactor = 1;
    tempSubstanceTable[1].mass = 1;
    tempSubstanceTable[1].color.R = 0;
    tempSubstanceTable[1].color.G = 128;
    tempSubstanceTable[1].color.B = 255;
    tempSubstanceTable[1].color.A = 255;

    tempSubstanceTable[2].jammingFactor = 1;
    tempSubstanceTable[2].mass = 1;
    tempSubstanceTable[2].color.R = 255;
    tempSubstanceTable[2].color.G = 255;
    tempSubstanceTable[2].color.B = 0;
    tempSubstanceTable[2].color.A = 255;

    cl::Buffer* substances = new cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(struct substance) * substanceCount);
    queue.enqueueWriteBuffer(*substances, true, 0, sizeof(struct substance) * substanceCount, tempSubstanceTable);
    this->allocatedGPUMemory.push_back(substances);

    cl::Buffer * substanceTable = new cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(struct substanceTable));
    this->allocatedGPUMemory.push_back(substanceTable);

    this->engineResources = cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(struct engineResources));
    this->eConfig = cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(struct engineConfig));
    queue.enqueueWriteBuffer(eConfig, true, 0, sizeof(struct engineConfig), &config);

    cl::Program::Sources sources;
    std::string structures = structuresAsString();
    std::string kernel_code = kernelCodeAsString();

    sources.push_back({structures.c_str(), structures.length()});
    sources.push_back({kernel_code.c_str(), kernel_code.length()});
    cl::Program program(openCLContext, sources);

    if (program.build() != CL_SUCCESS) { // Fallback exclusive difference with PhysicsProcessor.
        std::printf("Error building: %s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device).c_str());
        delete[] hostFallbackBuffer;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        exit(1);
    }

    // Setting OpenCL kernel arguments.
    cl::Kernel set_chunkKernel(program, "set_chunk");
    set_chunkKernel.setArg(0, *chunk);
    set_chunkKernel.setArg(1, *voxels);
    queue.enqueueNDRangeKernel(set_chunkKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));

    cl::Kernel set_substanceTableKernel(program, "set_substanceTable");
    set_substanceTableKernel.setArg(0, *substanceTable);
    set_substanceTableKernel.setArg(1, *substances);
    queue.enqueueNDRangeKernel(set_chunkKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));

    cl::Kernel set_engineResourcesKernel(program, "set_engineResources");
    set_engineResourcesKernel.setArg(0, this->engineResources);
    set_engineResourcesKernel.setArg(1, *substanceTable);
    set_engineResourcesKernel.setArg(2, *chunk);
    set_engineResourcesKernel.setArg(3, this->pbo_buff);

    // Fallback exclusive difference with PhysicsProcessor.
    queue.enqueueNDRangeKernel(set_chunkKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));
    queue.enqueueNDRangeKernel(set_substanceTableKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));
    queue.enqueueNDRangeKernel(set_engineResourcesKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));
    queue.enqueueReadBuffer(pbo_buff, CL_FALSE, 0, sizeof(color) * config.simulationWidth * config.simulationHeight, hostFallbackBuffer, NULL, NULL);
    queue.finish();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color) * config.simulationWidth * config.simulationHeight, hostFallbackBuffer);

    //Initializing the spawn_voxel_in_area kernel.
    this->spawn_voxelKernel = cl::Kernel(program, "spawn_voxel");
    this->spawn_voxel_in_areaKernel= cl::Kernel(program, "spawn_voxel_in_area");

    // Initializing the spawn_voxel kernel.
    this->spawn_voxelKernel = cl::Kernel(program, "spawn_voxel");

    // Initializing and seting up the sum_voxel kernel.
    cl::Buffer* sumWorkMemory = new cl::Buffer(openCLContext, CL_MEM_READ_WRITE, sizeof(cl_uint)*config.simulationHeight*config.simulationWidth/2+((config.simulationHeight*config.simulationWidth)&0x1));
    this->sumReturnValue = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(cl_uint));
    this->allocatedGPUMemory.push_back(sumWorkMemory);

    this->sum_voxelKernel = cl::Kernel(program, "sum_voxel");
    sum_voxelKernel.setArg(0, this->engineResources);
    sum_voxelKernel.setArg(1, *sumWorkMemory);
    sum_voxelKernel.setArg(2, config.simulationHeight * config.simulationWidth);
    sum_voxelKernel.setArg(3, this->sumReturnValue);


    // Freeing temporary memory.
    delete[] tempSubstanceTable;
}

// Setting arguments for the main (engine) kernel.
void PhysicsProcessor_Fallback::configureMainKernel(){
    this->engine.setArg(0, this->eConfig);
    this->engine.setArg(1, this->engineResources);
}

PhysicsProcessor_Fallback::PhysicsProcessor_Fallback(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, struct engineConfig config, cl::Device device){
    // Initializing GPU memory and allocating GPU.
    allocateHostMemory(openCLContext, engine, PBO, config, device);

    // Main construtror operations.
    constructorMain(openCLContext, config, device);

    // Setting arguments for the main (engine) kernel.
    configureMainKernel();
}

PhysicsProcessor_Fallback::~PhysicsProcessor_Fallback(){
    delete[] hostFallbackBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for (cl::Buffer* i :allocatedGPUMemory){
        delete i;
    }

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
    spawn_voxelKernel.setArg(3, this->engineResources);
    spawn_voxelKernel.setArg(4, this->eConfig);
    queue.enqueueNDRangeKernel(spawn_voxelKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));
    queue.finish();
}

uint PhysicsProcessor_Fallback::countVoxels(){
    cl_uint returnValue;
    queue.enqueueNDRangeKernel(sum_voxelKernel, cl::NullRange, cl::NDRange(256), cl::NDRange(256));
    queue.enqueueReadBuffer(sumReturnValue, CL_TRUE, 0, sizeof(cl_uint), &returnValue);
    queue.finish();

    return returnValue;
}

void PhysicsProcessor_Fallback::spawnVoxelInArea(uint x, uint y, uint width, uint height, uint substanceID){
    spawn_voxel_in_areaKernel.setArg(0, x);
    spawn_voxel_in_areaKernel.setArg(1, y);
    spawn_voxel_in_areaKernel.setArg(2, substanceID);
    spawn_voxel_in_areaKernel.setArg(3, this->engineResources);
    spawn_voxel_in_areaKernel.setArg(4, this->eConfig);
    queue.enqueueNDRangeKernel(spawn_voxel_in_areaKernel, cl::NullRange, cl::NDRange(width, height, 1), cl::NDRange(8, 8));
    queue.finish();
}
