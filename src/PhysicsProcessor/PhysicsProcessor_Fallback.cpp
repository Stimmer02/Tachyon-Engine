#include "PhysicsProcessor_Fallback.h"
#include "IPhysicsProcessor.h"

PhysicsProcessor_Fallback::PhysicsProcessor_Fallback(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, struct engineConfig config, cl::Device device){
    this->context = openCLContext;
    this->engine = engine;
    this->config = config;
    this->device = device;

    this->queue = cl::CommandQueue(openCLContext, device);

    this->hostFallbackBuffer = new unsigned char[sizeof(color) * config.simulationWidth * config.simulationHeight];
    this->pbo_mem = clCreateBuffer(context(), CL_MEM_WRITE_ONLY, sizeof(color) * config.simulationWidth * config.simulationHeight, NULL, NULL);
    this->pbo_buff = cl::Buffer(pbo_mem);

    glBindBuffer(GL_ARRAY_BUFFER, PBO);

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

    tempSubstanceTable[1].jammingFactor = 0;
    tempSubstanceTable[1].mass = 0;
    tempSubstanceTable[1].color.R = 100;
    tempSubstanceTable[1].color.G = 100;
    tempSubstanceTable[1].color.B = 110;
    tempSubstanceTable[1].color.A = 255;

    tempSubstanceTable[2].jammingFactor = 1;
    tempSubstanceTable[2].mass = 1;
    tempSubstanceTable[2].color.R = 255;
    tempSubstanceTable[2].color.G = 250;
    tempSubstanceTable[2].color.B = 162;
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
    std::string structures =
        "struct __attribute__ ((packed)) color{"
        "    unsigned char R;"
        "    unsigned char G;"
        "    unsigned char B;"
        "    unsigned char A;"
        "};"
        "struct __attribute__ ((aligned)) vector2D{"
        "    int x;"
        "    int y;"
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
    std::string kernel_code =
        "    void kernel spawn_voxel(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){"
        "        resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.x = 0;"
        "        resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.y = 0;"
        "        resources->worldMap->voxels[y * config->simulationWidth + x].substanceID = substanceID;"
        "    }"
        ""
        "    void kernel spawn_voxel_in_area(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){"
        "       uint globalID;"
        "       globalID = (y + get_global_id(1)) * config->simulationWidth + x + get_global_id(0);"
        ""
        "        if (config->simulationWidth > IDX && config->simulationHeight > IDY){"
        "           resources->worldMap->voxels[globalID].forceVector.x = 0;"
        "           resources->worldMap->voxels[globalID].forceVector.y = 0;"
        "           resources->worldMap->voxels[globalID].substanceID = substanceID;"
        "        }"
        ""
        "    void kernel set_chunk(global struct chunk* matrix, global struct voxel* voxels){"
        "        matrix->voxels = voxels;"
        "    }"
        "    void kernel set_substanceTable(global struct substanceTable* table, global struct substance* substances){"
        "        table->substances = substances;"
        "    }"
        "    void kernel set_engineResources(global struct engineResources* resources, global struct substanceTable* table, global struct chunk* matrix, global struct color* PBO){"
        "       resources->substanceTable = table;"
        "       resources->worldMap = matrix;"
        "       resources->PBO = PBO;"
        "    }";

    sources.push_back({structures.c_str(), structures.length()});
    sources.push_back({kernel_code.c_str(), kernel_code.length()});
    cl::Program program(openCLContext, sources);

    if (program.build() != CL_SUCCESS) {
        std::printf("Error building: %s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device).c_str());
        delete[] hostFallbackBuffer;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        exit(1);
    }

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

    queue.enqueueNDRangeKernel(set_chunkKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));
    queue.enqueueNDRangeKernel(set_substanceTableKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));
    queue.enqueueNDRangeKernel(set_engineResourcesKernel, cl::NullRange, cl::NDRange(1, 1, 1), cl::NDRange(1, 1, 1));
    queue.enqueueReadBuffer(pbo_buff, CL_FALSE, 0, sizeof(color) * config.simulationWidth * config.simulationHeight, hostFallbackBuffer, NULL, NULL);
    queue.finish();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color) * config.simulationWidth * config.simulationHeight, hostFallbackBuffer);

    this->spawn_voxelKernel = cl::Kernel(program, "spawn_voxel");
    this->spawn_voxel_in_area = cl::Kernel(program, "spawn_voxel_in_area");

    this->engine.setArg(0, this->eConfig);
    this->engine.setArg(1, this->engineResources);
    this->size = 0;

    delete[] tempSubstanceTable;
}

PhysicsProcessor_Fallback::~PhysicsProcessor_Fallback(){
    delete[] hostFallbackBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    for (cl::Buffer* i :allocatedGPUMemory){
        delete i;
    }

}

void PhysicsProcessor_Fallback::generateFrame(){
    queue.enqueueNDRangeKernel(engine, cl::NullRange, cl::NDRange(config.simulationWidth, config.simulationHeight), cl::NDRange(1, 256));
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

    ++size;
}

void PhysicsProcessor_Fallback::spawnVoxelInArea(uint x, uint y, uint width, uint height, uint substanceID){
    spawn_voxel_in_area.setArg(0, x);
    spawn_voxel_in_area.setArg(1, y);
    spawn_voxel_in_area.setArg(2, substanceID);
    spawn_voxel_in_area.setArg(3, this->engineResources);
    spawn_voxel_in_area.setArg(4, this->eConfig);
    queue.enqueueNDRangeKernel(spawn_voxel_in_area, cl::NullRange, cl::NDRange(width, height, 1), cl::NDRange(16, 16));
    queue.finish();

    ++size;
}

uint PhysicsProcessor_Fallback::countVoxels(){
	return size;
}
