#include "PhysicsProcessor/PhysicsProcessorBuilder.h"
#include <filesystem>

GLFWwindow* initializeGLFW(uint height, uint width);
cl::Program compileCopyKernel(cl::Context context, cl::Device default_device);

void glfwErrorCallback(int error, const char* description);

uint localXsize = 16;
uint localYsize = 16;
int width = 1024, height = 1024;

GLuint PBO;
GLuint texture;
GLuint fboId;

IPhysicsProcessor* physicsProcessor;



int main(){

    //Initialize GLFW

    GLFWwindow* window = initializeGLFW(height, width);

    if (!window){
         glfwTerminate();
         return -1;
    }


    //Initialize glew

    if(glewInit() != GLEW_OK)
        return -1;

    //Create PBO and texutres

    glGenBuffers(1, &PBO);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, sizeof(color)*width*height, NULL, GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    //Initialize PhysicsProcessor
    engineConfig config;
    config.atmosphereViscosity = 1;
    config.gravity = 1;
    config.timefactor = 1;
    config.simulationHeight = height;
    config.simulationWidth = width;

    std::filesystem::path workingPath = std::filesystem::current_path();
    std::printf("Running in direcory: %s\n", workingPath.c_str());

    PhysicsProcessorBuilder PBB;
    physicsProcessor = PBB.build("./engine_kernel_fragments", "./engine_structs", PBO, config, 0, 0);
    if (physicsProcessor == nullptr){
        std::printf("Something went wrong...\n");
        return 1;
    }


    //Prepare simulation
    physicsProcessor->spawnVoxelInArea(0, 0, 8, config.simulationWidth, 1);
    physicsProcessor->spawnVoxelInArea(0, 0, config.simulationHeight, 8, 1);
    physicsProcessor->spawnVoxelInArea(config.simulationHeight-8, 0, 8, config.simulationWidth, 1);
    physicsProcessor->spawnVoxelInArea(0, config.simulationWidth-8, config.simulationHeight, 8, 1);

    GLuint error = 0;
    uint x = 0, y = 0;
    uint frames = 0;
            physicsProcessor->spawnVoxel(width>>1, 128, 2);
    while (!glfwWindowShouldClose(window)){

        physicsProcessor->generateFrame();

        glClear(GL_COLOR_BUFFER_BIT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glfwSwapBuffers(window);
        glfwPollEvents();


        if (frames % 10 == 0){
            uint voxelCount = physicsProcessor->countVoxels();
            std::printf("\33[2\rKF: %5d; V: %d", frames, voxelCount);
            fflush(stdout);
        }
        frames++;


        error = glGetError();
        if (error != GL_NO_ERROR) {
            std::printf("OpenGL error: %d\n", error);
        }
    }
    std::printf("\n");

    delete physicsProcessor;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteBuffers(1, &PBO);

    glDeleteTextures(1, &texture);

    glfwDestroyWindow(window);
}

GLFWwindow* initializeGLFW(uint width, uint height){
    if (!glfwInit()){
        std::printf("Failed to initialize GLFW!\n");
        return nullptr;
    }
    glfwSetErrorCallback(glfwErrorCallback);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(width, height, "test", NULL, NULL);
    if (!window){
        std::printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glViewport(0,0, width, height);
    glfwSwapInterval(1);

    return window;
}



void glfwErrorCallback(int error, const char* description){
    printf("Error: %s\n", description);
}
